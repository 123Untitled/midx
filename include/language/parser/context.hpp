#ifndef ml_language_parser_context_hpp
#define ml_language_parser_context_hpp

#include "types.hpp"
#include "language/project_context.hpp"

#include <string>
#include <vector>


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- forward declarations ------------------------------------------------

	/* mapped file */
	class mapped_file;

	/* project */
	class project;


	// -- P A R S E R  C O N T E X T ------------------------------------------

	class parser_context final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::parser_context;


			// -- private members ---------------------------------------------

			/* iterator */
			const ml::u8* _it;

			/* end */
			const ml::u8* _end;

			/* line */
			ml::usz _line;

			/* column */
			ml::usz _column;

			/* buffer */
			std::string _buffer;

			/* count */
			ml::usz _count;


			std::vector<std::string> _errors;

			ml::project_context _data;


		public:

			parser_context(void);




			// -- public methods ----------------------------------------------

			/* initialize */
			auto initialize(const ml::mapped_file&, ml::project&) -> void;

			/* parse */
			auto parse(void) -> void;

			/* next byte */
			auto next_byte(void) noexcept -> void;

			/* newline */
			auto newline(void) noexcept -> void;

			/* count */
			auto count(void) noexcept -> void;

			/* uncount */
			auto uncount(void) noexcept -> void {
				--_count;
			}

			/* reset count */
			auto reset_count(void) noexcept -> void;

			/* append to buffer */
			auto append_to_buffer(void) -> void;

			/* extract view */
			auto extract_view(void) const noexcept -> std::string_view;


			// -- public accessors --------------------------------------------

			/* byte */
			auto byte(void) const noexcept -> ml::u8;

			/* line */
			auto line(void) const noexcept -> ml::usz;

			/* column */
			auto column(void) const noexcept -> ml::usz;

			/* eof */
			auto eof(void) const noexcept -> bool;


	}; // class parser_context

} // namespace ml

#endif // ml_language_parser_context_hpp
