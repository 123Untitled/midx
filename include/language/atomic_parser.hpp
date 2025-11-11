#ifndef language_atomic_parser_hpp
#define language_atomic_parser_hpp

#include "language/syntax/specifier.hpp"
#include "language/ast/param_view.hpp"
#include "language/ast/value.hpp"
#include "language/diagnostic.hpp"
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
			tk::token* _current;

			/* sequence reference */
			ml::sequence* _seq;

			/* diagnostic reference */
			an::diagnostic* _diag;



			// -- private methods ---------------------------------------------

			template <sp::id ID, bool neg, typename B> requires (std::same_as<B, ml::bin>)
			auto convert(void) -> void {
				mx::i8 v = ml::convert_bin<ID, neg>(*_current, *_diag);
				_seq->push(v, *_current);
			}

			template <sp::id ID, bool neg, typename B> requires (std::same_as<B, ml::dec>)
			auto convert(void) -> void {
				mx::i8 v = ml::convert_dec<ID, neg>(*_current, *_diag);
				_seq->push(v, *_current);
			}

			template <sp::id ID, bool neg, typename B> requires (std::same_as<B, ml::oct>)
			auto convert(void) -> void {
				mx::i8 v = ml::convert_oct<ID, neg>(*_current, *_diag);
				_seq->push(v, *_current);
			}

			template <sp::id ID, bool neg, typename B> requires (std::same_as<B, ml::hex>)
			auto convert(void) -> void {
				mx::i8 v = ml::convert_hex<ID, neg>(*_current, *_diag);
				_seq->push(v, *_current);
			}


			template <sp::id ID>
			auto state_negative(void) -> void {

				// only valid for specifiers that allow negative values
				static_assert(ml::limits<ID>::neg == true,
						"state_negative instantiated for specifier that does not allow negative values");


				switch (_current->id) {

					case tk::plus:
						return;

					case tk::hyphen:
						_state = &self::state_default<ID>;
						return;

					case tk::decimal:
						convert<ID, true, ml::dec>();
						_state = &self::state_default<ID>;
						return;

					case tk::hexadecimal:
						convert<ID, true, ml::hex>();
						_state = &self::state_default<ID>;
						return;

					case tk::octal:
						convert<ID, true, ml::oct>();
						_state = &self::state_default<ID>;
						return;

					case tk::binary: {
						convert<ID, true, ml::bin>();
						_state = &self::state_default<ID>;
						return;
					}

					default: {
						_diag->push_error("invalid value", *_current);
						_current->id = tk::invalid;
						break;
					}
				}

			}

			template <sp::id ID>
			auto state_default(void) -> void {

				switch (_current->id) {

					case tk::plus:
						return;

					case tk::hyphen: {
						if constexpr (ml::limits<ID>::neg == false) {
							_diag->push_error("negative values not allowed", *_current);
							_current->id = tk::invalid;
							break;
						}
						else {
							_state = &self::state_negative<ID>;
							break;
						}
					}

					case tk::decimal:
						convert<ID, false, ml::dec>();
						return;

					case tk::hexadecimal:
						convert<ID, false, ml::hex>();
						return;

					case tk::octal:
						convert<ID, false, ml::oct>();
						return;

					case tk::binary:
						convert<ID, false, ml::bin>();
						return;

					case tk::note: {
						// valid token, do nothing
						return;
					}

					default: {
						_diag->push_error("invalid value", *_current);
						_current->id = tk::invalid;
					}
				}

			}



		public:


			template <sp::id ID>
			auto parse(const as::param_view& pv, ml::sequence& seq, an::diagnostic& diag) -> void {

				// check valid specifier ID
				static_assert(ID >= sp::id::trig && ID <= sp::id::prob,
						"atomic_parser instantiated with invalid specifier id");

				_seq   = &seq;
				_diag  = &diag;
				_state = &self::state_default<ID>;

				// loop over values
				for (const auto& v : pv.values()) {
					_current = &v.token();
					(this->*(_state))();
				}

			}

	}; // class atomic_parser


} // namespace sx

#endif // language_atomic_parser_hpp
