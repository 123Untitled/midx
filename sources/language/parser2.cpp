//#include "language/parser.hpp"
//#include "language/tokens/tokens.hpp"
//#include "language/diagnostic.hpp"
//#include "language/ast/tree.hpp"
//
//#include <iostream>
//#include "language/parser/table.hpp"
//
//
//// -- P A R S E R -------------------------------------------------------------
//
//// -- public lifecycle --------------------------------------------------------
//
///* default constructor */
//pr::parser::parser(void)
//: _tokens{nullptr},
//  _diag{nullptr},
//  _it{},
//  _end{}
//{
//}
//
//
//// -- public methods ----------------------------------------------------------
//
//auto pr::parser::parse(tk::tokens&   tokens,
//					   as::tree&       tree,
//					   an::diagnostic& diag) -> void {
//
//	// get references
//	_tokens = &tokens;
//	_tree   = &tree;
//	_diag   = &diag;
//
//	_arena.clear();
//
//	// initialize iterators
//	const auto fi = tokens.filtered();
//	_it	 = fi.begin();
//	_end = fi.end();
//
//	_depth = 0U;
//	_new_track = true;
//
//	// parse scope
//	if (_it == _end) {
//		std::cout << "No tokens to parse\n";
//		return;
//	}
//
//	auto root = parse_expr(pr::precedence::none);
//
//	if (!root) {
//		std::cout << "No expression parsed\n";
//		return;
//	}
//	else {
//		std::cout << "Parsed expression:\n";
//	}
//
//	const auto& n = _arena.node(root);
//
//	as::debug::run(_arena, n);
//}
//
//auto dbg_token(const char* msg, const tk::token& t) -> void {
//	std::cout << msg << ": " << t << ' ' << &t << '\n';
//}
//
//
//// -- private methods ---------------------------------------------------------
//
//auto pr::parser::error(const char* msg) -> void {
//
//	if (_it == _end) {
//		_diag->push(msg, tk::range{});
//		return;
//	}
//
//	tk::token_view tv = *_it;
//	if (tv.num_chunks() == 0U)
//		throw std::runtime_error{"Token has no chunks"};
//
//	//_diag->push(msg, tv.first_chunk().range);
//	_diag->push(msg, tk::range{});
//}
//
//auto pr::parser::error(const char* msg,
//					   const tk::iterator& it) -> void {
//
//	it.token().id = tk::invalid;
//	tk::token_view tv = *it;
//	_diag->push(msg, tv.first_chunk().range);
//}
//
//struct depth_guard {
//	mx::uint& d;
//	depth_guard(mx::uint& depth) : d{depth} { ++d; }
//	~depth_guard() { --d; }
//};
//
//
///* parse expr */
//auto pr::parser::parse_expr(const pr::precedence min_pre) -> mx::usz {
//	depth_guard dg{_depth};
//
//	std::cout << "\x1b[32mparse_expr\x1b[0m [" << _depth << "]\n";
//	std::cout << "Min precedence: " << min_pre << '\n';
//
//	mx::usz left = 0U;
//
//	while (_it != _end) {
//
//		auto& tk = _it.token();
//		auto nud = pr::nud_of(tk);
//
//		//std::cout << " Current token: " << *_it << '\n';
//
//		// no NUD found, go to LED phase
//		if (!nud) {
//			//std::cout << " No NUD found\n";
//			break;
//		}
//		//std::cout << "\x1b[32mNUD\x1b[0m loop " << *_it << '\n';
//		//std::cout << " Found NUD\n";
//
//		if (pr::can_start(tk) == false) {
//			error("Token cannot start an expression", _it);
//			return 0U;
//			break;
//		}
//		//std::cout << " Can start expression\n";
//
//		// consume token in nud function
//		//const auto node = (this->*nud)(std::move(left));
//		//left = node != 0U ? node : left;
//		left = (this->*nud)(left);
//		if (!left) {
//			//std::cout << " NUD returned null\n";
//			//return 0U;
//			//break;
//		}
//		//else
//		//	std::cout << " NUD returned node\n";
//	}
//
//
//	// loop for led
//	while (_it != _end) {
//
//		const auto& tk = _it.token();
//		const auto led = pr::led_of(tk);
//		const auto pre = pr::pre_of(tk);
//
//		if (!led) {
//			//std::cout << " No LED found\n";
//			break;
//		}
//		//std::cout << "\x1b[31mLED\x1b[0m loop " << *_it << '\n';
//
//		if (pre <= min_pre)
//			break;
//
//		// appelle le handler LED
//		left = (this->*led)(left);
//
//		if (!left) {
//			//std::cout << " LED returned null\n";
//			return 0U;
//		}
//
//		// here check if left and right are compatible ?
//
//	}
//	//std::cout << "Returning from parse_expr\n";
//	return left;
//}
//
//auto debug(const char* msg, const tk::iterator& it) -> void {
//	//return;
//	std::cout << msg << ": " << *it << "\n";
//}
//
//
//// -- N U D S -----------------------------------------------------------------
//
//
//mx::usz pr::parser::nud_value(const mx::usz left) {
//
//	const auto id = _it.token().id;
//
//	// new marker
//    _arena.mark();
//
//	// left exists
//    if (left) {
//
//		// is sequence
//		if (_arena.is_sequence(left)) {
//
//			const auto& seq = _arena.node(left);
//			_arena.push(seq.start, seq.count);
//		}
//		// is not sequence
//		else { _arena.push(left); }
//	}
//
//	// push initial leaf
//	_arena.push(
//		_arena.make_node(as::type::leaf, _it.token())
//	);
//
//	// consume other values
//	while (++_it != _end && _it.token().id == id/*tk::decimal*/) {
//		_arena.push(
//			_arena.make_node(as::type::leaf, _it.token()));
//	}
//
//	if (left == 0U || !_arena.is_sequence(left)) {
//		const auto [start, count] = _arena.flush();
//		return _arena.make_node(as::type::sequence, start, count);
//	}
//
//	auto& seq = _arena.node(left);
//	auto [start,count] = _arena.flush();
//	seq.start = start;
//	seq.count = count;
//	return left;
//}
//
//
//auto pr::parser::lookahead(tk::iterator it) const noexcept -> bool {
//
//	mx::usz depth = 1U;
//
//	while (it != _end && depth > 0U) {
//		const auto& tk = it.token();
//		switch (tk.id) {
//			case tk::priority_open:
//			case tk::permutation_open:
//				++depth;
//				break;
//
//			case tk::priority_close:
//			case tk::permutation_close:
//				--depth;
//				break;
//
//            case tk::block_reference:
//			case tk::parameter:
//				return true;
//
//			default:
//				break;
//		}
//		++it;
//	}
//	return false;
//}
//
//// -- N U D S -----------------------------------------------------------------
//
//
///* nud group */
//auto pr::parser::nud_group(const mx::usz left) -> mx::usz {
//
//	debug("NUD group", _it);
//	const auto it = _it;
//
//
//
//	// consume opening parenthesis
//	++_it;
//
//	//if constexpr (L == pr::level::param
//	//		   || L == pr::level::sequence) {
//	//	if (lookahead(_it) == true) {
//	//		return 0;
//	//	}
//	//}
//
//	// parse inner expression
//    const auto inside = parse_expr(
//			//pr::precedence::tracksep
//			pr::precedence::none
//			);
//
//	if (_it != _end && _it.token().id == tk::priority_close) {
//		debug("NUD group", _it);
//		++_it; // consume closing parenthesis
//	}
//	else error("Unclosed group", it);
//
//	if (!inside) // 0 ()
//		return left;
//
//	if (!left)
//		return inside;
//
//	// check if left is sequence
//	if (_arena.is_sequence(left))
//		return _arena.append_to_sequence(left, inside);
//
//	// else make new sequence
//	return _arena.merge_as_sequence(left, inside);
//}
//
//
///* nud permutation */
//auto pr::parser::nud_permutation(const mx::usz left) -> mx::usz {
//
//	debug("NUD permutation", _it);
//	const auto it = _it;
//
//	// consume opening bracket
//	++_it;
//
//	// parse inner expression
//    const auto inside = parse_expr(pr::precedence::none);
//
//
//	if (_it != _end && _it.token().id == tk::permutation_close) {
//		debug("NUD permutation", _it);
//		++_it; // consume closing permutation
//	}
//	else error("Unclosed permutation", it);
//
//	if (!inside) // 0 []
//		return left;
//
//	 // make permutation node
//	mx::usz perm = _arena.make_node(as::type::permutation);
//	auto& pref = _arena.node(perm);
//
//	// check if inside is sequence
//	if (_arena.is_sequence(inside)) {
//		const auto& seq = _arena.node(inside);
//		pref.start = seq.start;
//		pref.count = seq.count;
//	}
//	else {
//		// need remap index here
//		pref.start = _arena.new_remap(inside);
//		pref.count = 1U;
//	}
//
//	if (!left)
//		return perm;
//
//	// check if left is sequence
//	if (_arena.is_sequence(left))
//		return _arena.append_to_sequence(left, perm);
//
//	// else make new sequence
//	return _arena.merge_as_sequence(left, perm);
//}
//
//
///* nud parameter */
//auto pr::parser::nud_parameter(mx::usz left) -> mx::usz {
//	++_it; // consume parameter token
//	return left;
//}
//
//
///* led parameter */
//auto pr::parser::led_parameter(mx::usz left) -> mx::usz {
//
//	std::cout << "LED parameter input left: " << as::what_is(_arena.node(left)) << "\n";
//
//    mx::usz track;
//
//	if (_arena.is_track(left)) {
//		track = left;
//	}
//	else if (left == 0U) {
//        // create new track node
//        track = _arena.make_node(as::type::track);
//	}
//	else {
//		std::cout << "\x1b[31merror\x1b[0m\n";
//		error("Parameter must follow a track", _it);
//		return 0U;
//    }
//
//	/* IMPLEMENTATION STEPS:
//	   keep last track list
//	   keep last param list
//
//	   and / or 
//	   
//	   use state and switch in this function
//	   led_parameter becomes nud_parameter
//	   !!!!!!
//
//	   */
//
//
//    // 2) create parameter node
//    mx::usz param = _arena.make_node(as::type::parameter, _it.token());
//    _arena.append_to_sequence(track, param);
//
//    ++_it; // consume :nt
//
//    // 3) parse the sequence belonging to this param
//    mx::usz seq = parse_expr(
//			precedence::parameter
//			);
//	std::cout << "[" << _depth << "] LED param: back from parse_expr: "
//			  << as::what_is(_arena.node(seq)) << '\n';
//
//    if (seq)
//        _arena.append_to_sequence(param, seq);
//
//    return track;
//}
//
//	//// recursive sequence parsing
//	//const auto seq = parse_expr<pr::level::sequence>(pr::precedence::none);
//	//std::cout << "back from sequence parse: " << seq << '\n';
//	//
//	//if (seq) {
//	//	_arena.append_to_sequence(param, seq);
//	//	//_arena.merge_as_sequence(param, seq);
//	//}
//	//
//	//
//	//return track;
//
//
//
///* nud tempo */
//auto pr::parser::nud_tempo(mx::usz left) -> mx::usz {
//    ++_it; // consume tempo token
//	return left;
//}
//
//
//// -- L E D S -----------------------------------------------------------------
//
//
//auto pr::parser::led_tracksep(const mx::usz left) -> mx::usz {
//
//	std::cout << "LED tracksep input left: " << as::what_is(_arena.node(left)) << "\n";
//
//	const auto& tk = _it.token();
//
//	do { // skip consecutive parallel operators
//		++_it;
//	} while (_it != _end && _it.token().id == tk::block_start);
//
//	if (_it == _end)
//		return left;
//
//	const auto right = parse_expr(
//			pr::precedence::none
//			//pr::pre_of(tk)
//			);
//
//	if (left == 0U) {
//		std::cout << "LED tracksep merging: null + "
//				  << as::what_is(_arena.node(right)) << "\n";
//		return right;
//	}
//
//	if (right == 0U) {
//		std::cout << "LED tracksep merging: "
//				  << as::what_is(_arena.node(left)) << " + null\n";
//		return left;
//	}
//
//	std::cout << "LED tracksep merging: "
//			  << as::what_is(_arena.node(left)) << " + "
//			  << as::what_is(_arena.node(right)) << "\n";
//
//	return _arena.merge_as_sequence(left, right);
//}
//
//
///* led parallel */
//auto pr::parser::led_parallel(const mx::usz left) -> mx::usz {
//
//	std::cout << "LED parallel input left: " << as::what_is(_arena.node(left)) << "\n";
//	debug("LED parallel", _it);
//
//	const auto& tk = _it.token();
//
//	do { // skip consecutive parallel operators
//		++_it;
//	} while (_it != _end && _it.token().id == tk::parallel);
//
//	if (_it == _end)
//		return left;
//
//    auto right = parse_expr(pr::pre_of(tk));
//
//	std::cout << "back from parallel parse: " << (right ? as::what_is(_arena.node(right)) : "null") << '\n';
//
//	if (left == 0U) {
//		std::cout << "LED parallel merging: null + "
//				  << as::what_is(_arena.node(right)) << "\n";
//		return right;
//	}
//
//	if (right == 0U) {
//		std::cout << "LED parallel merging: "
//				  << as::what_is(_arena.node(left)) << " + null\n";
//		return left;
//	}
//
//	std::cout << "LED parallel merging: "
//			  << as::what_is(_arena.node(left)) << " + "
//			  << as::what_is(_arena.node(right)) << "\n";
//
//	return _arena.make_node(
//				as::type::parallel,
//				_arena.new_remap(left),
//				_arena.new_remap(right)
//	);
//}
//
//
///* led crossfade */
//auto pr::parser::led_crossfade(const mx::usz left) -> mx::usz {
//
//	debug("LED crossfade", _it);
//
//	const auto& tk = _it.token();
//
//	do { // skip consecutive crossfade operators
//		++_it;
//	} while (_it != _end && _it.token().id == tk::crossfade);
//
//	if (_it == _end)
//		return left;
//
//	const auto right = parse_expr(pr::pre_of(tk));
//
//	return left == 0U ? right :
//		 (right == 0U ? left :
//		   _arena.make_node(
//			   as::type::crossfade,
//			   _arena.new_remap(left),
//			   _arena.new_remap(right)
//		   ));
//}
//
//
