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

	as::tree_locator = &tree;

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

	auto p = as::make_node<as::program>().index;
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

		constexpr auto L = pr::level::expr;

		auto expr = parse_expr<L>(pr::precedence<L>::separator);

		if (expr != 0U) {
			_tree->push(expr);
			continue;
		}

		if (_it == _end)
			break;

		if (_it.token().id == tk::separator) {
			++_it;
			continue;
		}

		error("Unexpected token in expression", _it);
		++_it;
	}

	auto& program = _tree->node<as::program>(p);
	program.range = _tree->flush();

	// compute max duration
	mx::frac dur;

	mx::usz it  = program.range.start;
	mx::usz end = it + program.range.count;

	for (; it < end; ++it) {
		const auto& h = _tree->remap_header(it);
		dur = dur < h.dur ? h.dur : dur;
	}
	program.header.dur = dur;

	return p;
}


/* parse identifiers */
auto pr::parser::_parse_identifiers(void) -> bool {

	if (_it == _end || _it.token().id != tk::identifier)
		return false;

	auto it = _it;
	while (++_it != _end && _it.token() == tk::identifier);
	auto end = _it;

	constexpr auto L = pr::level::expr;

	const auto expr = parse_expr<L>(pr::precedence<L>::none);

	while (it != end) {
		if (!_idents.insert(it.view().first_chunk().lexeme, expr)) {
			error("Duplicate identifier", it);
		}
		++it;
	}
	return true;
}



/* parse expr */
template <pr::level L>
auto pr::parser::parse_expr(const mx::uint min_pre) -> mx::usz {
	depth_guard dg{_depth};

	debug_level<L>("PARSE EXPR", "min pre", min_pre);

	mx::usz left = 0U;

	while (_it != _end) {
		debug_level<L>("parse_expr loop", "current token", *_it);

		auto& tk = _it.token();
		auto eval = pr::eval_of<L>(tk);

		if (!eval)
			break;

		// precedence check
		if (pr::pre_of<L>(tk) <= min_pre)
			break;

		// consume token in nud function
		left = (this->*eval)(left);

		if constexpr (L == pr::level::seq)
			if (_back == true)
				break;
	}

	return left;
}

auto debug(const char* msg, const tk::iterator& it) -> void {
	//return;
	std::cout << msg << ": " << *it << "\n";
}



// -- N U D S -----------------------------------------------------------------

/* nud atomic value */
auto pr::parser::eval_atomic_value(mx::usz left) -> mx::usz {

	debug("NUD atomic value", _it);

	const auto tok_start = _it.index();
	const auto val_start = _tree->value_start();
	   mx::usz count     = 0U;

	do {

		// get token view
		const auto tv = *_it;

		switch (_it.token().id) {

			case tk::note:
			case tk::floating:
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
		return left; // just only for temporary not implemented 'note' case


	// make atomic values node
	const auto values = _tree->make_atomics(_last_param, tok_start, val_start, count);

	if (!left)
		return values;

	// extend existing group
	if (as::is_group(left)) {
		_tree->extend_group(left, values);
		return left;
	}

	// get left header
	const auto& lh = _tree->header(left);

	// make new group
	const auto range = _tree->make_range(left, values);
	const auto dur = mx::frac{count, 1U} + lh.dur;

	return _tree->make_group(range, dur);
}






// -- N U D S -----------------------------------------------------------------


/* nud track reference */
auto pr::parser::eval_references(const mx::usz left) -> mx::usz {

	debug("NUD references", _it);

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
			dur   += h.dur; // HERE CHECK OVERFLOW ??
			continue;
		}

		if (found.empty != true)
			error("Undefined reference", _it);

	} while (++_it != _end
		&& _it.token() == tk::reference);


	// no references found
	if (!count)
		return left;

	//dur *= _tempo;
	dur.reduce();

	// create new references node
	const auto refs = _tree->make_refs(tok_start, ref_start, count, dur);

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
	dur   += lh.dur;
	return _tree->make_group(range, dur.reduce());
}



/* nud group */
template <pr::level L>
auto pr::parser::eval_group(const mx::usz left) -> mx::usz {

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
    const auto inside = parse_expr<L>(pr::precedence<L>::none);

	if (_it != _end && _it.token() == tk::priority_close) {
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
	if (as::is_group(left)) {
		_tree->extend_group(left, inside);
		return left;
	}

	// else make new group
	return as::make_group(left, inside);
}


/* nud permutation */
template <pr::level L>
auto pr::parser::eval_permutation(const mx::usz left) -> mx::usz {

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
    const auto inside = parse_expr<L>(pr::precedence<L>::none);


	if (_it != _end && _it.token() == tk::permutation_close) {
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
		pref.local = _tree->header(inside).dur;
	}
	else {

		//if (_tree->is_atomic_values(inside)) {
		//}

		// need remap index here
		//perm = inside;
		pref.range = as::remap_range{_tree->new_remap(inside), 1U};
		pref.header.dur   = _tree->header(inside).dur;
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
		  auto dur   = lh.dur   + ph.dur;

	return _tree->make_group(range, dur.reduce());
}

/* nud parameter */
auto pr::parser::eval_parameter(mx::usz left) -> mx::usz {

	debug("NUD parameter", _it);

	mx::usz params[pa::max_params]{};
	bool has_params = false;

	do {

		tk::iterator it;

		// skip consecutive parameters
		do { it = _it; } while
			(++_it != _end && _it.token() == tk::parameter);


		// here check parameter name
		_last_param = pa::to_id(*it);

		if (_last_param == pa::invalid) {
			error("Invalid parameter name", it);
			continue;
		}

		constexpr auto L = pr::level::seq;

		// recurse sequence parsing
		const mx::usz seq = parse_expr<pr::level::seq>(
								pr::precedence<L>::parameter);

		if (seq) {

			// get header
			const auto& h = _tree->header(seq);

			// get duration
			const auto dur = h.dur;

			// check if already parameter of same type
			if (params[_last_param]) {

				// extend existing parameter
				_tree->extend_parameter(params[_last_param], seq);
				continue;
			}

			// create new parameter
			params[_last_param] = _tree->make_parameter(_tree->make_range(seq), dur);
			has_params = true;
			//continue;
		}

		if (_back == true) {
			_back = false;
			--_it;
			break;
		}

	} while (_it != _end && _it.token() == tk::parameter);


	// no parameters found
	if (!has_params)
		return left;


	// make track node
	const auto track = as::make_node<as::track>(params);

	// compute polyrythmic cycle duration

	mx::frac dur;

	for (int p = 0; p < pa::max_params; ++p) {

		const auto pnode = params[p];
		if (!pnode)
			continue;

		const auto& h = _tree->header(pnode);

		if (dur.num == 0U)
			dur = h.dur;
		else
			dur = mx::lcm_frac(dur, h.dur);
	}

	dur.reduce();
	track.ref.header.dur = dur;


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
	dur += lh.dur;

	return _tree->make_group(range, dur.reduce());
}


/* nud tempo */
template <pr::level L>
auto pr::parser::eval_tempo(mx::usz left) -> mx::usz {

	// save tempo
	auto old = _tempo;

	const auto frac_start = _tree->frac_start();
	const auto tok_start  = _it.index();
	mx::usz count = 0U;
	mx::frac dur;


	do { // accumulate consecutive tempo changes

		auto tv = *_it;
		switch (_it.token().id) {

			case tk::tempo_fast: {
				const auto& ck = tv.last_chunk();
				_tempo = mx::to_fraction(ck, *_diag).fix();
				break;
			}

			case tk::tempo_slow: {
				const auto& ck = tv.last_chunk();
				_tempo = mx::to_fraction(ck, *_diag).fix().invert();
				break;
			}

			default:
				// never reached
				continue;
		}

		_tempo.reduce();
		_tree->push_frac(_tempo);
		++count;

	} while (++_it != _end && (_it.token() == tk::tempo_fast
						    || _it.token() == tk::tempo_slow));

	std::cout << "\x1b[31mTEMPO\x1b[0m: " << _tempo << std::endl;

	mx::usz right;
	if (_it != _end && _it.token() == tk::modulo) {
		right = eval_modulo<L>(0);
	}
	else {
		right = parse_expr<L>(pr::precedence<L>::tempo);
	}

	// recurse right expression
	//const auto right = parse_expr<L>(pr::precedence<L>::tempo - 1U);


	// restore old tempo
	_tempo = old;

	if (!right || count == 0U) {
		// remove pushed fracs
		_tree->resize_fracs(frac_start);
		return left;
	}

	// get right header
	const auto& rh = _tree->header(right);

	// calculate total duration: sum of (child_dur / tempo) for each tempo
	for (mx::usz i = 0U; i < count; ++i) {
		const auto& tempo_frac = _tree->frac_at(frac_start + i);
		dur += rh.dur / tempo_frac;
	}

	dur.reduce();

	// make tempo node
	const auto tempo = _tree->make_node<as::tempo>(
										frac_start,
										tok_start,
										count, right, dur);

	if (!left)
		return tempo;

	// extend existing group
	if (as::is_group(left)) {
		_tree->extend_group(left, tempo);
		return left;
	}


	// get left header
	const auto& lh = _tree->header(left);

	// else make new group
	const auto range = _tree->make_range(left, tempo);
	dur   += lh.dur;

	return _tree->make_group(range, dur.reduce());
}

/* nud modulo */
template <pr::level L>
auto pr::parser::eval_modulo(mx::usz left) -> mx::usz {

	//tk::iterator it;

	const auto frac_start = _tree->frac_start();
	const auto tok_start  = _it.index();
	mx::usz count = 0U;
	mx::frac dur;

	// skip consecutive modulo operators
	do {
		// convert value
		auto d = mx::to_fraction(_it.view().last_chunk(), *_diag);

		if (d.num == 0U || d.den == 0U)
			continue;

		d.reduce();
		dur += d;

		_tree->push_frac(d);
		++count;

	} while (++_it != _end && _it.token() == tk::modulo);

	mx::usz right;

	if (_it != _end && (_it.token() == tk::tempo_fast
					 || _it.token() == tk::tempo_slow)) {
		right = eval_tempo<L>(0);
	}
	else {
		right = parse_expr<L>(pr::precedence<L>::modulo);
	}

	// recurse right expression
	//const auto right = parse_expr<L>(pr::precedence<L>::modulo - 1U);

	// check duration validity
	if (!right || dur.num == 0U || dur.den == 0U) {
		// remove pushed fracs
		_tree->resize_fracs(frac_start);
		return left;
	}

	dur.reduce();

	// make modulo node
	const auto mod = _tree->make_node<as::modulo>(
										frac_start,
										tok_start,
										count, right, dur);

	if (!left)
		return mod;


	// extend existing group
	if (as::is_group(left)) {
		_tree->extend_group(left, mod);
		return left;
	}

	// get left header
	const auto& lh = _tree->header(left);

	// else make new group
	const auto range = _tree->make_range(left, mod);
	dur += lh.dur;

	return _tree->make_group(range, dur.reduce());
}



//template <pr::level L>
//auto pr::parser::eval_tempo(mx::usz left) -> mx::usz {
//
//	// save tempo
//	auto old = _tempo;
//
//	_tempo = mx::frac{1U, 1U};
//
//	do { // accumulate consecutive tempo changes
//
//		auto tv = *_it;
//		switch (_it.token().id) {
//
//			case tk::tempo_fast: {
//				const auto& ck = tv.last_chunk();
//				_tempo *= mx::to_fraction(ck, *_diag).fix();
//				break;
//			}
//
//			case tk::tempo_slow: {
//				const auto& ck = tv.last_chunk();
//				_tempo *= mx::to_fraction(ck, *_diag).fix().invert();
//				break;
//			}
//
//			default:
//				break;
//		}
//
//	} while (++_it != _end && (_it.token() == tk::tempo_fast
//						    || _it.token() == tk::tempo_slow));
//
//	// reduce tempo frac
//	_tempo.reduce();
//
//
//	// recurse right expression
//	const auto right = parse_expr<L>(pr::precedence<L>::tempo - 1U);
//
//
//	const auto frac = _tempo;
//	// restore old tempo
//	_tempo = old;
//
//	if (!right)
//		return left;
//
//	// get right header
//	const auto rh = _tree->header(right);
//
//	auto dur   = rh.dur;
//
//
//	bool runtime;
//
//	if constexpr (L == pr::level::seq) {
//		runtime = false;
//	}
//	else {
//		dur /= frac; // SOLUTION HERE
//		runtime = true;
//	}
//
//
//	// make tempo node
//	const auto tempo = _tree->make_tempo(frac, runtime, right, dur);
//
//	if (!left)
//		return tempo;
//
//	// extend existing group
//	if (as::is_group(left)) {
//		_tree->extend_group(left, tempo);
//		return left;
//	}
//
//
//	// get left header
//	const auto& lh = _tree->header(left);
//
//	// else make new group
//	const auto range = _tree->make_range(left, tempo);
//	dur   += lh.dur;
//
//	return _tree->make_group(range, dur.reduce());
//}

// -- L E D S -----------------------------------------------------------------


auto pr::parser::eval_track_separator(const mx::usz left) -> mx::usz {

	// skip consecutive track separators
	do { ++_it; } while
		(_it != _end && _it.token() == tk::track_separator);

	return left;
}


/* led parallel */
template <pr::level L>
auto pr::parser::eval_parallel(const mx::usz left) -> mx::usz {

	// get current token
	const auto& tk = _it.token();

	// skip consecutive parallel operators
	do { ++_it; } while
		(_it != _end && _it.token() == tk::parallel);

	if (_it == _end)
		return left;

	if constexpr (L == pr::level::seq) {
		if (lookahead_op(_it) == true) {
			_back = true;
			std::cout << "LED parallel: lookahead found parameter or block ref, returning left\n";
			return left;
		}
	}

	// recurse right expression
    auto right = parse_expr<L>(pr::pre_of<L>(tk));

	return  left == 0U ? right 
		: (right == 0U ? left
		// make parallel node by absorbing ranges
		: _tree->make_parallel(left, right));
}


/* led crossfade */
template <pr::level L>
auto pr::parser::eval_crossfade(const mx::usz left) -> mx::usz {

	debug("LED crossfade", _it);

	const auto& tk = _it.token();

	do { // skip consecutive crossfade operators
		++_it;
	} while (_it != _end && _it.token() == tk::crossfade);

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

	return _tree->make_crossfade(left, right);
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

			case tk::floating:
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

auto pr::parser::lookahead_op(tk::iterator it) const /*noexcept*/ -> bool {

	// assume we are at the token after the operator

	while (it != _end) {
		const auto& tk = it.token();
		switch (tk.id) {
			case tk::parameter:
			case tk::reference:
				return true;

			case tk::floating:
			case tk::decimal:
			case tk::hexadecimal:
			case tk::octal:
			case tk::binary:
			case tk::note:
				return false;

			case tk::priority_open:
			case tk::permutation_open:
				++it;
				return lookahead(it);

			case tk::priority_close:
			case tk::permutation_close:
				return false;

			default:
				std::cout << "lookahead_op: skipping token " << tk << '\n';
				//throw std::runtime_error{"Unexpected token in lookahead_op"};
		}
		++it;
	}
	return false;
}
