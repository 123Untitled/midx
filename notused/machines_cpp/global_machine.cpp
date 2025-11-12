#include "language/machines/global_machine.hpp"
#include "language/parser_context.hpp"
#include <iostream>





// -- G L O B A L  M A C H I N E ----------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
mx::global_machine::global_machine(void) noexcept
: /*state_machine{},*/
  _ctx{nullptr},
  _tr{&_machine[S_EX_BLOCK][C_EOF]},
  _last{_tr->state} {
}


// -- public methods ----------------------------------------------------------

/* parse */
auto mx::global_machine::parse(mx::parser_context& ctx) -> void {

	_ctx = &ctx;

	for (; ctx.eof() == false; ctx.next_byte()) {

		_byte = ctx.byte();
		//std::cout << "byte: " << dbc{_byte} << "\n";

		// next transition
		self::_transition();
	}

	// end of file
	self::_end_of_file();
}

template <mx::global_machine::action_type... Ap>
auto mx::global_machine::_call(void) -> void {
	((this->*_actions[Ap])(), ...);
}


auto mx::global_machine::_push_error(void) -> void {

	std::string err;
	err.append("error:");
	err.append(std::to_string(_ctx->line()));
	err.append(":");
	err.append(std::to_string(_ctx->column()));
	err.append(": ");
	err.append(_tr->error);

	std::cout << err << "\n";
	//_errors.emplace_back(std::move(err));
}

auto mx::global_machine::_push_state(void) noexcept  -> void {
	_last = _tr->last;
}

auto mx::global_machine::_pop_state(void) noexcept -> void {
	_tr = &_machine[_last][C_EOF];
}

auto mx::global_machine::_transition(void) -> void {
	_tr = &_machine[_tr->state][_ctable[_byte]];
	(this->*_tr->action)();
}

auto mx::global_machine::_end_of_chunk(void) -> void {
	_tr = &_machine[_tr->state][C_EOC];
	(this->*_tr->action)();
}

auto mx::global_machine::_end_of_file(void) -> void {
	_tr = &_machine[_tr->state][C_EOF];
	(this->*_tr->action)();
}


auto mx::global_machine::_newline(void) noexcept -> void {
	_ctx->newline();
}

auto mx::global_machine::_increment(void) noexcept -> void {
	_ctx->count();
}

auto mx::global_machine::_uncount(void) noexcept -> void {
	_ctx->uncount();
}

auto mx::global_machine::_reset_count(void) noexcept -> void {
	_ctx->reset_count();
}


auto mx::global_machine::_search_block(void) -> void {

	//return;
	const auto view = _ctx->extract_view();

	const bool found = _keywords.on_block(view, *this);

	if (found == false) {
		_tr = &_machine[S_EX_BLOCK][C_EOF];
		// here custom push error
		std::cout << "unknown block: " << view << "\n";
		return;
	}
	else {
		std::cout << "block found: " << view << "\n";
		return;
	}
}

auto mx::global_machine::_search_label(void) -> void {

	//return;
	const auto view = _ctx->extract_view();

	const bool found = _keywords.on_label(view, *this);

	if (found == false) {
		_tr = &_machine[S_EX_BLOCK][C_EOF];
		// here custom push error
		std::cout << "unknown label: " << view << "\n";
		return;
	}
	else {
		std::cout << "label found: " << view << "\n";
		return;
	}
}

auto mx::global_machine::_handle_identifier(void) noexcept -> void {
	//return;
	const auto view = _ctx->extract_view();
	std::cout << "identifier: " << view << "\n";
}

auto mx::global_machine::_append_in_buffer(void) noexcept -> void {
	_ctx->append_to_buffer();
	std::cout << "appended to buffer\n";
	//_buffer.append(std::string{
	//	(const char*)_ctx->back_to(_count), _count});
}
