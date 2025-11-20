#include "language/parser.hpp"
#include "language/tokens/tokens.hpp"
#include "language/diagnostic.hpp"
#include "language/ast/tree.hpp"

#include <iostream>
#include "language/parser/table.hpp"


// -- P A R S E R -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
pr::parser::parser(void)
: _tokens{nullptr},
  _diag{nullptr},
  _it{},
  _end{}
{
}


// -- public methods ----------------------------------------------------------

auto pr::parser::parse(tk::tokens&   tokens,
					   as::tree&       tree,
					   an::diagnostic& diag) -> void {

	// get references
	_tokens = &tokens;
	_tree   = &tree;
	_diag   = &diag;

	_arena.clear();

	// initialize iterators
	const auto fi = tokens.filtered();
	_it	 = fi.begin();
	_end = fi.end();

	_depth = 0U;
	_back = false;

	// parse scope
	if (_it == _end) {
		std::cout << "No tokens to parse\n";
		return;
	}

	auto root = parse_expr<pr::level::expression>(pr::precedence::none);

	if (!root) {
		std::cout << "No expression parsed\n";
		return;
	}
	else {
		std::cout << "Parsed expression:\n";
	}

	const auto& n = _arena.node(root);

	as::debug::run(_arena, n);
}

auto dbg_token(const char* msg, const tk::token& t) -> void {
	std::cout << msg << ": " << t << ' ' << &t << '\n';
}


// -- private methods ---------------------------------------------------------

auto pr::parser::error(const char* msg) const -> void {

	if (_it == _end) {
		_diag->push(msg, tk::range{});
		return;
	}

	tk::token_view tv = *_it;
	if (tv.num_chunks() == 0U)
		throw std::runtime_error{"Token has no chunks"};

	//_diag->push(msg, tv.first_chunk().range);
	_diag->push(msg, tk::range{});
}

auto pr::parser::error(const char* msg,
					   const tk::iterator& it) const -> void {

	it.token().id = tk::invalid;
	tk::token_view tv = *it;
	_diag->push(msg, tv.first_chunk().range);
}

struct depth_guard {
	mx::uint& d;
	depth_guard(mx::uint& depth) : d{depth} { ++d; }
	~depth_guard() { --d; }
};

template <pr::level L, typename... Tp>
auto debug_level(const Tp&... args) -> void {
	switch (L) {
		case pr::level::expression:
			std::cout << "[\x1b[32mlevel expression\x1b[0m]";
			break;

		case pr::level::sequence:
			std::cout << "[\x1b[31mlevel   sequence\x1b[0m]";
			break;
	}
	((std::cout << ' ' << args), ...);
	std::cout << '\n';
}


/* parse expr */
template <pr::level L>
auto pr::parser::parse_expr(const pr::precedence min_pre) -> mx::usz {
	depth_guard dg{_depth};

	debug_level<L>("parse_expr", "min pre", min_pre, "depth", _depth);

	mx::usz left = 0U;

	while (_it != _end) {

		auto& tk = _it.token();
		auto nud = pr::nud_of<L>(tk);

		// no NUD found, go to LED phase
		if (!nud)
			break;

		else
			std::cout << "\x1b[32mNUD\x1b[0m loop " << *_it << '\n';

		if (pr::can_start<L>(tk) == false) {
			error("Token cannot start an expression", _it);
			return 0U;
			break;
		}

		// consume token in nud function
		left = (this->*nud)(left);

		if constexpr (L == pr::level::sequence)
			if (_back == true)
				return left;
	}


	// loop for led
	while (_it != _end) {

		const auto& tk = _it.token();
		const auto led = pr::led_of<L>(tk);
		const auto pre = pr::pre_of<L>(tk);

		if (!led)
			break;

		std::cout << "\x1b[31mLED\x1b[0m loop " << *_it << '\n';

		if (pre <= min_pre)
			break;

		// consume token in led function
		left = (this->*led)(left);

		if (!left)
			return 0U;

		if constexpr (L == pr::level::sequence)
			if (_back == true)
				return left;

	}
	return left;
}

auto debug(const char* msg, const tk::iterator& it) -> void {
	//return;
	std::cout << msg << ": " << *it << "\n";
}


// -- N U D S -----------------------------------------------------------------

/* nud atomic value */
auto pr::parser::nud_atomic_value(mx::usz left) -> mx::usz {

	const auto start = _it.index();
	   mx::usz count = 1U;

	while (++_it != _end) {

		switch (_it.token().id) {
			case tk::note:
			case tk::binary:
			case tk::octal:
			case tk::decimal:
			case tk::hexadecimal:
				++count;
				continue;
			default:
				break;
		}
	}

	// make atomic values node
	const auto av = _arena.make_node(as::type::atomic_values, start, count);

	if (left == 0U) {
		left = _arena.make_node(as::type::sequence);
	}

	return _arena.append_to_sequence(left, av);
}




mx::usz pr::parser::nud_value(const mx::usz left) {

	const auto id = _it.token().id;

	// new marker
    _arena.mark();

	// left exists
    if (left) {

		// is sequence
		if (_arena.is_sequence(left)) {

			const auto& seq = _arena.node(left);
			_arena.push(seq.start, seq.count);
		}
		// is not sequence
		else { _arena.push(left); }
	}

	// push initial leaf
	_arena.push(
		_arena.make_node(as::type::leaf, *_it/*.token()*/)
	);

	// consume other values
	while (++_it != _end && _it.token().id == id/*tk::decimal*/) {
		_arena.push(
			_arena.make_node(as::type::leaf, *_it/*.token()*/));
	}

	if (left == 0U || !_arena.is_sequence(left)) {
		const auto [start, count] = _arena.flush();
		return _arena.make_node(as::type::sequence, start, count);
	}

	auto& seq = _arena.node(left);
	auto [start,count] = _arena.flush();
	seq.start = start;
	seq.count = count;
	return left;
}

// -- N U D S -----------------------------------------------------------------


/* nud group */
template <pr::level L>
auto pr::parser::nud_group(const mx::usz left) -> mx::usz {

	debug("NUD group", _it);
	const auto it = _it;

	// consume opening parenthesis
	++_it;

	if constexpr (L == pr::level::sequence) {
		if (lookahead(_it) == true) {
			_back = true;
			std::cout << "NUD group: lookahead found parameter or block ref, returning left\n";
			return left;
		}
	}

	// parse inner expression
    const auto inside = parse_expr<L>(
			//pr::precedence::tracksep
			pr::precedence::none
			);

	if (_it != _end && _it.token().id == tk::priority_close) {
		debug("NUD group", _it);
		++_it; // consume closing parenthesis
	}
	else error("Unclosed group", it);

	if (!inside) // 0 ()
		return left;

	if (!left)
		return inside;

	// check if left is sequence
	if (_arena.is_sequence(left))
		return _arena.append_to_sequence(left, inside);

	// else make new sequence
	return _arena.merge_as_sequence(left, inside);
}


/* nud permutation */
template <pr::level L>
auto pr::parser::nud_permutation(const mx::usz left) -> mx::usz {

	debug("NUD permutation", _it);
	const auto it = _it;

	// consume opening bracket
	++_it;

	if constexpr (L == pr::level::sequence) {
		if (lookahead(_it) == true) {
			_back = true;
			std::cout << "NUD group: lookahead found parameter or block ref, returning left\n";
			return left;
		}
	}

	// parse inner expression
    const auto inside = parse_expr<L>(pr::precedence::none);


	if (_it != _end && _it.token().id == tk::permutation_close) {
		debug("NUD permutation", _it);
		++_it; // consume closing permutation
	}
	else error("Unclosed permutation", it);

	if (!inside) // 0 []
		return left;

	 // make permutation node
	mx::usz perm = _arena.make_node(as::type::permutation);
	auto& pref = _arena.node(perm);

	// check if inside is sequence
	if (_arena.is_sequence(inside)) {
		const auto& seq = _arena.node(inside);
		pref.start = seq.start;
		pref.count = seq.count;
	}
	else {
		// need remap index here
		pref.start = _arena.new_remap(inside);
		pref.count = 1U;
	}

	if (!left)
		return perm;

	// check if left is sequence
	if (_arena.is_sequence(left))
		return _arena.append_to_sequence(left, perm);

	// else make new sequence
	return _arena.merge_as_sequence(left, perm);
}


/* nud parameter */
auto pr::parser::nud_parameter(mx::usz left) -> mx::usz {

	debug("NUD parameter", _it);

	// create new track
	auto track = _arena.make_node(as::type::track);

	do {

		tk::iterator it;

		// skip consecutive parameters
		do {
			it = _it++; // copy and consume
		} while (_it != _end
			&& _it.token().id == tk::parameter);

		// recurse sequence parsing
		const mx::usz seq = parse_expr<pr::level::sequence>(
								pr::precedence::none
				);

		if (seq) {
			// create new parameter
			mx::usz param = _arena.make_node(as::type::parameter,
											*it/*.token()*/);

			_arena.append_to_sequence(param, seq);
			_arena.append_to_sequence(track, param);
		}
		; int i = 0;

		if (_back == true) {
			_back = false;
			--_it;
			break;
		}

	} while (_it != _end && _it.token().id == tk::parameter);


	if (left) {

		if (_arena.is_sequence(left)) // work with permutations because permu is created at end
			return _arena.append_to_sequence(left, track);

		auto seq = _arena.make_node(as::type::sequence);
		_arena.append_to_sequence(seq, left);
		_arena.append_to_sequence(seq, track);
		return seq;
	}

	return track;
}





/* nud tempo */
template <pr::level L>
auto pr::parser::nud_tempo(mx::usz left) -> mx::usz {
    ++_it; // consume tempo token
	return left;
}


// -- L E D S -----------------------------------------------------------------


auto pr::parser::nud_track_separator(const mx::usz left) -> mx::usz {

	do { // skip consecutive track separators
		++_it;
	} while (_it != _end && _it.token().id == tk::track_separator);

	return left;
}


/* led parallel */
template <pr::level L>
auto pr::parser::led_parallel(const mx::usz left) -> mx::usz {

	debug("LED parallel", _it);

	const auto& tk = _it.token();

	do { // skip consecutive parallel operators
		++_it;
	} while (_it != _end && _it.token().id == tk::parallel);

	if (_it == _end)
		return left;

	if constexpr (L == pr::level::sequence) {
		if (lookahead_op(_it) == true) {
			_back = true;
			std::cout << "LED parallel: lookahead found parameter or block ref, returning left\n";
			return left;
		}
	}

    auto right = parse_expr<L>(pr::pre_of<L>(tk));


	if (left == 0U) {
		return right;
	}

	if (right == 0U) {
		return left;
	}
	return _arena.make_node(
				as::type::parallel,
				_arena.new_remap(left),
				_arena.new_remap(right)
	);
}


/* led crossfade */
template <pr::level L>
auto pr::parser::led_crossfade(const mx::usz left) -> mx::usz {

	debug("LED crossfade", _it);

	const auto& tk = _it.token();

	do { // skip consecutive crossfade operators
		++_it;
	} while (_it != _end && _it.token().id == tk::crossfade);

	if (_it == _end)
		return left;

	if constexpr (L == pr::level::sequence) {
		if (lookahead_op(_it) == true) {
			_back = true;
			std::cout << "LED crossfade: lookahead found parameter or block ref, returning left\n";
			return left;
		}
	}

	const auto right = parse_expr<L>(pr::pre_of<L>(tk));

	return left == 0U ? right :
		 (right == 0U ? left :
		   _arena.make_node(
			   as::type::crossfade,
			   _arena.new_remap(left),
			   _arena.new_remap(right)
		   ));
}



// -- L O O K A H E A D S -----------------------------------------------------

auto pr::parser::lookahead(tk::iterator it) const noexcept -> bool {

	// assume we are at the token after an opening token

	mx::usz depth = 1U;
	bool value_found = false;

	while (it != _end && depth > 0U) {
		const auto& tk = it.token();
		switch (tk.id) {
			case tk::priority_open:
			case tk::permutation_open:
				++depth;
				break;

			case tk::priority_close:
			case tk::permutation_close:
				--depth;
				break;

            case tk::track_reference:
			case tk::parameter: {
				if (value_found == true)
					error("mixed expression not allowed", it);
				return true;
			}

			case tk::decimal:
			case tk::hexadecimal:
			case tk::octal:
			case tk::binary:
			case tk::note:
				value_found = true;

			default:
				break;
		}
		++it;
	}

	return false;
}

auto pr::parser::lookahead_op(tk::iterator it) const noexcept -> bool {

	// assume we are at the token after the operator

	while (it != _end) {
		const auto& tk = it.token();
		switch (tk.id) {
			case tk::parameter:
			case tk::track_reference:
				return true;

			case tk::decimal:
			case tk::hexadecimal:
			case tk::octal:
			case tk::binary:
			case tk::note:
				return false;

			case tk::priority_open:
			case tk::permutation_open:
				++it;
				return lookahead_op(it);

			default:
				break;
		}
		++it;
	}


	return false;
}
