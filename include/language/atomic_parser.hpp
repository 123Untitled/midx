#ifndef language_atomic_parser_hpp
#define language_atomic_parser_hpp

#include "language/syntax/specifier.hpp"
#include "language/ast/param_view.hpp"
#include "language/ast/value.hpp"
#include "language/diagnostic.hpp"
#include "data/node.hpp"
#include "atoi.hpp"



// -- S X  N A M E S P A C E --------------------------------------------------

namespace sx {


	// -- forward declarations ------------------------------------------------

	class builder;


	// -- A T O M I C  P A R S E R --------------------------------------------

	class atomic_parser final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sx::atomic_parser;

			/* action type */
			using action_type = auto (sx::atomic_parser::*)(void) -> void;


			// -- private members ---------------------------------------------

			/* state */
			action_type _state;

			/* current */
			tk::raw::token* _current;

			/* sequence reference */
			//mx::sequence* _seq;
			mx::atomic_type* _seq;

			std::vector<mx::node*> _stack;

			/* diagnostic reference */
			an::diagnostic* _diag;

			/* signature */
			mx::signature _sign;

			/* numerator */
			mx::u32 _num;

			/* denominator */
			mx::u32 _den;


			// -- private methods ---------------------------------------------

			template <sp::id ID, bool neg, typename B> requires (std::same_as<B, mx::bin>)
			auto convert(void) -> void {
				mx::i8 v = mx::convert_bin<ID, neg>(*_current, *_diag);

				//if constexpr (ID == sp::id::trig) {
				//	_seq->push_trig(v, *_current, _sign);
				//}
				//else
				//	_seq->push(v, *_current, _sign);
			}

			template <sp::id ID, bool neg, typename B> requires (std::same_as<B, mx::dec>)
			auto convert(void) -> void {
				mx::i8 v = mx::convert_dec<ID, neg>(*_current, *_diag);

				//if constexpr (ID == sp::id::trig) {
				//	_seq->push_trig(v, *_current, _sign);
				//}
				//else
				//	_seq->push(v, *_current, _sign);
			}

			template <sp::id ID, bool neg, typename B> requires (std::same_as<B, mx::oct>)
			auto convert(void) -> void {
				mx::i8 v = mx::convert_oct<ID, neg>(*_current, *_diag);

				//if constexpr (ID == sp::id::trig) {
				//	_seq->push_trig(v, *_current, _sign);
				//}
				//else
				//	_seq->push(v, *_current, _sign);
			}

			template <sp::id ID, bool neg, typename B> requires (std::same_as<B, mx::hex>)
			auto convert(void) -> void {
				mx::i8 v = mx::convert_hex<ID, neg>(*_current, *_diag);

				//if constexpr (ID == sp::id::trig) {
				//	_seq->push_trig(v, *_current, _sign);
				//}
				//else
				//	_seq->push(v, *_current, _sign);
			}


			template <sp::id ID>
			auto state_negative(void) -> void {

				// only valid for specifiers that allow negative values
				static_assert(mx::overflow<ID>::neg == true,
						"state_negative instantiated for specifier that does not allow negative values");


				switch (_current->id) {

					//case tk::plus:
					//	return;
					//
					//case tk::hyphen:
					//	_state = &self::state_default<ID>;
					//	return;
					//
					//case tk::decimal:
					//	convert<ID, true, mx::dec>();
					//	_state = &self::state_default<ID>;
					//	return;
					//
					//case tk::hexadecimal:
					//	convert<ID, true, mx::hex>();
					//	_state = &self::state_default<ID>;
					//	return;
					//
					//case tk::octal:
					//	convert<ID, true, mx::oct>();
					//	_state = &self::state_default<ID>;
					//	return;
					//
					//case tk::binary: {
					//	convert<ID, true, mx::bin>();
					//	_state = &self::state_default<ID>;
					//	return;
					//}

					//default: {
					//	_diag->push("invalid value", *_current);
					//	_current->id = tk::raw::invalid;
					//	break;
					//}
				}

			}

			template <sp::id ID>
			auto expect_numerator(void) -> void {
				std::cout << "EXPECT NUMERATOR\n";

				switch (_current->id) {

					//case tk::decimal: {
					//	mx::i8 v = mx::convert<mx::overflow<sp::id::prob>, mx::dec, false>(*_current, *_diag);
					//	_num = static_cast<mx::u32>(v);
					//	_state = &self::expect_slash<ID>;
					//	//_current->id = tk::signature;
					//	return;
					//}
					//
					//default: {
					//	_diag->push("invalid numerator value", *_current);
					//	_current->id = tk::raw::invalid;
					//	_state = &self::state_default<ID>;
					//	break;
					//}
				}
			}

			template <sp::id ID>
			auto expect_slash(void) -> void {

				std::cout << "EXPECT SLASH\n";

				switch (_current->id) {

					//case tk::slash:
					//	_state = &self::expect_denominator<ID>;
					//	//_current->id = tk::signature;
					//	return;
					//
					//default: {
					//	_diag->push("expected '/' after denominator", *_current);
					//	_current->id = tk::raw::invalid;
					//	_state = &self::state_default<ID>;
					//	break;
					//}
				}
			}

			template <sp::id ID>
			auto expect_denominator(void) -> void {

				std::cout << "EXPECT DENOMINATOR\n";

				switch (_current->id) {

					//case tk::decimal: {
					//	mx::i8 v = mx::convert<mx::overflow<sp::id::prob>, mx::dec, false>(*_current, *_diag);
					//	_den = static_cast<mx::u32>(v);
					//	_sign = mx::signature{_num, _den};
					//	_state = &self::state_default<ID>;
					//	//_current->id = tk::signature;
					//	return;
					//}
					//
					//default: {
					//	_diag->push("invalid denominator value", *_current);
					//	_current->id = tk::raw::invalid;
					//	_state = &self::state_default<ID>;
					//	break;
					//}
				}
			}

			template <sp::id ID>
			auto state_default(void) -> void {

				std::cout << "STATE DEFAULT\n";

				switch (_current->id) {

					//case tk::plus:
					//	return;
					//
					//case tk::hyphen: {
					//	if constexpr (mx::overflow<ID>::neg == false) {
					//		_diag->push("negative values not allowed", *_current);
					//		_current->id = tk::raw::invalid;
					//		break;
					//	}
					//	else {
					//		_state = &self::state_negative<ID>;
					//		break;
					//	}
					//}
					//
					//case tk::decimal:
					//	convert<ID, false, mx::dec>();
					//	return;
					//
					//case tk::hexadecimal:
					//	convert<ID, false, mx::hex>();
					//	return;
					//
					//case tk::octal:
					//	convert<ID, false, mx::oct>();
					//	return;
					//
					//case tk::binary:
					//	convert<ID, false, mx::bin>();
					//	return;
					//
					//case tk::note: {
					//	// valid token, do nothing
					//	return;
					//}
					//
					//case tk::at_sign: {
					//	_state = &self::expect_numerator<ID>;
					//	return;
					//}
					//
					//
					//default: {
					//	_diag->push("invalid value", *_current);
					//	_current->id = tk::raw::invalid;
					//}
				}
			}

			auto state_begin(void) -> void {

				switch (_current->id) {

					//case tk::plus:
					//case tk::hyphen:
					//case tk::decimal:
					//case tk::hexadecimal:
					//case tk::octal:
					//case tk::binary:
					//case tk::note: {
					//	// create node
					//	auto value = std::make_unique<mx::value>(0);
					//	auto& node = *value;
					//	break;
					//}
					//
					//default: {
					//	_diag->push("invalid value", *_current);
					//	_current->id = tk::raw::invalid;
					//}
				}
			}



		public:


			template <sp::id ID>
			auto parse(const as::param_view& pv, mx::atomic_type& seq, an::diagnostic& diag) -> void {
			//auto parse(const as::param_view& pv, mx::sequence& seq, an::diagnostic& diag) -> void {

				// check valid specifier ID
				static_assert(ID >= sp::id::trig && ID <= sp::id::prob,
						"atomic_parser instantiated with invalid specifier id");

				_sign  = mx::signature{};
				_seq   = &seq;
				_diag  = &diag;
				//_state = &self::state_default<ID>;
				_state = &self::state_begin;

				_stack.clear();
				_stack.push_back(_seq->root_node());

				// loop over values
				for (const auto& v : pv.values()) {
					_current = &v.token();
					(this->*(_state))();
				}

			}

	}; // class atomic_parser


} // namespace sx

#endif // language_atomic_parser_hpp
