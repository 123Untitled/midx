#include "language/lexer/context.hpp"
#include "system/mapped_file.hpp"


// -- L E X E R  C O N T E X T ------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
lx::context::context(void)
: _it{nullptr}, _end{nullptr},
  _line{1U}, _column{1U},
  _buffer{}, _count{0U} {
}

// -- public methods ----------------------------------------------------------

/* initialize */
auto lx::context::initialize(const ml::mapped_file& mf) -> void {

	_it   = mf.begin();
	_end  = mf.end();

	_line   = 1U;
	_column = 1U;
}


/* next byte */
auto lx::context::next_byte(void) noexcept -> void {
	//const ml::u8 c = *_it;
	++_it;
	++_column;
	//return c;
}

/* newline */
auto lx::context::newline(void) noexcept -> void {
	++_line;
	_column = 0U;
}

/* count */
auto lx::context::count(void) noexcept -> void {
	++_count;
}

/* uncount */
auto lx::context::uncount(void) noexcept -> void {
	--_count;
}

/* start range */
auto lx::context::start_range(void) noexcept -> void {
	//_range.row_head = _line;
	//_range.col_head = _column;
}

/* end range */
auto lx::context::end_range(void) noexcept -> void {
	//_range.row_tail = _line;
	//_range.col_tail = _column;
}

/* reset count */
auto lx::context::reset_count(void) noexcept -> void {
	_count = 0U;
}


/* append to buffer */
auto lx::context::append_to_buffer(void) -> void {
	_buffer.append(reinterpret_cast<const char*>(_it - _count), _count);
	//_count = 0U;
}

/* extract view */
auto lx::context::extract_view(void) const noexcept -> std::string_view {
	return std::string_view{
		reinterpret_cast<const char*>(_it - _count),
		_count
	};
}



// -- public accessors --------------------------------------------------------

/* byte */
auto lx::context::byte(void) const noexcept -> ml::u8 {
	return *_it;
}

/* line */
auto lx::context::line(void) const noexcept -> ml::uint {
	return _line;
}

/* column */
auto lx::context::column(void) const noexcept -> ml::uint {
	return _column;
}

/* eof */
auto lx::context::eof(void) const noexcept -> bool {
	return _it >= _end;
}

/* tokens */
auto lx::context::tokens(void) noexcept -> std::vector<tk::token>& {
	return _data.tokens();
}
