#ifndef language_lexer_data_hpp
#define language_lexer_data_hpp

#include "language/tokens.hpp"
#include <vector>


// -- L X  N A M E S P A C E --------------------------------------------------

namespace lx {


	// -- forward declarations ------------------------------------------------

	class context;


	// -- D A T A -------------------------------------------------------------

	class data final {


		private:


			// -- private members ---------------------------------------------

			/* tokens */
			std::vector<tk::token> _tokens;

			/* parse indices */
			std::vector<ml::uint> _parse_indices;

			/* ranges */
			std::vector<tk::range> _ranges;

			/* content */
			std::vector<ml::u8> _content;

			/* errors */
			std::vector<std::string> _errors;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			data(void) noexcept = default;


			/* start range */
			auto start_range(const lx::context&) -> void;

			/* end range */
			auto end_range(const lx::context&) -> ml::uint;


			/* tokens */
			auto tokens(void) noexcept -> std::vector<tk::token>& {
				return _tokens;
			}


	}; // class data

} // namespace lx

#endif // language_lexer_data_hpp
