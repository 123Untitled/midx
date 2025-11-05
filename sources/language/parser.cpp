#include "language/parser.hpp"
#include "language/tokens/token_list.hpp"

#include <iostream>




// -- P A R S E R -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
pr::parser::parser(void)
: _buffer{nullptr}, _tokens{nullptr}, _current{nullptr},
  _ctx{}, _states{} {

	   _ctx.reserve(16U);
	_states.reserve(16U);
}


// -- public methods ----------------------------------------------------------

auto pr::parser::parse(const ml::byte_range& br, tk::token_list& tokens) -> void {

	_buffer  = br.begin;
	_tokens  = &tokens;
	_current = nullptr;


	   _ctx.clear();
	_states.clear();

	ast::clear();

	self::push_state<expect_block>();

	for (auto& tok : tokens) {
		_current = &tok;
		(this->*(_states.back()))();
	}

	//for (const auto& b : ast::blocks)
	//	b.debug();
	//for (const auto& p : ast::params)
	//	p.debug();
	//for (const auto& v : ast::values)
	//	v.debug();

	ast::params_sort();
	ast::values_sort();
	ast::debug();

	//for (ml::usz i = 0U; i < ast::params.size(); ++i) {
	//	ast::params[ast::params_order[i]].debug();
	//}

	//for (const auto& v : ast::values)
	//	v.debug();

}


/* switch state */
template <typename S>
auto pr::parser::switch_state(void) noexcept -> void {
	_states.back() = S::state;
}

/* push state */
template <typename S>
auto pr::parser::push_state(void) -> void {
	_states.push_back(S::state);
}

/* pop state */
auto pr::parser::pop_state(void) -> void {
	_states.pop_back();
}

/* pop level */
auto pr::parser::pop_level(void) -> void {
	_states.pop_back();
	   _ctx.pop_back();
}

/* push error */
template <ml::literal E>
auto pr::parser::push_error(void) -> void {

	std::string err;
	err.append("error:");
	err.append(std::to_string(_current->line));
	err.append(":");
	err.append(std::to_string(_current->col_head));
	err.append(": ");
	err.append(E.data);

	_errors.emplace_back(std::move(err));
}


/* mark invalid */
auto pr::parser::mark_invalid(void) noexcept -> void {
	_current->id = tk::invalid;
}

auto pr::parser::top_block(void) noexcept -> ast::block& {
	const ml::usz i = _ctx.back().bi;
	return ast::blocks[i];
}



/* push block */
auto pr::parser::push_block(void) -> void {
	const auto bi = ast::blocks.size();
	ast::blocks.emplace_back();
	_ctx.emplace_back(bi);
}

/* push param */
auto pr::parser::push_param(void) -> void {
	_current->id = tk::parameter;
	_ctx.back().pi = ast::params.size();
	ast::params.emplace_back(
			_current,
			_ctx.back().bi
	);
}

/* push value */
auto pr::parser::push_value(void) -> void {
	const auto pi = _ctx.back().pi;
	ast::values.emplace_back(
			_current,
			pi
	);
}

/* push nested block */
auto pr::parser::push_nested_block(void) -> void {
	_current->id = tk::nested_block;
	const auto bi = ast::blocks.size();
	ast::blocks.emplace_back();
	ast::values.emplace_back(
			bi,
			_ctx.back().pi
	);
	_ctx.emplace_back(bi);
}


// -- states ------------------------------------------------------------------


/* expect block */
auto pr::parser::state_expect_block(void) -> void {
	//std::cout << "expect_block\n";

	switch (_current->id) {

		case tk::bracket_open:
			self::push_block();
			self::push_state<expect_specifier>();
			break;

		case tk::end_of_tokens:
			break;

		default:
			self::push_error<"expected block">();
			_current->id = tk::invalid;
	}
}


/* expect specifier */
auto pr::parser::state_expect_specifier(void) -> void {
	//std::cout << "expect_specifier\n";

	// this is always top level block
	// this state is never called for nested blocks

	switch (_current->id) {

		case tk::bracket_close:
			ast::blocks.pop_back(); // so we don't keep empty blocks
			self::pop_level();
			break;

		case tk::text: {
			auto& b = ast::blocks.back(); // maybe use _block_stack.back() instead
			b.specifier(_current);
			self::switch_state<expect_identifier>();
			break;
		}

		default:
			self::push_error<"expected specifier">();
			self::switch_state<panic_remove_block>();
	}
}

/* expect identifier */
auto pr::parser::state_expect_identifier(void) -> void {
	//std::cout << "expect_identifier\n";

	switch (_current->id) {

		// start parameter
		case tk::dot: {

			// create anonymous identifier
			auto& b = self::top_block();
			b.anonymous(ast::blocks.size());

			_current->id = tk::param_dot;
			self::switch_state<expect_parameter>();
			break;
		}

		// identifier
		case tk::string: {

			// set identifier
			auto& b = self::top_block();
			b.identifier(_current);

			// switch to expect dot
			self::switch_state<expect_dot>();
			break;
		}

		// end of tokens
		case tk::end_of_tokens:
			// fallthrough to remove block
			self::push_error<"block not closed">();

		// exit block
		case tk::bracket_close:
			// we can remove, this block is empty and anonymous
			ast::blocks.pop_back();
			self::pop_level();
			break;

		// error
		default:
			self::push_error<"expected identifier or parameter">();
			_current->id = tk::invalid;
			self::switch_state<panic_remove_block>();
	}
}

/* expect identifier */
auto pr::parser::state_expect_identifier_nested(void) -> void {
	//std::cout << "expect_identifier_nested\n";

	switch (_current->id) {

		// start parameter
		case tk::dot: {

			// create anonymous identifier
			auto& b = self::top_block();
			b.anonymous(ast::blocks.size());

			_current->id = tk::param_dot;
			self::switch_state<expect_parameter>();
			break;
		}

		// identifier
		case tk::string: {

			// set identifier
			auto& b = self::top_block();
			b.identifier(_current);

			// switch to expect dot
			self::switch_state<expect_dot>();
			break;
		}

		// end of tokens
		case tk::end_of_tokens:
			// fallthrough to remove block
			self::push_error<"block not closed">();

		// exit block
		case tk::bracket_close: {
			ast::blocks.pop_back(); // maybe we can remove this block if empty
			self::pop_level();
			auto& b = self::top_block();
			//b.remove_last_value(); // HANDLE THIS !!!
			// REPLACED BY :
			ast::values.pop_back();
			break;
		}


		// error
		default:
			self::push_error<"expected identifier or parameter">();
			_current->id = tk::invalid;
			self::switch_state<panic_remove_block>();
	}
}

/* expect dot */
auto pr::parser::state_expect_dot(void) -> void {
	//std::cout << "expect_dot\n";

	switch (_current->id) {

		// start parameter
		case tk::dot:
			_current->id = tk::param_dot;
			self::switch_state<expect_parameter>();
			break;

		// exit block
		case tk::bracket_close:
			self::pop_level();
			break;

		default:
			self::push_error<"expected parameter">();
			self::switch_state<panic_parameter>();
			// maybe a remove parameter panic state?
	}
}

/* expect parameter */
auto pr::parser::state_expect_parameter(void) -> void {
	//std::cout << "expect_parameter\n";

	switch (_current->id) {

		case tk::text: {
			self::push_param();
			self::switch_state<expect_value>();
			break;
		}

		case tk::dot: {
			self::push_error<"expected parameter name">();
			// stay in the same state
			// mark previous dot token as invalid
			break;
		}

		case tk::bracket_close: {
			self::push_error<"expected parameter name">();
			self::pop_level();
			break;
		}

		default:
			self::push_error<"expected parameter">();
			self::switch_state<panic_parameter>();
			//_states.push_back(&pr::parser::panic_parameter);
	}
}

/* expect value */
auto pr::parser::state_expect_value(void) -> void {
	//std::cout << "expect_value\n";

	switch (_current->id) {

		// inplace block
		case tk::bracket_open: {
			self::push_nested_block();
			self::push_state<expect_identifier_nested>();
			break;
		}

		case tk::bracket_close: {
			self::pop_level();
			break;
		}

		case tk::dot:
			self::switch_state<expect_parameter>();
			break;

		default:
			self::push_value();
	}
}

/* panic block */
auto pr::parser::state_panic_block(void) -> void {
	//std::cout << "panic_block\n";

	switch (_current->id) {

		case tk::bracket_open: {
			self::push_state<panic_block>();
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

/* remove block */
auto pr::parser::state_panic_remove_block(void) -> void {

	switch (_current->id) {

		case tk::bracket_open: {
			self::push_state<panic_block>();
			break;
		}

		case tk::bracket_close: {
			ast::blocks.pop_back();
			self::pop_level();
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
			self::switch_state<expect_parameter>();
			break;

		case tk::bracket_open:
			self::push_state<panic_block>();
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
