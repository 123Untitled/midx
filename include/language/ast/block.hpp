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
			tk::raw::token* _specifier;

			/* spec id */
			sp::id _spec_id;

			/* identifier */
			tk::raw::token* _identifier;


		public:

			// -- public members ----------------------------------------------

			/* params start index */
			mx::usz ps;

			/* params count */
			mx::usz pc;


			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			block(void) = delete;

			/* token constructor */
			block(tk::raw::token& tk, const mx::usz ps) noexcept
			: _specifier{&tk},
			  _spec_id{sp::to_id(tk.lexeme)},
			  _identifier{nullptr},
			  ps{ps}, pc{0U} {

				// invalidate token if spec id is invalid
				//_specifier->id = (_spec_id == sp::id::invalid)
				//			   ? tk::raw::invalid
				//			   : tk::specifier;
			}


			// -- public modifiers --------------------------------------------

			/* identifier */
			auto identifier(tk::raw::token* tk) noexcept -> void {
				_identifier = tk;
			}


			// -- public accessors --------------------------------------------

			/* specifier */
			auto specifier(void) const noexcept -> tk::raw::token& {
				return *_specifier;
			}

			/* const specifier */
			auto specifier(void) noexcept -> const tk::raw::token& {
				return *_specifier;
			}


			/* spec id */
			auto spec_id(void) const noexcept -> sp::id {
				return _spec_id;
			}


			/* identifier */
			auto identifier(void) noexcept -> tk::raw::token& {
				return *_identifier;
			}

			/* const identifier */
			auto identifier(void) const noexcept -> const tk::raw::token& {
				return *_identifier;
			}


			/* is anonymous */
			auto is_anonymous(void) const noexcept -> bool {
				return _identifier == nullptr;
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
