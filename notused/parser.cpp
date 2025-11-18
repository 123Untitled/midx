




//#include "language/parser.hpp"
//#include "language/tokens/tokens.hpp"
//#include "language/diagnostic.hpp"
//
//#include <iostream>
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
//  _end{},
//  _depth{0U},
//  _out{}
//{
//}
//
//
//// -- public methods ----------------------------------------------------------
//
//auto pr::parser::parse(tk::tokens&   tokens,
//					   an::diagnostic& diag) -> void {
//
//	// get references
//	_tokens = &tokens;
//	_diag   = &diag;
//
//	// initialize iterators
//	const auto fi = tokens.filtered();
//	_it	 = fi.begin();
//	_end = fi.end();
//
//	_depth = 0U;
//
//
//	// parse scope
//	if (_it == _end) {
//		std::cout << "No tokens to parse\n";
//		return;
//	}
//
//	parse_scope();
//
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
//	if (_it != _end && _it.token().id != tk::end_of_tokens) {
//
//		tk::token_view tv = *_it;
//		if (tv.num_chunks() == 0U)
//			throw std::runtime_error{"Token has no chunks"};
//
//		_diag->push(msg, tv.first_chunk().range);
//		return;
//	}
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
//
//
//
///* parse global */
//auto pr::parser::parse_scope(void) -> void {
//	std::cout << "[SCOPE]" << std::endl;
//
//
//	while (_it != _end) {
//
//		switch (_it.token().id) {
//
//			// skip separators
//			case tk::separator:
//				++_it; // maybe emit separator token ??
//				continue;
//
//			// assignment
//			case tk::identifier: {
//				if (++_it == _end || _it.token().id != tk::assignment) {
//					error("Expected assignment operator '=' after identifier",
//							_it - 1);
//					panic_expression();
//					continue;
//				}
//				emit(pr::assignments, _it.index() - 1U, 2U);
//				++_it;
//				// fallthrough to expression
//			}
//
//			default:
//				parse_expression();
//		}
//
//	}
//}
//
//auto pr::parser::parse_expression(void) -> void {
//	std::cout << "[EXPRESSION]" << std::endl;
//
//	while (_it != _end) {
//
//		switch (_it.token().id) {
//
//			// end of expression
//			case tk::separator:
//				++_it; // maybe emit separator token ??
//			case tk::identifier:
//				// back to scope
//				return;
//
//
//			case tk::tempo_fast:
//			case tk::tempo_slow:
//				++_it; // implemented later
//				break;
//
//			case tk::parallel:
//				emit(pr::block_parallel, _it.index(), 1U);
//				++_it;
//				break;
//
//			case tk::crossfade:
//				emit(pr::block_crossfade, _it.index(), 1U);
//				++_it;
//				break;
//
//			case tk::block_start: {
//				++_it;
//				// skipping
//				break;
//			}
//
//			case tk::parameter: {
//				++_it;
//				break;
//			}
//
//			case tk::block_reference:
//				emit(pr::block_references, _it.index(), 1U);
//				++_it;
//				break;
//
//			case tk::priority_open:
//				emit(pr::block_group_in, _it.index(), 1U);
//				++_it;
//				break;
//			case tk::permutation_open:
//				emit(pr::block_perm_in, _it.index(), 1U);
//				++_it;
//				break;
//
//
//			default:
//				error("Unexpected token in block");
//				panic_expression();
//				return;
//		}
//	}
//}
//
//
//
//auto pr::parser::parse_block(void) -> void {
//	std::cout << "[BLOCK]" << std::endl;
//
//	while (_it != _end) {
//
//		switch (_it.token().id) {
//
//			// end of expression
//			case tk::separator:
//				++_it; // maybe emit separator token ??
//			case tk::identifier:
//				// back to scope
//				return;
//
//
//			case tk::tempo_fast:
//			case tk::tempo_slow:
//				++_it;
//				break;
//
//			case tk::parallel:
//			case tk::crossfade:
//				++_it;
//				break;
//
//			case tk::block_start: {
//				++_it;
//				break;
//			}
//
//			case tk::parameter: {
//				++_it;
//				break;
//			}
//
//			case tk::block_reference:
//				++_it;
//				break;
//
//			case tk::priority_open:
//			case tk::permutation_open:
//				++_it;
//				break;
//
//
//			default:
//				error("Unexpected token in block");
//				panic_expression();
//				return;
//		}
//	}
//}
//
//
///* parse param */
//auto pr::parser::parse_param(void) -> void {
//	std::cout << "parsing param" << std::endl;
//
//	// assume current token is a parameter
//
//	// skip consecutive parameters
//	// last is the real parameter
//	do {
//		if (++_it == _end)
//			return;
//	} while (_it.token().id == tk::parameter);
//
//	// emit parameter token
//	emit(pr::parameter, _it.index() - 1U, 1U);
//
//	//:tr :nt ([:
//
//
//	switch (_it.token().id) {
//
//		case tk::note:
//		case tk::binary:
//		case tk::octal:
//		case tk::decimal:
//		case tk::hexadecimal:
//			merge_values();
//			break;
//
//
//		default:
//			return;
//	}
//}
//
//
///* merge assignments */
//auto pr::parser::merge_assignments(void) -> void {
//	std::cout << "-> merge assignment" << std::endl;
//
//	// assume first token is an identifier
//
//	const mx::usz start = _it.index();
//		  mx::usz count = 1U;
//	   bool wait_assign = true;
//
//	while ((++_it).is_end() == false) {
//
//		if (wait_assign == false) {
//
//			if (_it.token().id != tk::identifier)
//				break;
//
//			++count;
//			wait_assign = true;
//			continue;
//		}
//
//
//		if (_it.token().id == tk::assignment) {
//			++count;
//			wait_assign = false;
//			continue;
//		}
//
//		error("Expected assignment operator '=' after identifier");
//		panic_expression();
//		return;
//	}
//
//	// emit assignment
//	emit(pr::assignments, start, count);
//	// block parser will be pushed by scope parser
//}
//
//
///* merge values */
//auto pr::parser::merge_values(void) noexcept -> void {
//
//	// assume first token is a value
//	++_it;
//
//	const mx::usz start = _it.index();
//	mx::usz count = 1U;
//
//	while (_it != _end) {
//
//		auto& t = _it.token();
//
//		switch (t.id) {
//
//			case tk::note:
//			case tk::binary:
//			case tk::octal:
//			case tk::decimal:
//			case tk::hexadecimal:
//				++count;
//				++_it;
//				continue;
//		}
//		// not a value token
//		break;
//	}
//
//	// emit values token
//	emit(pr::values, start, count);
//}
//
//
///* merge references */
//auto pr::parser::merge_references(void) noexcept -> void {
//
//	// assume first token is a reference
//
//	const mx::usz start = _it.index();
//	mx::usz count = 1U;
//
//	while (++_it != _end) {
//
//		if (_it.token().id != tk::block_reference)
//			break;
//		++count;
//	}
//
//	// emit references token
//	emit(pr::block_references, start, count);
//}
//
//
//
///* panic expression */
//auto pr::parser::panic_expression(void) noexcept -> void {
//	std::cout << "[PANIC]" << std::endl;
//
//	for (; _it != _end; ++_it) {
//		auto& id = _it.token().id;
//		if (id == tk::separator || id == tk::identifier)
//			return;
//		id = tk::invalid;
//	}
//}
//
//
//auto pr::parser::classify_nest(void) noexcept -> nest_kind {
//
//	// assume current token is '(' or '['
//    auto it = _it;
//    auto start_tok = it.token().id;
//
//    mx::usz depth = 0;
//    bool seen_value = false;
//    bool seen_block = false;
//
//    // avancer pour passer '('
//    ++it;
//    depth = 1;
//
//    while (it != _end && depth > 0)
//    {
//        auto& tk = it.token();
//
//        switch (tk.id)
//        {
//            case tk::priority_open:
//            case tk::permutation_open:
//                ++depth;
//                break;
//
//            case tk::priority_close:
//            case tk::permutation_close:
//                --depth;
//                break;
//
//            // value-level tokens
//            case tk::note:
//            case tk::decimal:
//            case tk::binary:
//            case tk::hexadecimal:
//            case tk::tempo_fast:
//            case tk::tempo_slow:
//            case tk::parallel:
//			case tk::crossfade:
//                seen_value = true;
//                break;
//
//            // block-level tokens
//            case tk::parameter:
//            case tk::block_reference:       // &bass
//                seen_block = true;
//                break;
//
//            default:
//                // invalid token inside group
//                seen_block = seen_value = true; // force hybrid
//                break;
//        }
//
//        ++it;
//    }
//
//    if (seen_block && seen_value)
//        return nest_kind::error_hybrid;
//    if (seen_block)
//        return nest_kind::nest_block;
//
//    return nest_kind::nest_value;
//}


//#ifndef language_parser_hpp
//#define language_parser_hpp
//
//#include "language/tokens/tokens.hpp"
//#include "language/diagnostic.hpp"
//#include "language/parser/rules.hpp"
//
//
//// -- forward declarations ----------------------------------------------------
//
//namespace as { class tree;       }
//namespace an { class diagnostic; }
//
//
//// -- P R  N A M E S P A C E --------------------------------------------------
//
//namespace pr {
//
//
//	/* id */
//	using id = mx::usz;
//
//
//
//	enum : pr::id {
//
//		// scope tokens
//		assignments,
//		separator,
//
//        // references
//		block_references,
//		param_references,
//
//        // parameters
//        parameter,
//
//        // tempo
//        tempo_fast,
//		tempo_slow,
//
//        // value sequences (numbers)
//        values,
//
//        // operators
//		block_parallel,
//		value_parallel,
//		block_crossfade,
//		value_crossfade,
//
//		// grouping
//		block_group_in,
//		block_group_out,
//		value_group_in,
//		value_group_out,
//
//        // permutations
//		block_perm_in,
//		block_perm_out,
//		value_perm_in,
//		value_perm_out,
//
//        invalid
//    };
//
//	constexpr const char* names[] {
//		"assignments",
//		"separator",
//		"block_references",
//		"param_references",
//		"parameter",
//		"tempo_fast",
//		"tempo_slow",
//		"values",
//		"block_parallel",
//		"value_parallel",
//		"block_crossfade",
//		"value_crossfade",
//		"block_group_in",
//		"block_group_out",
//		"value_group_in",
//		"value_group_out",
//		"block_perm_in",
//		"block_perm_out",
//		"value_perm_in",
//		"value_perm_out",
//		"invalid"
//	};
//
//
//    struct token {
//		// token type
//		pr::id  id;
//		// structural depth
//		mx::usz depth;
//		// index start
//		mx::usz start;
//		// number of covered tokens
//		mx::usz count;
//    };
//
//
//
//	// -- P A R S E R ---------------------------------------------------------
//
//	class parser final {
//
//
//		private:
//
//			// -- private types -----------------------------------------------
//
//			/* self type */
//			using self = pr::parser;
//
//			/* level */
//			using level = auto (self::*)(void) -> void;
//
//			//enum class level : pr::id {
//			//	scope,
//			//	block,
//			//	param,
//			//	value
//			//};
//
//
//			// -- private members ---------------------------------------------
//
//			/* tokens */
//			tk::tokens* _tokens;
//
//			/* diagnostic */
//			an::diagnostic* _diag;
//
//			tk::iterator _it;
//			tk::iterator _end;
//
//
//			mx::usz _depth;
//
//			std::vector<pr::token> _out;   // résultat final
//
//
//
//
//
//		public:
//
//			// -- public lifecycle --------------------------------------------
//
//			/* default constructor */
//			parser(void);
//
//
//			// -- public methods ----------------------------------------------
//
//			/* parse */
//			auto parse(tk::tokens&, an::diagnostic&) -> void;
//
//
//
//			// -- private methods ---------------------------------------------
//
//			auto error(const char*) -> void;
//			auto error(const char*, const tk::iterator&) -> void;
//
//			void emit(pr::id id, mx::usz start, mx::usz count) {
//				_out.emplace_back(id, _depth, start, count);
//				std::cout << "-> emit token " << names[id] << '\n';
//			}
//
//
//			auto parse_scope(void) -> void;
//			auto parse_expression(void) -> void;
//			auto parse_block(void) -> void;
//			auto parse_param(void) -> void;
//			auto parse_value(void) -> void;
//
//
//			auto initial(void) -> void;
//			auto block(void) -> void;
//
//
//
//
//			auto panic_expression(void) noexcept -> void;
//
//			auto merge_references(void) noexcept -> void;
//			auto merge_values(void) noexcept -> void;
//
//			auto merge_assignments(void) -> void;
//
//
//			enum class nest_kind {
//				nest_block,
//				nest_value,
//				error_hybrid
//			};
//
//			auto classify_nest(void) noexcept -> nest_kind;
//
//
//			auto current(void) const noexcept -> tk::token& {
//				if (_it == _end)
//					return tk::eot;
//				return _it.token();
//			}
//
//			auto previous(void) const noexcept -> tk::token& {
//				return _it.token_at(-1);
//			}
//
//			auto consume(void) noexcept -> tk::token& {
//				if (_it == _end || _it.token().id == tk::end_of_tokens)
//					return tk::eot;
//
//				auto& tok = _it.token();
//				++_it;
//				return tok;
//			}
//
//			auto is_end(void) const noexcept -> bool {
//				return _it == _end || _it.token().id == tk::end_of_tokens;
//			}
//
//			auto abord(void) noexcept -> void {
//				_it = _end;
//			}
//
//	}; // class parser
//
//} // namespace pr
//
//#endif // language_parser_hpp
