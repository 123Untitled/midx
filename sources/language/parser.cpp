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
  _end{},
  _depth{0U},
  _back{false},
  _last_param{pa::invalid},
  _tempo{}
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
	//_factor = 1.0;
	_tempo.reset();
	_back = false;

	// parse scope
	if (_it == _end) {
		std::cout << "No tokens to parse\n";
		error("No tokens to parse");
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
	mx::usz steps = 0U;
	mx::frac dur;

	mx::usz it  = program.range.start;
	mx::usz end = it + program.range.count;

	for (; it < end; ++it) {
		const auto& h = _tree->remap_header(it);
		steps = steps < h.steps ? h.steps : steps;
		dur   = dur   < h.dur   ? h.dur   : dur;
	}
	program.header.steps = steps;
	program.header.dur   = dur;

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

	const auto tok_start = _it.index();
	const auto val_start = _tree->value_start();
	   mx::usz steps     = 0U;


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
				++steps;
				continue;

			case tk::octal:
				_tree->push_value(
					mx::convert_oct<false>[_last_param](tv, *_diag)
				);
				++steps;
				continue;

			case tk::decimal:
				_tree->push_value(
					mx::convert_dec<false>[_last_param](tv, *_diag)
				);
				++steps;
				continue;

			case tk::hexadecimal:
				_tree->push_value(
					mx::convert_hex<false>[_last_param](tv, *_diag)
				);
				++steps;
				continue;
		}

		break;

	} while (++_it != _end);

	if (steps == 0U) // not necessarily, because nud called on valid token
		return left; // just only for temporary not implemented 'note' case


	// make duration
	auto dur = apply_tempo(steps).reduce();

	// make atomic values node
	const auto values = _tree->make_atomics(_last_param,
											tok_start, val_start,
											steps, dur);

	if (!left)
		return values;

	// extend existing group
	if (_tree->is_group(left)) {
		_tree->extend_group(left, values);
		return left;
	}

	// get left header
	const auto& lh = _tree->header(left);

	// make new group
	const auto range = _tree->make_range(left, values);
	steps += lh.steps;
	dur   += lh.dur;

	return _tree->make_group(range, steps, dur.reduce());
}






// -- N U D S -----------------------------------------------------------------


/* nud track reference */
auto pr::parser::nud_references(const mx::usz left) -> mx::usz {

	debug("NUD references", _it);

	mx::usz steps = 0U;
	mx::usz count = 0U;
	mx::frac dur;
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
			steps += h.steps;
			dur   += h.dur; // HERE CHECK OVERFLOW ??
			continue;
		}

		if (found.empty != true)
			error("Undefined reference", _it);

	} while (++_it != _end
		&& _it.token().id == tk::reference);


	// no references found
	if (!count)
		return left;

	//dur *= _tempo;
	dur.reduce();

	// create new references node
	const auto refs = _tree->make_refs(tok_start, ref_start,
										count, steps, dur);

	if (!left)
		return refs;

	// extend existing group
	if (_tree->is_group(left)) {
		_tree->extend_group(left, refs);
		return left;
	}

	// get left header
	const auto& lh = _tree->header(left);

	// else make new group
	const auto range = _tree->make_range(left, refs);
	steps += lh.steps;
	dur   += lh.dur;
	return _tree->make_group(range, steps, dur.reduce());
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

	// extend existing group
	if (_tree->is_group(left)) {
		_tree->extend_group(left, inside);
		return left;
	}

	// get headers
	const auto& lh = _tree->header(left);
	const auto& ih = _tree->header(inside);

	const auto range = _tree->make_range(left, inside);
	const auto steps = lh.steps + ih.steps;
		  auto dur   = lh.dur   + ih.dur;

	return _tree->make_group(range, steps, dur.reduce());
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

	if (!inside)
		return left;

	 // make permutation node
	mx::usz perm = _tree->make_node<as::permutation>();
	auto& pref = _tree->node<as::permutation>(perm);

	// check for collapse
	if (_tree->is_collapsable(inside)) {
		pref.range = _tree->range_of(inside);
		pref.header.dur = _tree->product_duration(pref.range);
		pref.header.steps = _tree->product_steps(pref.range);
		pref.local = _tree->header(inside).dur;
	}
	else {

		//if (_tree->is_atomic_values(inside)) {
		//}

		// need remap index here
		//perm = inside;
		pref.range = as::remap_range{_tree->new_remap(inside), 1U};
		pref.header.dur   = _tree->header(inside).dur;
		pref.header.steps = _tree->header(inside).steps;
		pref.local = _tree->header(inside).dur;
	}

	if (!left)
		return perm;

	// extend existing group
	if (_tree->is_group(left)) {
		_tree->extend_group(left, perm);
		return left;
	}

	// get headers
	const auto& lh = _tree->header(left);
	const auto& ph = _tree->header(perm);


	// else make new group
	const auto range = _tree->make_range(left, perm);
	const auto steps = lh.steps + ph.steps;
		  auto dur   = lh.dur   + ph.dur;

	return _tree->make_group(range, steps, dur.reduce());
}


/* nud parameter */
auto pr::parser::nud_parameter(mx::usz left) -> mx::usz {

	debug("NUD parameter", _it);

	mx::usz params[pa::max_params]{};
	bool has_params = false;


	do {

		tk::iterator it;

		// skip consecutive parameters
		do {
			it = _it; // copy and consume
		} while (++_it != _end
			&& _it.token().id == tk::parameter);


		// here check parameter name
		_last_param = pa::to_id(*it);

		if (_last_param == pa::invalid) {
			error("Invalid parameter name", it);
			continue;
		}


		// recurse sequence parsing
		const mx::usz seq = parse_expr<pr::level::seq>(
								pr::precedence::none);

		if (seq) {

			// get header
			const auto& h = _tree->header(seq);

			// get steps
			const auto steps = h.steps;
			const auto dur   = h.dur;

			// check if already parameter of same type
			if (params[_last_param]) {

				// extend existing parameter
				_tree->extend_parameter(params[_last_param], seq);
				continue;
			}

			// create new parameter
			params[_last_param] = _tree->make_parameter(_tree->make_range(seq),
														steps, dur);
			has_params = true;
			continue;
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
	const auto track = as::make_node<as::track>(*_tree, params);


	{
		const auto dura = params[pa::dura];

		if (false /* dura */) {

			// get dura header
			const auto& h = _tree->header(dura);

			track.ref.header.steps = h.steps;
			track.ref.header.dur   = h.dur;
		}
		else {

			// no duration parameter, compute polyrythmic cycle duration

			mx::frac dur = mx::frac{0,1};
			mx::usz steps = 0U;

			for (int p = 0; p < pa::max_params; ++p) {

				const auto pnode = params[p];
				if (!pnode)
					continue;

				const auto& h = _tree->header(pnode);

				if (dur.num == 0)
					dur = h.dur;
				else
					dur = mx::lcm_frac(dur, h.dur);

				if (steps == 0U)
					steps = h.steps;
				else
					steps = mx::lcm(steps, h.steps);
			}

			dur.reduce();
			track.ref.header.steps = steps;
			track.ref.header.dur = dur;
		}
	}


	if (!left)
		return track.index;

	// extend existing group
	if (_tree->is_group(left)) {
		_tree->extend_group(left, track.index);
		return left;
	}

	// get left header
	const auto& lh = _tree->header(left);

	// else make new group
	const auto range = _tree->make_range(left, track.index);
	const auto steps = lh.steps + track.ref.header.steps;
		  auto dur   = lh.dur   + track.ref.header.dur;

	return _tree->make_group(range, steps, dur.reduce());
}


/* nud tempo */
template <pr::level L>
auto pr::parser::nud_tempo(mx::usz left) -> mx::usz {

	// save tempo
	auto old = _tempo;

	mx::frac temp{1, 1};


	do { // accumulate consecutive tempo changes

		auto tv = *_it;
		switch (_it.token().id) {

			case tk::tempo_fast: {
				const auto& ck = tv.last_chunk();
				//_tempo *= mx::to_fraction(ck, *_diag).fix();
				auto f = mx::to_fraction(ck, *_diag).fix();
				temp *= f;
				_tempo *= f;
				break;
			}

			case tk::tempo_slow: {
				const auto& ck = tv.last_chunk();
				//_tempo *= mx::to_fraction(ck, *_diag).fix().invert();
				auto f = mx::to_fraction(ck, *_diag).fix().invert();
				temp *= f;
				_tempo *= f;
				break;
			}

			default:
				break;
		}

	} while (++_it != _end && (_it.token().id == tk::tempo_fast
						    || _it.token().id == tk::tempo_slow));

	// reduce tempo frac
	_tempo.reduce();
	// reduce temp frac
	temp.reduce();

	//mx::frac temp = _tempo;


	std::cout << "\x1b[32mTEMPO\x1b[0m: frac=" << _tempo << '\n';


	// recurse right expression
	const auto right = parse_expr<L>(pr::precedence::none);

	// restore old tempo
	_tempo = old;

	if (!right)
		return left;

	// get right header
	const auto rh = _tree->header(right);

	auto dur   = rh.dur;
	auto steps = rh.steps;


	bool runtime;

	if constexpr (L == pr::level::seq) {
		runtime = false;
	}
	else {
		dur /= temp; // SOLUTION HERE
		runtime = true;
	}


	// make tempo node
	const auto tempo = _tree->make_tempo(temp, runtime, right, steps, dur);

	if (!left)
		return tempo;


	// get left header
	const auto& lh = _tree->header(left);

	// else make new group
	const auto range = _tree->make_range(left, tempo);
	steps += lh.steps;
	dur   += lh.dur;

	return _tree->make_group(range, steps, dur.reduce());
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
		std::cout << "LED parallel: right is 0U\n";
	else
		std::cout << "LED parallel: right is " << _tree->what_is(right) << " node\n";

	if (!left)
		std::cout << "LED parallel: left is 0U\n";
	else
		std::cout << "LED parallel: left is " << _tree->what_is(left) << " node\n";

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

	const auto steps = lh.steps > rh.steps ? lh.steps : rh.steps;

	return _tree->make_crossfade(left, right, steps);
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
