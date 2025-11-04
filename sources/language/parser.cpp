#include "language/parser.hpp"
#include "language/tokens/token_list.hpp"

#include <iostream>




// -- S T A T E  D R I V E R --------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
pr::parser::parser(void)
: _buffer{nullptr}, _tokens{nullptr}, _current{nullptr},
  _blocks{}, _stack{}, _states{} {
}


// -- public methods ----------------------------------------------------------

auto pr::parser::parse(const ml::byte_range& br, tk::token_list& tokens) -> void {

	_buffer  = br.begin;
	_tokens  = &tokens;
	_current = nullptr;

	_stack.clear();
	_states.clear();
	_blocks.clear();

	_stack.reserve(tokens.parsable_size());
	_states.reserve(tokens.parsable_size());

	_states.push_back(&pr::parser::expect_block);

	for (auto& tok : tokens) {
		_current = &tok;
		(this->*(_states.back()))();
	}


	for (const auto& b : _blocks) {
		b.debug();
	}
}


/* switch state */
template <pr::parser::action_type A>
auto pr::parser::switch_state(void) noexcept -> void {
	// assume stack is never empty
	_states.back() = A;
}

/* push state */
template <pr::parser::action_type A>
auto pr::parser::push_state(void) -> void {
	_states.push_back(A);
}

/* pop state */
auto pr::parser::pop_state(void) -> void {
	// assume stack is never empty
	_states.pop_back();
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


/* mark invalid */
auto pr::parser::mark_invalid(void) noexcept -> void {
	_current->id = tk::invalid;
}

auto pr::parser::top_block(void) noexcept -> ast::block& {
	const ml::usz i = _stack.back();
	return _blocks[i];
}




// -- states ------------------------------------------------------------------

/* expect block */
auto pr::parser::expect_block(void) -> void {
	//std::cout << "expect_block\n";

	switch (_current->id) {

		case tk::bracket_open:
			_blocks.emplace_back();
			_stack.push_back(_blocks.size() - 1U);
			_states.push_back(&pr::parser::expect_specifier);
			break;

		case tk::end_of_tokens:
			// do nothing
			break;

		default:
			self::push_error<"expected block">();
			_current->id = tk::invalid;
	}
}


/* expect specifier */
auto pr::parser::expect_specifier(void) -> void {
	//std::cout << "expect_specifier\n";

	// this is always top level block
	// this state is never called for nested blocks

	switch (_current->id) {

		case tk::bracket_close:
			_blocks.pop_back(); // so we don't keep empty blocks
			_stack.pop_back();
			_states.pop_back();
			break;

		case tk::text: {
			auto& b = _blocks.back(); // maybe use _block_stack.back() instead
			b.set_name(_current);
			_states.back() = &pr::parser::expect_identifier;
			break;
		}

		default:
			self::push_error<"expected specifier">();
			self::switch_state<&pr::parser::panic_block>();
	}
}

/* expect identifier */
auto pr::parser::expect_identifier(void) -> void {
	//std::cout << "expect_identifier\n";

	switch (_current->id) {

		// start parameter
		case tk::dot: {

			// create anonymous identifier
			auto& b = self::top_block();
			b.set_anonymous(_blocks.size());

			_current->id = tk::param_dot;
			_states.back() = &pr::parser::expect_parameter;
			break;
		}

		// identifier
		case tk::string: {

			// set identifier
			auto& b = self::top_block();
			b.set_id(_current);

			// switch to expect dot
			_states.back() = &pr::parser::expect_dot;
			break;
		}

		// exit block
		case tk::bracket_close:
			_blocks.pop_back(); // maybe we can remove this block if empty
			_states.pop_back();
			_stack.pop_back();
			break;

		// end of tokens
		case tk::end_of_tokens:
			// maybe remove block if empty
			self::push_error<"block not closed">();
			break;

		// error
		default:
			self::push_error<"expected identifier or parameter">();
			_current->id = tk::invalid;
			_states.back() = &pr::parser::panic_block;
	}
}

/* expect identifier */
auto pr::parser::expect_identifier_nested(void) -> void {
	//std::cout << "expect_identifier_nested\n";

	switch (_current->id) {

		// start parameter
		case tk::dot: {

			// create anonymous identifier
			auto& b = self::top_block();
			b.set_anonymous(_blocks.size());

			_current->id = tk::param_dot;
			_states.back() = &pr::parser::expect_parameter;
			break;
		}

		// identifier
		case tk::string: {

			// set identifier
			auto& b = self::top_block();
			b.set_id(_current);

			// switch to expect dot
			_states.back() = &pr::parser::expect_dot;
			break;
		}

		// exit block
		case tk::bracket_close: {
			_blocks.pop_back(); // maybe we can remove this block if empty
			_states.pop_back();
			_stack.pop_back();
			auto& b = self::top_block();
			b.remove_last_value();
			break;
		}

		// end of tokens
		case tk::end_of_tokens:
			// maybe remove block if empty
			self::push_error<"block not closed">();
			break;

		// error
		default:
			self::push_error<"expected identifier or parameter">();
			_current->id = tk::invalid;
			_states.back() = &pr::parser::panic_block;
	}
}

/* expect dot */
auto pr::parser::expect_dot(void) -> void {
	//std::cout << "expect_dot\n";

	switch (_current->id) {

		// start parameter
		case tk::dot:
			_current->id = tk::param_dot;
			_states.back() = &pr::parser::expect_parameter;
			break;

		// exit block
		case tk::bracket_close:
			_states.pop_back();
			_stack.pop_back();
			break;

		default:
			self::push_error<"expected parameter">();
			//_states.push_back(&pr::parser::panic_parameter);
			_states.back() = &pr::parser::panic_parameter;
			//self::switch_state<&pr::parser::panic_parameter>();
	}
}

/* expect parameter */
auto pr::parser::expect_parameter(void) -> void {
	//std::cout << "expect_parameter\n";

	switch (_current->id) {

		case tk::text: {

			auto& b = self::top_block();
			b.new_param(_current);

			_current->id = tk::parameter;
			_states.back() = &pr::parser::expect_value;
			break;
		}

		case tk::dot: {

			self::push_error<"expected parameter name">();
			// stay in the same state
			break;
		}

		case tk::bracket_close: {
			self::push_error<"expected parameter name">();
			_states.pop_back();
			_stack.pop_back();
			break;
		}

		default:
			self::push_error<"expected parameter">();
			//self::switch_state<&pr::parser::panic_parameter>();
			_states.back() = &pr::parser::panic_parameter;
			//_states.push_back(&pr::parser::panic_parameter);
	}
}

/* expect value */
auto pr::parser::expect_value(void) -> void {
	//std::cout << "expect_value\n";

	switch (_current->id) {

		// inplace block
		case tk::bracket_open: {

			auto& b = self::top_block();
			auto& p = b.last_param();

			const ml::usz idx = _blocks.size();
			_blocks.emplace_back();
			_stack.push_back(idx);

			p.new_value(idx);

			_current->id = tk::nested_block;
			_states.push_back(&pr::parser::expect_identifier_nested);
			break;
		}

		case tk::bracket_close: {
			_stack.pop_back();
			_states.pop_back();
			break;
		}

		//case tk::string: {
			//auto& b = self::top_block();
			//auto& p = b.last_param();
			//p.new_value(_current);

			//break;
		//}

		case tk::dot:
			_states.back() = &pr::parser::expect_parameter;
			break;

		default:
			auto& b = self::top_block();
			auto& p = b.last_param();
			p.new_value(_current);
			//_current->id = tk::invalid;
			//self::push_error<"expected parameter value">();
	}
}

/* panic block */
auto pr::parser::panic_block(void) -> void {
	//std::cout << "panic_block\n";

	switch (_current->id) {

		case tk::bracket_open: {
			_states.push_back(&pr::parser::panic_block);
			// maybe push fake block?
			break;
		}

		case tk::bracket_close: {
			_states.pop_back();
			// maybe pop fake block?
			break;
		}

		default:
			_current->id = tk::invalid;
	}
}


/* panic parameter */
auto pr::parser::panic_parameter(void) -> void {
	//std::cout << "panic_parameter\n";

	switch (_current->id) {

		case tk::dot:
			_states.back() = &pr::parser::expect_parameter;
			break;

		case tk::bracket_open:
			_states.push_back(&pr::parser::panic_block);
			break;

		case tk::bracket_close:
			_states.pop_back();
			break;

		default:
			_current->id = tk::invalid;
	}
}




/* expect specifier */
		//case tk::text: {
			// extract view
			//const auto& l = _current->lexeme;
			//auto bv = sx::layout.find_block(l);
			//
			//if (bv.block_not_found()) {
			//	std::cout << "unknown specifier: \x1b[31m'" << l << "'\x1b[0m\n";
			//	_current->id = tk::invalid;
			//	self::push_error<"unknown specifier">();
			//	self::switch_state<&pr::parser::panic_block>();
			//	return;
			//}
			//
			//_context.push_back(bv);
			////sx::action a = bv.action();
			//
			//std::cout << "found specifier: \x1b[32m'" << l << "'\x1b[0m\n";

/* expect parameter */
		//case tk::text: {
			//const auto& bv = _context.back();
			//const auto& l = _current->lexeme;
			//
			//auto pv = bv.find_param(l);
			//
			//if (pv.param_not_found()) {
			//	_current->id = tk::invalid;
			//	std::cout << "unknown parameter: \x1b[31m'" << l << "'\x1b[0m\n";
			//	self::push_error<"unknown parameter">();
			//	self::switch_state<&pr::parser::panic_parameter>();
			//	return;
			//}
			//
			//std::cout << "found parameter: \x1b[32m'" << l << "'\x1b[0m\n";
			//const sx::action a = pv.action();
			//_stack.push_back(_actions[a]);

//
//[note 'theme'
//	.seq C4 D4 E4 F4
//]
//
//[track 'synth'
//
//	.note 'theme' [ .seq C4 (E4 G4) B4 ]
//
//	.trig [
//		.seq 1 0 1
//	]
//]
//
//
//[note 'theme'
//	.seq C4 D4 E4 F4
//]
//[note '__anon98'
//	.seq G4 A4 B4 C5
//]
//
//
//[track 'synth'
//	.note 'theme' '_anon98'
//]
//
//[pattern 
// .section 'synth'
// .cond {
//	 if 'synth'.note == C4
//		 .note_on 100
//	}
//
//
//[song
// .timeline 4/4 120
// .start 'main'
// .loop 'main'
// 
// .track 'synth'
// 
//]
//
//
//
