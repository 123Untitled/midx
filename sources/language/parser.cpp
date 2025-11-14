#include "language/parser.hpp"
#include "language/tokens/token_list.hpp"
#include "language/diagnostic.hpp"
#include "language/ast/tree.hpp"

#include <iostream>


// -- P A R S E R -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
pr::parser::parser(void)
: _tokens{nullptr},
  _diag{nullptr},
  //_it{}, _checkpoint{},
  _state{} {
}


// -- public methods ----------------------------------------------------------

auto pr::parser::parse(tk::tokens& tokens,
					   an::diagnostic& diag,
					   as::tree& tree) -> void {

	//_tree = &tree;
	//_tokens = &tokens;
	//_diag   = &diag;
	//
	//
	//const auto& tks = tokens;
	//
	//auto mt = tks.map_tokens();
	//const auto end = mt.end();
	//_it = mt.begin();
	//
	//// push initial state
	//_state = &pr::parser::initial;
	//
	//
	//// loop over tokens
	//while (_it != end) {
	//
	//	// check raw token id
	//	switch (_it->id) {
	//
	//
	//		// identifier
	//		case tk::raw::text: {
	//
	//			_tokens->push_syn_token(
	//					tk::syn::identifier,
	//					_it
	//			);
	//			++_it;
	//			continue;
	//		}
	//
	//		// parameter
	//		case tk::raw::colon: {
	//			_checkpoint = _it;
	//			++_it;
	//			if (_it == end || _it->id != tk::raw::text) {
	//				_diag->push("expected parameter after '.'", *_checkpoint);
	//				break;
	//			}
	//			_tokens->push_syn_token(
	//				tk::syn::parameter,
	//				_checkpoint,
	//				2U
	//			);
	//			++_it;
	//			continue;
	//		}
	//
	//
	//		// caret
	//		case tk::raw::caret: {
	//
	//			_checkpoint = _it;
	//			++_it;
	//
	//			if (_it == end || _it->id != tk::raw::decimal) {
	//				_diag->push("expected tempo after '^'", *_checkpoint);
	//				break;
	//			}
	//
	//			++_it;
	//			_tokens->push_syn_token(
	//					tk::syn::tempo_fast,
	//					_checkpoint,
	//					2U
	//					);
	//			continue;
	//		}
	//
	//
	//		// slash
	//		case tk::raw::backslash: {
	//			_checkpoint = _it;
	//			++_it;
	//
	//			if (_it == end || _it->id != tk::raw::decimal) {
	//				_diag->push("expected tempo after '*'", *_checkpoint);
	//				break;
	//			}
	//
	//			++_it;
	//			if (_it == end || _it->id != tk::raw::dot) {
	//				_tokens->push_syn_token(
	//					tk::syn::tempo_slow,
	//					_checkpoint,
	//					2U
	//				);
	//				continue;
	//			}
	//
	//			++_it;
	//			if (_it == end || _it->id != tk::raw::decimal) {
	//				_diag->push("expected number after '.'", *_checkpoint);
	//				break;
	//			}
	//
	//			_tokens->push_syn_token(
	//				tk::syn::tempo_slow,
	//				_checkpoint,
	//				4U
	//			);
	//
	//			++_it;
	//			break;
	//		}
	//
	//		// reference
	//		case tk::raw::ampersand: {
	//			_checkpoint = _it;
	//			++_it;
	//			if (_it == end || _it->id != tk::raw::text) {
	//				_diag->push("expected identifier after '&'", *_checkpoint);
	//				break;
	//			}
	//			_tokens->push_syn_token(
	//				tk::syn::reference,
	//				_checkpoint,
	//				2U
	//			);
	//			++_it;
	//			continue;
	//		}
	//
	//
	//		// end of tokens
	//		case tk::raw::end_of_tokens:
	//			break;
	//
	//		// otherwise error
	//		default: {
	//			_diag->push("expected identifier", *_it);
	//			// implement panic =
	//			//switch_state<panic_block>();
	//			//_it->id = tk::raw::invalid;
	//
	//		}
	//	}
	//	++_it;
	//}
}


// -- private methods ---------------------------------------------------------

/* initial */
auto pr::parser::initial(void) -> void {

}


