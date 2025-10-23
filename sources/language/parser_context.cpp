#include "language/parser_context.hpp"
#include "system/mapped_file.hpp"
#include "data/project.hpp"
#include "language/machines/global_machine.hpp"


// -- P A R S E R  C O N T E X T ----------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
ml::parser_context::parser_context(void)
: _it{nullptr}, _end{nullptr},
  _line{1U}, _column{1U},
  _buffer{}, _count{0U},
  _errors{},
  _data{} {
}

// -- public methods ----------------------------------------------------------

/* initialize */
auto ml::parser_context::initialize(const ml::mapped_file& mf,
									ml::project& project) -> void {

	_it   = mf.begin();
	_end  = mf.end();
	_data.set_project(project);

	_line   = 1U;
	_column = 1U;
}


/* next byte */
auto ml::parser_context::next_byte(void) noexcept -> void {
	//const ml::u8 c = *_it;
	++_it;
	++_column;
	//return c;
}

/* newline */
auto ml::parser_context::newline(void) noexcept -> void {
	++_line;
	_column = 0U;
}

/* count */
auto ml::parser_context::count(void) noexcept -> void {
	++_count;
}

/* reset count */
auto ml::parser_context::reset_count(void) noexcept -> void {
	_count = 0U;
}

/* append to buffer */
auto ml::parser_context::append_to_buffer(void) -> void {
	_buffer.append(reinterpret_cast<const char*>(_it - _count), _count);
	//_count = 0U;
}

/* extract view */
auto ml::parser_context::extract_view(void) const noexcept -> std::string_view {
	return std::string_view{
		reinterpret_cast<const char*>(_it - _count),
		_count
	};
}



// -- public accessors --------------------------------------------------------

/* byte */
auto ml::parser_context::byte(void) const noexcept -> ml::u8 {
	return *_it;
}

/* line */
auto ml::parser_context::line(void) const noexcept -> ml::usz {
	return _line;
}

/* column */
auto ml::parser_context::column(void) const noexcept -> ml::usz {
	return _column;
}

/* eof */
auto ml::parser_context::eof(void) const noexcept -> bool {
	return _it >= _end;
}
