#include "language/parser.hpp"
#include "language/tokens/token_list.hpp"
#include "language/diagnostic.hpp"
#include "language/ast/tree.hpp"

#include <iostream>


// -- P A R S E R -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
pr::parser::parser(void)
: _states{},
  _tokens{nullptr}, _diag{nullptr},
  _current{nullptr}, _prev{nullptr},
  _tree{},
  _counter{UINT64_MAX / 2U} {

	_states.reserve(16U);
}


// -- public methods ----------------------------------------------------------

auto pr::parser::parse(tk::token_list& tokens,
					   an::diagnostic& diag,
					   as::tree& tree) -> void {

	//_buffer  = br.begin;
	_tokens  = &tokens;
	_diag    = &diag;
	_tree    = &tree;
	_current = nullptr;
	_prev    = nullptr;
	_counter = UINT64_MAX / 2U;

	   tree.clear();
	_states.clear();

	self::push_state<expect_block>();

	for (auto& tok : tokens) {
		_current = &tok;
		(this->*(_states.back().st))();
		_prev = _current;
	}
}


/* switch state */
template <typename S>
auto pr::parser::switch_state(void) noexcept -> void {
	_states.back().st = S::state;
}

/* push state */
template <typename S>
auto pr::parser::push_state(void) -> void {
	_states.emplace_back(S::state, _current);
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

/* expect block */
auto pr::parser::state_expect_block(void) -> void {
	//std::cout << "expect_block\n";

	switch (_current->id) {

		case tk::bracket_open: {
			_tree->new_block();
			//_current->id = tk::specifier;
			push_state<expect_specifier>();
			break;
		}

		case tk::end_of_tokens:
			_states.pop_back();
			break;

		default:
			push_warning<"expected block">();
			_current->id = tk::invalid;
	}
}


/* expect specifier */
auto pr::parser::state_expect_specifier(void) -> void {
	//std::cout << "expect_specifier\n";

	// this is always top level block
	// this state is never called for nested blocks

	switch (_current->id) {

		// block closing
		case tk::bracket_close:
			// so we don't keep empty blocks
			_tree->remove_last_block();
			_states.pop_back();
			break;

		// specifier
		case tk::text: {
			auto& b = _tree->last_block();
			b.specifier(_current);
			switch_state<expect_identifier<false>>();
			_current->id = tk::specifier;
			break;
		}

		// end of tokens
		case tk::end_of_tokens:
			push_warning<"block not closed">(
					_states.back().tk
			);
			_states.pop_back();
			break;

		// otherwise error
		default: {
			push_error<"expected specifier">();
			switch_state<panic_remove_block>();
			if (_current->id == tk::bracket_open)
				push_state<panic_block>();
			_current->id = tk::invalid;
		}
	}
}

/* expect identifier */
template <bool N>
auto pr::parser::state_expect_identifier(void) -> void {
	//std::cout << "expect_identifier\n";

	switch (_current->id) {

		// start parameter
		case tk::dot: {
			// create anonymous identifier
			auto& b = _tree->last_block();
			b.anonymous(new_id());

			_current->id = tk::param_dot;
			switch_state<expect_parameter>();
			break;
		}

		// identifier
		case tk::string: {
			// set identifier
			auto& b = _tree->last_block();
			b.identifier(_current);

			// switch to expect dot
			switch_state<expect_dot>();
			break;
		}

		// end of tokens
		case tk::end_of_tokens:
			push_warning<"block not closed">(
					_states.back().tk
			);

		// exit block
		case tk::bracket_close:
			// we only keep this block if not nested
			// because we need to validate specifier
			if constexpr (N == false)
				_tree->flush();
			else
				_tree->remove_last_block();
			_states.pop_back();
			break;

		// error
		default: {
			push_error<"expected identifier or parameter">();
			switch_state<panic_remove_block>();
			if (_current->id == tk::bracket_open)
				push_state<panic_block>();
			_current->id = tk::invalid;
		}
	}
}


/* expect dot */
auto pr::parser::state_expect_dot(void) -> void {
	//std::cout << "expect_dot\n";

	switch (_current->id) {

		// start parameter
		case tk::dot:
			_current->id = tk::param_dot;
			switch_state<expect_parameter>();
			break;

		// end of tokens
		case tk::end_of_tokens:
			push_warning<"block not closed">(
					_states.back().tk
			);

		// exit block
		case tk::bracket_close:
			_tree->flush();
			_states.pop_back();
			break;

		default:
			push_error<"expected parameter">();
			switch_state<panic_parameter>();
			// maybe a remove parameter panic state?
	}
}

/* expect parameter */
auto pr::parser::state_expect_parameter(void) -> void {
	//std::cout << "expect_parameter\n";

	switch (_current->id) {

		case tk::text: {
			_tree->new_param(_current);
			_prev->id = tk::param_dot;
			_current->id = tk::parameter;
			switch_state<expect_value>();
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
			push_warning<"block not closed">(
					_states.back().tk
			);

		case tk::bracket_close: {
			push_error<"expected parameter">(_prev);
			_prev->id = tk::invalid;
			_tree->flush();
			_states.pop_back();
			break;
		}

		default:
			push_error<"expected parameter">(_prev);
			_current->id = _prev->id = tk::invalid;
			switch_state<panic_parameter>();
			//_states.push_back(&pr::parser::panic_parameter);
	}
}

/* expect value */
auto pr::parser::state_expect_value(void) -> void {
	//std::cout << "expect_value\n";

	switch (_current->id) {

		// inplace block
		case tk::bracket_open: {
			//_current->id = tk::nested_block;
			_tree->new_nested_block();
			push_state<expect_identifier<true>>();
			break;
		}

		case tk::bracket_close: {
			_tree->flush();
			_states.pop_back();
			break;
		}

		case tk::dot:
			switch_state<expect_parameter>();
			break;

		case tk::end_of_tokens: {

			push_warning<"block not closed">(_states.back().tk);
			_tree->flush();
			_states.pop_back();

			while (_states.empty() == false)
				_states.back().execute(*this);
			break;
		}

		default:
			_tree->new_value(_current);
	}
}

/* panic block */
auto pr::parser::state_panic_block(void) -> void {
	//std::cout << "panic_block\n";

	switch (_current->id) {

		case tk::bracket_open: {
			push_state<panic_block>();
			break;
		}

		case tk::bracket_close: {
			_states.pop_back();
			break;
		}

		case tk::end_of_tokens:
			_states.pop_back();
			break;

		default:
			_current->id = tk::invalid;
	}
}

/* remove block */
auto pr::parser::state_panic_remove_block(void) -> void {
	//std::cout << "panic_remove_block\n";

	switch (_current->id) {

		case tk::bracket_open: {
			push_state<panic_block>();
			break;
		}

		case tk::end_of_tokens:
		case tk::bracket_close: {
			_tree->remove_last_block();
			_states.pop_back();
			break;
		}

		default:
			_current->id = tk::invalid;
	}
}


/* panic parameter */
auto pr::parser::state_panic_parameter(void) -> void {
	//std::cout << "panic_parameter\n";

	switch (_current->id) {

		case tk::dot:
			switch_state<expect_parameter>();
			break;

		case tk::bracket_open:
			self::push_state<panic_block>();
			break;

		case tk::bracket_close:
			_states.pop_back();
			break;

		case tk::end_of_tokens:
			_states.pop_back();
			break;

		default:
			_current->id = tk::invalid;
	}
}
