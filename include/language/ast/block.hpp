#ifndef language_ast_block_hpp
#define language_ast_block_hpp

#include "language/tokens.hpp"
#include "language/ast/param.hpp"
#include "language/syntax/specifier.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- B L O C K -----------------------------------------------------------

	class block final {


		private:

			// -- private members ---------------------------------------------

			/* specifier */
			tk::token* _specifier;

			/* spec id */
			sp::id _spec_id;

			/* identifier */
			tk::token* _identifier;


		public:

			// -- public members ----------------------------------------------

			/* params start index */
			ml::usz ps;

			/* params count */
			ml::usz pc;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			block(void) noexcept
			: _specifier{nullptr},
			  _spec_id{sp::id::invalid},
			  _identifier{nullptr},
			  ps{0U}, pc{0U} {
			}

			/* params start constructor */
			block(const ml::usz ps) noexcept
			: _specifier{nullptr},
			  _spec_id{sp::id::invalid},
			  _identifier{nullptr},
			  ps{ps}, pc{0U} {
			}

			/* nested constructor */
			block(const ml::usz ps, const as::param& p) noexcept
			: _specifier{p._param},
			  _spec_id{
				  // convert param id to spec id
				  p.param_to_spec()
			  },
			  _identifier{nullptr},
			  ps{ps}, pc{0U} {
			}


			// -- public modifiers --------------------------------------------

			/* specifier */
			auto specifier(tk::token* tk) noexcept -> void {

				_specifier = tk;
				_spec_id = sp::to_id(tk->lexeme);

				// invalidate token if spec id is invalid
				_specifier->id = (_spec_id == sp::id::invalid)
							   ? tk::invalid
							   : tk::specifier;
			}

			/* identifier */
			auto identifier(tk::token* tk) noexcept -> void {
				_identifier = tk;
			}


			// -- public accessors --------------------------------------------

			/* specifier */
			auto specifier(void) const noexcept -> tk::token& {
				return *_specifier;
			}

			/* const specifier */
			auto specifier(void) noexcept -> const tk::token& {
				return *_specifier;
			}


			/* spec id */
			auto spec_id(void) const noexcept -> sp::id {
				return _spec_id;
			}


			/* identifier */
			auto identifier(void) noexcept -> tk::token& {
				return *_identifier;
			}

			/* const identifier */
			auto identifier(void) const noexcept -> const tk::token& {
				return *_identifier;
			}


			/* is anonymous */
			auto is_anonymous(void) const noexcept -> bool {
				return (_identifier == nullptr);
			}

			/* is nested */
			auto is_nested(void) const noexcept -> bool {
				return _specifier->id == tk::parameter;
			}



			// -- public methods ----------------------------------------------

			auto debug(void) const -> void {

				std::cout << "BLOCK: ";
				if (_specifier == nullptr) {
					std::cout << "////// ";
				} else {
					std::cout << *_specifier << " ";
				}
				if (_identifier == nullptr) {
					std::cout << "__anonymous";
				} else {
					std::cout << *(_identifier);
				}
				std::cout << '\n';
			}
	};

} // namespace as

#endif // language_ast_block_hpp
