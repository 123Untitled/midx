#ifndef language_ast_value_hpp
#define language_ast_value_hpp

#include "language/tokens.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- V A L U E -----------------------------------------------------------

	class value final {


		private:

			// -- private members ---------------------------------------------

			/* value type */
			enum class value_type : ml::usz {
				BLOCK,
				TOKEN
			} _type;

			/* value */
			union {
				ml::usz    block;
				tk::token* token;
			} _value;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			value(void) noexcept
			: _type{value_type::TOKEN}, _value{.token = nullptr} {
			}

			/* nested block constructor */
			explicit value(const ml::usz bi) noexcept
			: _type{value_type::BLOCK}, _value{.block = bi} {
			}

			/* token constructor */
			explicit value(tk::token* v) noexcept
			: _type{value_type::TOKEN}, _value{.token = v} {
			}


			// -- public accessors --------------------------------------------

			/* is nested */
			auto is_nested(void) const noexcept -> bool {
				return _type == value_type::BLOCK;
			}

			/* block index */
			auto block_index(void) const noexcept -> ml::usz {
				return _value.block;
			}

			/* token */
			auto token(void) const noexcept -> tk::token& {
				return *(_value.token);
			}


			// -- public methods ----------------------------------------------

			auto debug(void) const -> void {
				std::cout << "        VALUE: ";
				switch (_type) {
					case value_type::TOKEN:
						std::cout << *(_value.token) << '\n';
						break;
					case value_type::BLOCK:
						std::cout << "[nested block #" << _value.block << "]\n";
						break;
					default:
						break;
				}
			}

	}; // class value

} // namespace as

#endif // language_ast_value_hpp
