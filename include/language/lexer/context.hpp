#ifndef language_lexer_context_hpp
#define language_lexer_context_hpp

#include "language/tokens.hpp"
#include "language/lexer/data.hpp"
#include <vector>


// -- forward declarations ----------------------------------------------------

namespace ml {
	class mapped_file;
}


// -- L X  N A M E S P A C E --------------------------------------------------

namespace lx {


	// -- C O N T E X T -------------------------------------------------------

	class context final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = lx::context;


			// -- private members ---------------------------------------------

			/* iterator */
			const ml::u8* _it;

			/* end */
			const ml::u8* _end;

			/* line */
			ml::uint _line;

			/* column */
			ml::uint _column;

			/* buffer */
			std::string _buffer;

			/* count */
			ml::usz _count;

			lx::data _data;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			context(void);


			/* initialize */
			auto initialize(const ml::mapped_file&) -> void;

			/* lex */
			auto lex(void) -> void;

			/* next byte */
			auto next_byte(void) noexcept -> void;

			/* newline */
			auto newline(void) noexcept -> void;

			/* count */
			auto count(void) noexcept -> void;

			/* uncount */
			auto uncount(void) noexcept -> void;

			/* start range */
			auto start_range(void) noexcept -> void;

			/* end range */
			auto end_range(void) noexcept -> void;


			/* reset count */
			auto reset_count(void) noexcept -> void;

			/* append to buffer */
			auto append_to_buffer(void) -> void;

			/* extract view */
			auto extract_view(void) const noexcept -> std::string_view;


			// -- public accessors --------------------------------------------

			/* it */
			auto it(void) const noexcept -> const ml::u8* {
				return _it;
			}

			/* end */
			auto end(void) const noexcept -> const ml::u8* {
				return _end;
			}

			/* byte */
			auto byte(void) const noexcept -> ml::u8;

			/* line */
			auto line(void) const noexcept -> ml::uint;

			/* column */
			auto column(void) const noexcept -> ml::uint;

			/* eof */
			auto eof(void) const noexcept -> bool;

			/* tokens */
			auto tokens(void) noexcept -> std::vector<tk::token>&;

	}; // class context


} // namespace lx

#endif // language_lexer_context_hpp
