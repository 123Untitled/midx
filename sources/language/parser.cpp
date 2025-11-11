#include "language/parser.hpp"
#include "language/tokens/token_list.hpp"
#include "language/diagnostic.hpp"
#include "language/ast/tree.hpp"

#include <iostream>


// -- P A R S E R -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
pr::parser::parser(void)
: _tree{nullptr},
  _diag{nullptr},
  _state{},
  _specifier{nullptr},
  _current{nullptr},
  _prev{nullptr} {
}


// -- public methods ----------------------------------------------------------

auto pr::parser::parse(tk::token_list& tokens,
					   an::diagnostic& diag,
					   as::tree& tree) -> void {

	_tree = &tree;
	_diag = &diag;

	self::switch_state<expect_block_open>();

	for (auto& tok : tokens) {
		_current = &tok;
		//(this->*(_states.back().st))();
		(this->*_state)();
		//_tree->dbg();
		_prev = _current;
	}

	_tree->sort();
}


/* switch state */
template <typename S>
auto pr::parser::switch_state(void) noexcept -> void {
	_state = S::state;
}

/* push error */
template <ml::literal E>
auto pr::parser::push_error(const tk::token* tk) -> void {

	tk = (tk == nullptr) ? _current : tk;

	_diag->push_error(
		E.data,
		tk->line,
		tk->col_head,
		tk->col_tail
	);
}

/* push warning */
template <ml::literal W>
auto pr::parser::push_warning(const tk::token* tk) -> void {

	tk = (tk == nullptr) ? _current : tk;

	_diag->push_warning(
		W.data,
		tk->line,
		tk->col_head,
		tk->col_tail
	);
}



// -- states ------------------------------------------------------------------

/* expect block open */
auto pr::parser::state_expect_block_open(void) -> void {

	switch (_current->id) {

		// block opening
		case tk::bracket_open: {
			switch_state<expect_specifier>();
			break;
		}

		// end of tokens
		case tk::end_of_tokens:
			break;

		// otherwise error
		default:
			push_warning<"expected block">();
			_current->id = tk::invalid;
	}
}


/* expect specifier */
auto pr::parser::state_expect_specifier(void) -> void {

	switch (_current->id) {

		// block closing
		case tk::bracket_close:
			switch_state<expect_block_open>();
			break;

		// specifier
		case tk::text: {
			_specifier = _current;
			switch_state<expect_block_close>();
			break;
		}

		// end of tokens
		case tk::end_of_tokens:
			push_warning<"block not closed">(_prev);
			break;

		// otherwise error
		default: {
			push_error<"expected specifier">();
			switch_state<panic_block>();
			_current->id = tk::invalid;
		}
	}
}

/* expect block close */
auto pr::parser::state_expect_block_close(void) -> void {

	switch (_current->id) {

		// block closing
		case tk::bracket_close:
			_tree->new_block(*_specifier);
			if (_specifier->id == tk::invalid)
				push_error<"invalid specifier">(_specifier);
			switch_state<expect_identifier>();
			break;

		// otherwise error
		default: {
			push_error<"block not closed">(_current);
			switch_state<panic_block>();
			_current->id = tk::invalid;
		}
	}
}

/* expect identifier */
auto pr::parser::state_expect_identifier(void) -> void {

	switch (_current->id) {

		// identifier
		case tk::text: {
			_tree->last_block().identifier(_current);
			_current->id = tk::string;
			switch_state<expect_dot>();
			break;
		}

		// dot
		case tk::dot:
			_current->id = tk::param_dot;
			switch_state<expect_parameter>();
			break;

		// restart block
		case tk::bracket_open: {
			switch_state<expect_specifier>();
			break;
		}

		// end of tokens
		case tk::end_of_tokens:
			break;

		// otherwise error
		default: {
			push_warning<"expected identifier">(_specifier);
			switch_state<panic_block>();
			_current->id = tk::invalid;
		}
	}
}


/* expect dot */
auto pr::parser::state_expect_dot(void) -> void {

	switch (_current->id) {

		// start parameter
		case tk::dot:
			_current->id = tk::param_dot;
			switch_state<expect_parameter>();
			break;

		// restart block
		case tk::bracket_open: {
			switch_state<expect_specifier>();
			break;
		}

		// end of tokens
		case tk::end_of_tokens:
			break;

		// otherwise error
		default:
			push_error<"expected parameter">();
			_current->id = tk::invalid;
	}
}

/* expect parameter */
auto pr::parser::state_expect_parameter(void) -> void {

	switch (_current->id) {

		case tk::text: {
			_tree->new_param(*_current);
			if (_current->id == tk::invalid)
				push_error<"invalid parameter">(_current);
			switch_state<expect_value>();
			break;
		}

		// restart block
		case tk::bracket_open: {
			push_error<"expected parameter">(_prev);
			_prev->id = tk::invalid;
			switch_state<expect_specifier>();
			break;
		}

		case tk::dot: {
			push_error<"expected parameter">(_prev);
			// stay in the same state
			// mark previous dot token as invalid
			_prev->id = tk::invalid;
			// HERE IMPLEMENT <bool FIRST_DOT>
			// TO NOT PUSH ERROR IF IT NOT THE FIRST DOT
			break;
		}

		case tk::end_of_tokens:
			push_warning<"expected parameter">(_prev);
			_prev->id = tk::invalid;
			break;

		default:
			push_error<"expected parameter">(_prev);
			_current->id = _prev->id = tk::invalid;
			switch_state<expect_dot>();
	}
}

/* expect value */
auto pr::parser::state_expect_value(void) -> void {

	switch (_current->id) {

		// restart block
		case tk::bracket_open: {
			switch_state<expect_specifier>();
			break;
		}

		case tk::dot:
			switch_state<expect_parameter>();
			_current->id = tk::param_dot;
			break;

		case tk::bracket_close: {
			push_error<"already in block">();
			break;
		}

		case tk::end_of_tokens: {
			break;
		}

		default:
			_tree->new_value(*_current);
	}
}

/* panic block */
auto pr::parser::state_panic_block(void) -> void {

	switch (_current->id) {

		// block opening
		case tk::bracket_open:
			switch_state<expect_specifier>();
			break;

		// block closing
		case tk::bracket_close:
			switch_state<expect_block_open>();
			break;

		// end of tokens
		case tk::end_of_tokens:
			break;

		// otherwise stay in panic
		default:
			_current->id = tk::invalid;
	}
}
