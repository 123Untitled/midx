#include "language/parser.hpp"
#include "language/parser/context.hpp"

#include <iostream>

#include "language/syntax/syntax_layout.hpp"
#include "language/syntax/block_view.hpp"
#include "debug_chars.hpp"


// -- S T A T E  D R I V E R --------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
pr::parser::parser(void)
: _stack{} {

	_stack.reserve(16U);

	// push initial state
	//self::push_state<ml::expect_block>();
	//_stack.push_back(&ml::expect_block::table);
}

// -- public methods ----------------------------------------------------------

auto pr::parser::parse(pr::context& ctx) -> void {

	std::cout << "\x1b[32m--- parsing started ---\x1b[0m\n";
	_ctx = &ctx;

	//for (; ctx.eof() == false; ctx.next_byte()) {

		//std::cout << ml::dbc{_ctx->byte()} << "\n";
		//_stack.back()->execute(*this, ctx.byte());
	//}
	// trigger end of chunk
	//_stack.back()->end_of_chunk(*this);
}

/* end of file */
auto pr::parser::end_of_file(void) -> void {
	std::cout << "\x1b[32m--- end of file ---\x1b[0m\n";
	//_stack.back()->end_of_file(*this);
	_stack.clear();
	//self::push_state<ml::expect_block>();
}

/* call actions */
//template <pr::action... As>
//auto pr::parser::call(void) -> void {
//	((this->*As)(), ...);
//}

/* switch state */
template <typename S>
auto pr::parser::switch_state(void) noexcept -> void {
	// assume stack is never empty
	_stack.back() = &S::table;
	std::cout << "switch to state: " << S::name << "\n";
}

/* push state */
template <typename S>
auto pr::parser::push_state(void) -> void {
	_stack.push_back(&S::table);
	std::cout << "push state: " << S::name << "\n";
}

/* pop state */
auto pr::parser::pop_state(void) -> void {
	// assume stack is never empty
	_stack.pop_back();
	std::cout << "pop state " << "\n";
}


/* newline */
auto pr::parser::newline(void) noexcept -> void {
	//_ctx->newline();
	std::cout << "newline\n";
}

/* push error */
template <ml::literal E>
auto pr::parser::push_error(void) -> void {

	std::string err;
	err.append("error:");
	//err.append(std::to_string(_ctx->line()));
	err.append(":");
	//err.append(std::to_string(_ctx->column()));
	err.append(": ");
	err.append(E.data);

	//_errors.emplace_back(std::move(err));
	// nothing for now
	//std::cout << err << " " << ml::dbc{_ctx->byte()} << "\n";
}

/* count */
auto pr::parser::count(void) noexcept -> void {
	//_ctx->count();
}

/* reset count */
auto pr::parser::reset_count(void) noexcept -> void {
	//_ctx->reset_count();
}



// -- specifier actions -------------------------------------------------------

/* search specifier */
auto pr::parser::search_specifier(void) -> void {
	// extract view
	//const auto view = _ctx->extract_view();

	//auto bv = sx::layout.find_block(view.data());
	//_ctx->reset_count();

	//if (bv.block_not_found()) {
		//std::cout << "unknown specifier: \x1b[31m'" << view << "'\x1b[0m\n";
		//self::push_error<"unknown specifier">();
		//self::switch_state<ml::expect_block>();
		return;
	//}

	//switch_state<ml::expect_identifier>();
	//(*this.*(bv.action()))();

	//std::cout << "found specifier: \x1b[32m'" << view << "'\x1b[0m\n";
}

/* on track */
auto pr::parser::on_track(void) -> void {
	std::cout << "action: on_track\n";
}


// -- identifier actions ------------------------------------------------------

/* handle identifier */
auto pr::parser::handle_identifier(void) -> void {
	// extract view
	//const auto view = _ctx->extract_view();

	//std::cout << "handled identifier: \x1b[34m'" << view << "'\x1b[0m\n";
}


// -- parameter actions -------------------------------------------------------

/* search parameter */
auto pr::parser::search_parameter(void) -> void {
	// extract view
	//const auto view = _ctx->extract_view();

	//std::cout << "searched parameter: \x1b[34m'" << view << "'\x1b[0m\n";
}
