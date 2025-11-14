#ifndef language_ast_param_hpp
#define language_ast_param_hpp

#include "language/tokens.hpp"
#include "language/syntax/parameter.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- P A R A M -----------------------------------------------------------

	class param final {


		// -- friends ---------------------------------------------------------

		/* block as friend */
		friend class block;


		private:

			// -- private members ---------------------------------------------

			/* parameter */
			tk::raw::token* _param;

			/* param id */
			pa::id _param_id;


		public:

			/* values start index */
			mx::usz vs;

			/* values count */
			mx::usz vc;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			param(void) noexcept
			: _param{nullptr},
			  _param_id{pa::id::invalid},
			   vs{0U}, vc{0U} {
			}

			/* specifier constructor */
			param(const sp::id spec_id, tk::raw::token& tk, const mx::usz vs) noexcept
			: _param{&tk}, _param_id{pa::id::invalid},
			  vs{vs}, vc{0U} {

				// search param id
				_param_id = pa::search_table[static_cast<mx::usz>(spec_id)]
					(tk.lexeme);

				// set token id
				//_param->id = (_param_id == pa::id::invalid)
				//		   ? tk::raw::invalid
				//		   : tk::parameter;
			}


			// -- public accessors --------------------------------------------

			/* token */
			auto token(void) const noexcept -> tk::raw::token& {
				return *_param;
			}

			/* param id */
			auto param_id(void) const noexcept -> pa::id {
				return _param_id;
			}

			/* param to spec id */
			auto param_to_spec(void) const noexcept -> sp::id {
				return pa::param_to_spec[static_cast<mx::usz>(_param_id)];
			}


			// -- public methods ----------------------------------------------

			auto debug(void) const -> void {
				std::cout << "    PARAM: ";
				if (_param == nullptr) {
					std::cout << "//////\n";
				} else {
					std::cout << *_param << "\n";
				}
			}

	}; // class param

} // namespace as

#endif // language_ast_param_hpp
