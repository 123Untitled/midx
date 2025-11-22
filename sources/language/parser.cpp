#include "language/parser.hpp"
#include "language/tokens/tokens.hpp"
#include "language/diagnostic.hpp"
#include "language/ast/tree.hpp"

#include <iostream>
#include "language/parser/table.hpp"

#include "atoi.hpp"
#include "math.hpp"

// -- P A R S E R -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
pr::parser::parser(void)
: _tree{nullptr},
  _tokens{nullptr},
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

	_tree->clear();
	_tree->tokens = &tokens;
	_idents.clear();

	// initialize iterators
	const auto fi = tokens.filtered();
	_it	 = fi.begin();
	_end = fi.end();

	_depth = 0U;
	_tempo = 1.0f;
	_back = false;

	// parse scope
	if (_it == _end) {
		std::cout << "No tokens to parse\n";
		return;
	}

	auto root = self::_parse();
	std::cout << "Parsing completed, root node index: " << root << '\n';

	as::printer::run(*_tree, root, tokens);
	//_tree->debug();
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



/* parse (private) */
auto pr::parser::_parse(void) -> mx::usz {

	auto p = as::make_node<as::program>(*_tree).index;
	_tree->mark();

	while (_it != _end) {
		std::cout << "\n-- PARSE LOOP --\nCurrent token: " << *_it << '\n';

		// skip separators
		do {
			if (_it.token().id != tk::separator)
				break;
		} while (++_it != _end);

		if (_parse_identifiers() == true)
			continue;

		auto expr = parse_expr<pr::level::expr>(pr::precedence::none);

		if (expr != 0U) {
			_tree->push(expr);
			continue;
		}

		if (_it == _end)
			break;

		error("Unexpected token in expression", _it);
		++_it;
	}

	auto& program = _tree->node<as::program>(p);
	program.range = _tree->flush();

	// compute max duration
	mx::usz dur = 0U;
	mx::usz it  = program.range.start;
	mx::usz end = it + program.range.count;

	for (; it < end; ++it) {
		const auto& h = _tree->remap_header(it);
		dur = dur < h.duration ? h.duration : dur;
	}
	program.header.duration = dur;

	return p;
}


/* parse identifiers */
auto pr::parser::_parse_identifiers(void) -> bool {

	if (_it == _end || _it.token().id != tk::identifier)
		return false;

	auto it = _it;
	while (++_it != _end && _it.token().id == tk::identifier);
	auto end = _it;

	const auto expr = parse_expr<pr::level::expr>(pr::precedence::none);

	while (it != end) {
		std::cout << "Mapping identifier '"
				  << it.view().first_chunk().lexeme
				  << "' to expr index " << expr << '\n';
		if (!_idents.insert(it.view().first_chunk().lexeme, expr)) {
			error("Duplicate identifier", it);
		}
		++it;
	}
	return true;
}



/* parse expr */
template <pr::level L>
auto pr::parser::parse_expr(const pr::precedence min_pre) -> mx::usz {
	depth_guard dg{_depth};

	debug_level<L>("PARSE*EXPR", "min pre", min_pre);

	mx::usz left = 0U;

	while (_it != _end) {
		debug_level<L>("nud loop", "current token", *_it);

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

		if constexpr (L == pr::level::seq)
			if (_back == true)
				return left;
	}


	// loop for led
	while (_it != _end) {
		debug_level<L>("led loop", "current token", *_it);

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

		if constexpr (L == pr::level::seq)
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

	debug("NUD atomic value", _it);

	const auto token_start = _it.index();
	const auto value_start = _tree->value_start();
	   mx::usz count       = 0U;


	do {

		// get token view
		const auto tv = *_it;

		switch (_it.token().id) {

			case tk::note:
				//++count;
				continue;

			case tk::binary:
				_tree->push_value(
					mx::convert_bin<false>[_last_param](tv, *_diag)
				);
				++count;
				continue;

			case tk::octal:
				_tree->push_value(
					mx::convert_oct<false>[_last_param](tv, *_diag)
				);
				++count;
				continue;

			case tk::decimal:
				_tree->push_value(
					mx::convert_dec<false>[_last_param](tv, *_diag)
				);
				++count;
				continue;

			case tk::hexadecimal:
				_tree->push_value(
					mx::convert_hex<false>[_last_param](tv, *_diag)
				);
				++count;
				continue;
		}

		break;

	} while (++_it != _end);

	if (count == 0U) // not necessarily, because nud called on valid token
		return left;

	// make atomic values node
	const auto av = as::make_node<as::atomic_values>(
								*_tree,
								token_start,
								value_start,
								count);

	if (left == 0U)
		return av.index;

	if (_tree->is_group(left))
		// extend group with new atomic values
		return _tree->extend_group(left, av.index), left;

	// make new group
	return _tree->make_group_with(left, av.index);
}






// -- N U D S -----------------------------------------------------------------


/* nud track reference */
auto pr::parser::nud_references(const mx::usz left) -> mx::usz {

	debug("NUD references", _it);

	mx::usz   dur = 0U;
	mx::usz count = 0U;
	const auto ref_start = _tree->ref_start();
	const auto tok_start = _it.index();


	do {

		// get lexeme
		const auto lex = _it.view().last_chunk().lexeme;

		// lookup identifier
		const auto found = _idents.find(lex);


		if (found.index != 0U) {
			++count;
			_tree->push_ref(found.index);

			// get header to accumulate duration
			const auto& h = _tree->header(found.index);
			dur += h.duration;
			continue;
		}

		if (found.empty != true)
			error("Undefined reference", _it);

	} while (++_it != _end
		&& _it.token().id == tk::reference);


	if (count == 0U)
		return left;

	// create new references node
	const auto r = as::make_node<as::references>(*_tree,
												 tok_start,
												 ref_start,
												 count,
												 dur);

	if (!left)
		return r.index;

	if (_tree->is_group(left)) {
		_tree->extend_group(left, r.index);
		return left;
	}

	// else make new group
	return _tree->make_group_with(left, r.index);
}


/* nud group */
template <pr::level L>
auto pr::parser::nud_group(const mx::usz left) -> mx::usz {

	debug("NUD group", _it);
	const auto it = _it;

	// consume opening parenthesis
	++_it;

	if constexpr (L == pr::level::seq) {
		if (lookahead(_it) == true) {
			_back = true;
			return left;
		}
	}

	// parse inner expression
    const auto inside = parse_expr<L>(pr::precedence::none);

	if (_it != _end && _it.token().id == tk::priority_close) {
		debug("NUD group", _it);
		++_it; // consume closing parenthesis
	}
	else error("Unclosed group", it);

	if (!inside)
		return left;

	if (!left)
		return inside;


	// CHECK IF LEFT IS SAME TYPE AS INSIDE ??

	// check if left is group
	if (_tree->is_group(left)) {
		_tree->extend_group(left, inside);
		return left;
	}

	// else make new group
	return _tree->make_group_with(left, inside);
}


/* nud permutation */
template <pr::level L>
auto pr::parser::nud_permutation(const mx::usz left) -> mx::usz {

	debug("NUD permutation", _it);
	const auto it = _it;

	// consume opening bracket
	++_it;

	if constexpr (L == pr::level::seq) {
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
	mx::usz perm = _tree->make_node<as::permutation>();
	auto& pref = _tree->node<as::permutation>(perm);

	// check for collapse
	if (_tree->is_collapsable(inside))
		pref.range = _tree->range_of(inside);

	else {
		// need remap index here
		pref.range = as::remap_range(_tree->new_remap(inside), 1U);
	}

	if (!left)
		return perm;

	// check if left is group
	if (_tree->is_group(left)) {
		_tree->extend_group(left, perm);
		return left;
	}

	// else make new group
	return _tree->make_group_with(left, perm);
}


/* nud parameter */
auto pr::parser::nud_parameter(mx::usz left) -> mx::usz {

	debug("NUD parameter", _it);

	static std::vector<mx::usz> params[pa::max_params];
	for (mx::usz i = 0U; i < pa::max_params; ++i)
		params[i].clear();
	bool has_params = false;


	do {

		tk::iterator it;

		// skip consecutive parameters
		do {
			it = _it; // copy and consume
		} while (++_it != _end
			&& _it.token().id == tk::parameter);



		// here check parameter name !
		_last_param = pa::to_id(*it);

		if (_last_param == pa::invalid) {
			error("Invalid parameter name", it);
			abord();
			break;
		}


		// recurse sequence parsing
		const mx::usz seq = parse_expr<pr::level::seq>(
								pr::precedence::none);

		if (seq) {

			// get header of seq
			const auto dur = _tree->header(seq).duration;

			// create new parameter
			const auto param =
				as::make_node<as::parameter>(*_tree, seq, *it, dur);

			//_tree.push(param.index);
			params[_last_param].push_back(param.index);
			has_params = true;
		}

		if (_back == true) {
			_back = false;
			--_it;
			break;
		}

	} while (_it != _end && _it.token().id == tk::parameter);

	// no parameters found
	if (!has_params)
		return left;

	// make track node
	const auto track = as::make_node<as::track>(*_tree);

	// loop over parameters and extend ranges
	for (mx::usz i = 0U; i < pa::max_params; ++i) {

		if (params[i].empty() == true)
			continue;

		// mark
		_tree->mark();
		// push all parameter nodes
		for (const auto p : params[i])
			_tree->push(p);
		// flush and set range
		track.ref.ranges[i] = _tree->flush();
	}


	const auto count = track.ref.ranges[pa::dura].count;

	mx::usz dur = 0U;
	if (count > 0U) {
		// for each child in duration parameter
		// accumulate duration
		for (mx::usz i = 0U; i < count; ++i) {
			const auto& h =
				_tree->remap_header(
					track.ref.ranges[pa::dura].start + i);
			dur += h.duration;
		}
		track.ref.header.duration = dur;
	}
	else {
		// no duration parameter, compute polyrythmic cycle duration

		const auto& ranges = track.ref.ranges;

		for (mx::usz i = 0U; i < pa::max_params; ++i) {

			const auto& range = ranges[i];
			if (range.count == 0U)
				continue;

			mx::usz p_dur = 0U;
			// for each child in parameter
			for (mx::usz j = 0U; j < range.count; ++j) {
				const auto& h = _tree->remap_header(range.start + j);
				p_dur += h.duration;
			}
			if (dur == 0U)
				dur = p_dur;
			else
				dur = mx::lcm(dur, p_dur);
		}
		track.ref.header.duration = dur;
	}





	if (!left)
		return track.index;

	// get header of left
	const auto& header = _tree->header(left);

	// check if left is group
	if (header.type == as::type::group) {
		_tree->extend_range_of(left, track.index); // HERE NEED TO CALL EXTEND GROUP
		return left;
	}

	else
		return _tree->make_group_with(left, track.index);
}


/* nud tempo */
template <pr::level L>
auto pr::parser::nud_tempo(mx::usz left) -> mx::usz {

	mx::usz num = 1U;
	mx::usz den = 1U;

	do { // accumulate consecutive tempo changes

		auto tv = *_it;
		switch (_it.token().id) {

			case tk::tempo_fast: {
				// conversion will be implemented later...
				num += 1;
				break;
			}

			case tk::tempo_slow: {
				// conversion will be implemented later...
				den += 1;
				break;
			}

			default:
				break;
		}

	} while (++_it != _end && (_it.token().id == tk::tempo_fast
						    || _it.token().id == tk::tempo_slow));

	// recurse right expression
	const auto right = parse_expr<L>(pr::precedence::none);

	if (!right)
		return left;

	// get right duration
	const auto dur = _tree->header(right).duration;

	const auto tempo = as::make_node<as::tempo>(*_tree, num, den, right, dur);

	if (!left)
		return tempo.index;

	// else make new group
	return _tree->make_group_with(left, tempo.index);
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

	// get current token
	const auto& tk = _it.token();

	do { // skip consecutive parallel operators
		++_it;
	} while (_it != _end && _it.token().id == tk::parallel);

	if (_it == _end)
		return left;

	if constexpr (L == pr::level::seq) {
		if (lookahead_op(_it) == true) {
			_back = true;
			return left;
		}
	}

	// recurse right expression
    auto right = parse_expr<L>(pr::pre_of<L>(tk));

	if (!right)
		std::cout << "LED parallel: right is 0\n";
	else
		std::cout << "LED parallel: right is " << _tree->what_is(right) << '\n';

	if (!left)
		std::cout << "LED parallel: left is 0\n";
	else
		std::cout << "LED parallel: left is " << _tree->what_is(left) << '\n';

	return  left == 0U ? right 
		: (right == 0U ? left
		// make parallel node by absorbing ranges
		: _tree->make_parallel(left, right));
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

	if constexpr (L == pr::level::seq) {
		if (lookahead_op(_it) == true) {
			_back = true;
			std::cout << "LED crossfade: lookahead found parameter or block ref, returning left\n";
			return left;
		}
	}

	// recurse right expression
	const auto right = parse_expr<L>(pr::pre_of<L>(tk));

	if (!right) return left;
	if (!left)  return right;

	// get left header
	const auto& lh = _tree->header(left);
	const auto& rh = _tree->header(right);

	const auto dur = lh.duration > rh.duration ? lh.duration : rh.duration;

	return as::make_node<as::crossfade>(*_tree, left, right, dur).index;
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

            case tk::reference:
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
			case tk::reference:
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
