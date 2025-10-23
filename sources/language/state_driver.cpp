#include "language/state_driver.hpp"
#include "language/action_table.hpp"
#include "language/parser_context.hpp"

#include <iostream>

#include "language/states.hpp"
#include "debug_chars.hpp"


// -- S T A T E  D R I V E R --------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
ml::state_driver::state_driver(void)
: _stack{} {

	_stack.reserve(16U);

	// push initial state
	_stack.push_back(&ml::expect_block::table);
}

// -- public methods ----------------------------------------------------------

auto ml::state_driver::parse(ml::parser_context& ctx) -> void {

	std::cout << "\x1b[32m--- parsing started ---\x1b[0m\n";
	_ctx = &ctx;

	//while (it != end) {
	for (; ctx.eof() == false; ctx.next_byte()) {

		//std::cout << ml::dbc{_ctx->byte()} << "\n";
		auto at = _stack.back();
		at->execute(*this, ctx.byte());
	}
	// trigger end of chunk
	auto at = _stack.back();
	at->end_of_chunk(*this);
}

/* call actions */
template <ml::state_driver::action_type... As>
auto ml::state_driver::call(void) -> void {
	((this->*As)(), ...);
}

/* switch state */
template <typename S>
auto ml::state_driver::switch_state(void) noexcept -> void {
	// assume stack is never empty
	_stack.back() = &S::table;
	std::cout << "switch to state: " << S::name
			  << " " << ml::dbc{_ctx->byte()} << "\n";
}

/* push state */
template <typename S>
auto ml::state_driver::push_state(void) -> void {
	_stack.push_back(&S::table);
	std::cout << "push state: " << S::name
			  << " " << ml::dbc{_ctx->byte()} << "\n";
}

/* pop state */
auto ml::state_driver::pop_state(void) -> void {
	// assume stack is never empty
	_stack.pop_back();
	std::cout << "pop state " << " " << ml::dbc{_ctx->byte()} << "\n";
}


/* newline */
auto ml::state_driver::newline(void) noexcept -> void {
	_ctx->newline();
	std::cout << "newline\n";
}

/* push error */
template <ml::literal E>
auto ml::state_driver::push_error(void) -> void {

	std::string err;
	err.append("error:");
	err.append(std::to_string(_ctx->line()));
	err.append(":");
	err.append(std::to_string(_ctx->column()));
	err.append(": ");
	err.append(E.data);

	//_errors.emplace_back(std::move(err));
	// nothing for now
	std::cout << err << " " << ml::dbc{_ctx->byte()} << "\n";
}
