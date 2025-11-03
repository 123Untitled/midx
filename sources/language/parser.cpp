#include "language/parser.hpp"
#include "language/tokens/token_list.hpp"

#include <iostream>




// -- S T A T E  D R I V E R --------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
pr::parser::parser(void)
: _buffer{nullptr}, _tokens{nullptr}, _current{nullptr},
  _stack{}, _context{} {
}


// -- public methods ----------------------------------------------------------

auto pr::parser::parse(const ml::byte_range& br, tk::token_list& tokens) -> void {

	_buffer  = br.begin;
	_tokens  = &tokens;
	_current = nullptr;
	_stack.clear();
	_stack.reserve(tokens.parsable_size());
	_stack.push_back(&pr::parser::expect_block);

	for (auto& tok : tokens) {
		_current = &tok;
		(this->*(_stack.back()))();
	}
}


/* switch state */
template <pr::parser::action_type A>
auto pr::parser::switch_state(void) noexcept -> void {
	// assume stack is never empty
	_stack.back() = A;
}

/* push state */
template <pr::parser::action_type A>
auto pr::parser::push_state(void) -> void {
	_stack.push_back(A);
}

/* pop state */
auto pr::parser::pop_state(void) -> void {
	// assume stack is never empty
	_stack.pop_back();
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


struct block;

struct value final {
	enum {
		nested_block,
		param_value
	} type;

	union {
		block* nested_block;
		tk::token* param_value;
	} val;
};

struct param final {
	tk::token* param_name;
	std::vector<value> _values;
};


struct block {

	tk::token* spec;

	union {
		const char* anonymous;
		tk::token* token;
	} identifier;

	std::vector<param> _params;
};



// -- states ------------------------------------------------------------------

/* expect block */
auto pr::parser::expect_block(void) -> void {

	switch (_current->id) {

		case tk::bracket_open:
			self::push_state<&pr::parser::expect_specifier>();
			break;

		case tk::end_of_tokens:
			// do nothing
			break;

		default:
			self::push_error<"expected block">();
			self::mark_invalid();
	}
}


/* expect specifier */
auto pr::parser::expect_specifier(void) -> void {

	switch (_current->id) {

		case tk::bracket_close:
			self::pop_state();
			break;

		case tk::text: {
			// extract view
			const auto& l = _current->lexeme;
			auto bv = sx::layout.find_block(l);

			if (bv.block_not_found()) {
				std::cout << "unknown specifier: \x1b[31m'" << l << "'\x1b[0m\n";
				_current->id = tk::invalid;
				self::push_error<"unknown specifier">();
				self::switch_state<&pr::parser::panic_block>();
				return;
			}

			_context.push_back(bv);
			//sx::action a = bv.action();

			std::cout << "found specifier: \x1b[32m'" << l << "'\x1b[0m\n";
			self::switch_state<&pr::parser::expect_identifier>();
			break;
		}

		default:
			self::push_error<"expected specifier">();
			self::switch_state<&pr::parser::panic_block>();
	}
}

/* expect identifier */
auto pr::parser::expect_identifier(void) -> void {

	switch (_current->id) {

		// start parameter
		case tk::dot:

			// create anonymous identifier

			_current->id = tk::param_dot;
			self::switch_state<&pr::parser::expect_parameter>();
			break;

		// identifier
		case tk::string: {
			// handle identifier
			self::switch_state<&pr::parser::expect_dot>();
			break;
		}

		// exit block
		case tk::bracket_close:
			self::pop_state();
			_context.pop_back();
			break;

		// end of tokens
		case tk::end_of_tokens:
			self::push_error<"block not closed">();
			break;

		// error
		default:
			self::push_error<"expected identifier or parameter">();
			self::mark_invalid();
	}
}

/* expect dot */
auto pr::parser::expect_dot(void) -> void {

	switch (_current->id) {

		// start parameter
		case tk::dot:
			_current->id = tk::param_dot;
			self::switch_state<&pr::parser::expect_parameter>();
			break;

		// exit block
		case tk::bracket_close:
			self::pop_state();
			_context.pop_back();
			break;

		default:
			self::push_error<"expected parameter">();
			self::switch_state<&pr::parser::panic_parameter>();
	}
}

/* expect parameter */
auto pr::parser::expect_parameter(void) -> void {

	switch (_current->id) {

		case tk::text: {
			const auto& bv = _context.back();
			const auto& l = _current->lexeme;

			auto pv = bv.find_param(l);

			if (pv.param_not_found()) {
				_current->id = tk::invalid;
				std::cout << "unknown parameter: \x1b[31m'" << l << "'\x1b[0m\n";
				self::push_error<"unknown parameter">();
				self::switch_state<&pr::parser::panic_parameter>();
				return;
			}

			std::cout << "found parameter: \x1b[32m'" << l << "'\x1b[0m\n";

			// switch for next parameter or end of block
			self::switch_state<&pr::parser::expect_dot>();
			_current->id = tk::parameter;
			const sx::action a = pv.action();

			_stack.push_back(_actions[a]);

			break;
		}

		default:
			self::push_error<"expected parameter">();
			self::switch_state<&pr::parser::panic_parameter>();
	}
}

/* panic block */
auto pr::parser::panic_block(void) -> void {

	switch (_current->id) {

		case tk::bracket_open: {
			self::push_state<&pr::parser::panic_block>();

			// push fake block (maybe not needed?)
			constexpr auto bv = sx::request_block<"note">();
			_context.push_back(bv);
			break;
		}

		case tk::bracket_close: {
			self::pop_state();
			_context.pop_back(); // pop fake block --^
			break;
		}

		default:
			self::mark_invalid();
	}
}


/* panic parameter */
auto pr::parser::panic_parameter(void) -> void {

	switch (_current->id) {

		case tk::dot:
			self::switch_state<&pr::parser::expect_parameter>();
			break;

		case tk::bracket_open:
			self::push_state<&pr::parser::panic_block>();
			break;

		case tk::bracket_close:
			self::pop_state();
			break;

		default:
			self::mark_invalid();
	}
}



// -- context states ----------------------------------------------------------

/* on block trig */
auto pr::parser::on_block_trig(void) -> void {
	// nothing for now
}

/* on trig sequence */
auto pr::parser::on_trig_sequence(void) -> void {
	// parse values
}

/* on trig repeat */
auto pr::parser::on_trig_repeat(void) -> void {
	// 
}

/* on trig linked */
auto pr::parser::on_trig_linked(void) -> void {
	// expect bool
}

/* on block note */
auto pr::parser::on_block_note(void) -> void {
	// nothing for now
}

/* on note sequence */
auto pr::parser::on_note_sequence(void) -> void {
	// parse values
}

/* on note repeat */
auto pr::parser::on_note_repeat(void) -> void {
	// 
}

/* on note linked */
auto pr::parser::on_note_linked(void) -> void {
	// expect bool
}

/* on block gate */
auto pr::parser::on_block_gate(void) -> void {
	// nothing for now
}

/* on gate sequence */
auto pr::parser::on_gate_sequence(void) -> void {
	// parse values
}

/* on gate repeat */
auto pr::parser::on_gate_repeat(void) -> void {
	// 
}

/* on gate linked */
auto pr::parser::on_gate_linked(void) -> void {
	// expect bool
}

/* on block track */
auto pr::parser::on_block_track(void) -> void {
	// nothing for now
}

/* on track trig */
auto pr::parser::on_track_trig(void) -> void {
	// nothing for now
}

/* on track note */
auto pr::parser::on_track_note(void) -> void {
	// nothing for now
}

/* on track gate */
auto pr::parser::on_track_gate(void) -> void {
	// nothing for now
}

/* on block pattern */
auto pr::parser::on_block_pattern(void) -> void {
	// nothing for now
}

/* on block song */
auto pr::parser::on_block_song(void) -> void {
	// nothing for now
}




template <ml::literal L>
auto pr::parser::on_nestable_param(void) -> void {

	switch (_current->id) {

		// inplace block
		case tk::bracket_open: {
			constexpr auto bv = sx::request_block<L>();
			_context.push_back(bv);
			_current->id = tk::nested_block;
			//self::push_state<&pr::parser::expect_dot>();
			self::push_state<&pr::parser::expect_identifier>();
			break;
		}

		case tk::bracket_close:
			self::pop_state();
			break;

		case tk::string:
			// handle reference
			break;

		case tk::dot:
			self::switch_state<&pr::parser::expect_parameter>();
			break;

		default:
			self::mark_invalid();
			self::push_error<"expected parameter value">();
	}
}



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
