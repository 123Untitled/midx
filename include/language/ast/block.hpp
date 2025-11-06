#ifndef language_ast_block_hpp
#define language_ast_block_hpp

#include "language/tokens.hpp"
#include "language/ast/param.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- B L O C K -----------------------------------------------------------

	class block final {


		private:

			// -- private members ---------------------------------------------

			enum class spec_type : ml::usz {
				BLOCK,
				FROM_PARAM
			} _spec_type;

			/* specifier */
			tk::token* _specifier;

			/* id type */
			enum class id_type : ml::usz {
				ANONYMOUS,
				TOKEN
			} _id_type;

			/* identifier */
			union {
				ml::usz anonymous;
				tk::token* token;
			} _identifier;


		public:
			ml::usz _params_start;
			ml::usz _params_count;

		public:

			// -- public lifecycle --------------------------------------------

			block(void) noexcept
			: _spec_type{spec_type::BLOCK},
			  _specifier{nullptr},
			  _id_type{id_type::ANONYMOUS},
			  _identifier{0U},
			  _params_start{0U},
			  _params_count{0U} {
			}

			/* constructor */
			block(const ml::usz ps) noexcept
			: _spec_type{spec_type::BLOCK},
			  _specifier{nullptr},
			  _id_type{id_type::ANONYMOUS},
			  _identifier{0U},
			  _params_start{ps},
			  _params_count{0U} {
			}


			// -- public methods ----------------------------------------------

			auto specifier(tk::token* tk) noexcept -> void {
				_spec_type = spec_type::BLOCK;
				_specifier = tk;
			}

			auto specifier(const as::param& p) noexcept -> void {
				_spec_type = spec_type::FROM_PARAM;
				_specifier = p._token;
			}

			auto specifier(void) const noexcept -> tk::token& {
				return *_specifier;
			}

			auto anonymous(const ml::usz id) noexcept -> void {
				_id_type = id_type::ANONYMOUS;
				_identifier.anonymous = id;
			}

			auto identifier(tk::token* tk) noexcept -> void {
				_id_type = id_type::TOKEN;
				_identifier.token = tk;
			}


			auto debug(void) const -> void {

				std::cout << "BLOCK: ";
				if (_specifier == nullptr) {
					std::cout << "////// ";
				} else {
					std::cout << *_specifier << " ";
				}
				if (_id_type == id_type::ANONYMOUS) {
					std::cout << "__anon_" << _identifier.anonymous;
				} else {
					std::cout << *(_identifier.token);
				}
				std::cout << "\n";

				// debug output
			}
	};

} // namespace as

#endif // language_ast_block_hpp
