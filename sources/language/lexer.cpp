#include "language/lexer.hpp"
#include "language/lexer/table.hpp"
#include "language/lexer/context.hpp"
#include "language/lexer/states.hpp"

#include <iostream>

#include "debug_chars.hpp"


// -- S T A T E  D R I V E R --------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
lx::lexer::lexer(void) noexcept
: _it{nullptr}, _end{nullptr},
  _line{1U}, _column{0U},
  _state{&lx::on_default::table},
  _tokens{},
  _errors{} {
}



// -- public methods ----------------------------------------------------------

auto lx::lexer::lex(lx::context& ctx) -> void {
	std::cout << "\x1b[32m--- parsing started ---\x1b[0m\n";

	_it  = ctx.it();
	_end = ctx.end();

	for (; _it < _end; ++_it) {
		++_column; // maybe use arithmetic to increment column only on certain chars
		_state->execute(*this, *_it);
	}
	_state->end_of_file(*this);


	for (const auto& t : _tokens) {
		std::cout << "token: " << (unsigned)t.type << " [" << t.line << "] -> ";
		for (const ml::u8* p = t.begin; p < t.end; ++p) {
			//std::cout << *p;
			std::cout << ml::dbc{*p};
		}
		std::cout << "\n";
	}

	for (const auto& e : _errors) {
		std::cout << e << '\n';
	}

}


/* call actions */
template <lx::action... As>
auto lx::lexer::call(void) -> void {
	((this->*As)(), ...);
}


/* switch state */
template <typename S>
auto lx::lexer::switch_state(void) noexcept -> void {
	//static_assert(lx::is_state<S>, "S must be a state");
	// assume stack is never empty
	_state = &S::table;
	std::cout << "switch to state: " << S::name << "\n";
}

/* push state */
//template <typename S>
//auto lx::lexer::push_state(void) -> void {
//	_stack.push_back(&S::table);
//	std::cout << "push state: " << S::name << "\n";
//}

/* pop state */
//auto lx::lexer::pop_state(void) -> void {
//	// assume stack is never empty
//	_stack.pop_back();
//	std::cout << "pop state " << "\n";
//}


/* checkpoint */
auto lx::lexer::checkpoint(void) noexcept -> void {
	_checkpoint = _it;
	//std::cout << "checkpoint at: " << ml::dbc{*_it} << "\n";
}

/* newline */
auto lx::lexer::newline(void) noexcept -> void {
	_line += 1U;
	_column = 0U;
	//std::cout << "newline\n";
}


/* push token */
template <tk::is_token_class T, ml::uint SHIFT>
auto lx::lexer::push_token(void) -> void {

	//std::cout << "push token: " << static_cast<unsigned>(T::id) << '\n';

	_tokens.emplace_back(
		tk::token{
			T::id,
			_line,
			_checkpoint,
			_it + SHIFT
		}
	);
}

template <tk::is_token_class T>
auto lx::lexer::push_token2(void) -> void {
	_tokens.emplace_back(
		tk::token{T::id, _line, _checkpoint, _it}
	);
}

template <tk::is_token_class T, typename... Tp>
auto lx::lexer::push_token(const Tp&... args) -> void {
	_tokens.emplace_back(
		tk::token{
			T::id,
			args...
		}
	);
}

template <tk::is_token_class T>
auto lx::lexer::push_token_byte(void) -> void {
	_tokens.emplace_back(
		T::id, _line, _it, _it + 1U
	);
}

template <tk::is_token_class T>
auto lx::lexer::push_byte_token(void) -> void {
	_tokens.emplace_back(
		T::id, _line, _it, _it + 1U
	);
}

/* push error */
template <ml::literal E>
auto lx::lexer::push_error(void) -> void {

	std::string err;
	err.append("\x1b[31merror\x1b[0m:");
	err.append(std::to_string(_line));
	err.append(":");
	err.append(std::to_string(_column));
	err.append(": ");
	err.append(E.data);

	_errors.emplace_back(std::move(err));
}








/* count */
auto lx::lexer::count(void) noexcept -> void {
}

/* reset count */
auto lx::lexer::reset_count(void) noexcept -> void {
}

/* end of file */
//auto lx::lexer::end_of_file(void) -> void {
//	std::cout << "\x1b[32m--- end of file ---\x1b[0m\n";
//	_stack.back()->end_of_file(*this);
//	_stack.clear();
//}









auto lx::lexer::_atoi(void) noexcept -> ml::i8 {

	bool neg = false;

	if (_it < _end) {
		if (*_it == '-') {
			++_it; neg = true;
		}
		else if (*_it == '+')
			++_it;
	}

	ml::i8 num = 0U;
	constexpr ml::i8 max       = +127;
	constexpr ml::i8 min       = -128;
	constexpr ml::i8 mul_limit = min / 10;


	// accumulate as negative to handle -128

	while (_it < _end && (*_it >= '0' && *_it <= '9')) {

		// check -10 multiplication overflow
		if (num < mul_limit) {
			this->push_error<"note octave overflow">();
			num = neg ? min : max;
			break;
		}
		num *= 10;
		const ml::i8 digit = *_it - '0';

		// check subtraction overflow
		if (num < (min + digit)) {
			this->push_error<"note octave overflow">();
			num = neg ? min : max;
			break;
		}

		num -= digit;
		++_it;
	}

	// finalize octave value
	if (neg == false) {
		if (num == min) {
			this->push_error<"note octave overflow">();
			num = max;
		}
		else {
			num = -num;
		}
	}
	return 0;
}

auto lx::lexer::lex2(lx::context& ctx) -> void {
	_it  = ctx.it();
	_end = ctx.end();

	_line   = 1U;
	_column = 0U;


	while (_it < _end) {

		const ml::u8 c = *_it;
		++_column;


		// -- identifier ------------------------------------------------------

		if (cc::match<cc::lower>(c)) {
		//if (c >= 'a' && c <= 'z') {

			_checkpoint = _it;

			do {
				++_it;
			} while (_it < _end && (*_it >= 'a' && *_it <= 'z'));

			self::push_token2<tk::identifier>();
			_column += static_cast<ml::uint>(_it - _checkpoint - 1U);
			continue;
		}



		// -- notes -----------------------------------------------------------

		if (c >= 'A' && c <= 'G') {

			std::cout << "note detected: " << ml::dbc{c} << "\n";
			struct alteration final {
				const bool sharp;
				const bool flat;
			};

			constexpr ml::u8 notes[] {
				9U, 11U, 0U, 2U, 4U, 5U, 7U
			};

			constexpr alteration alts[] {
				{true,  true}, {false, true}, {true, false},
				{true,  true}, {false, true}, {true, false},
				{true,  true}
			};

			const ml::u8* begin = _it;
			auto value = notes[(c - 'A')];
			++_it;


			// -- alteration --------------------------------------------------

			if (_it < _end) {

				if (*_it == '#' && alts[(c - 'A')].sharp == true) {
					++_it;
					value = (value + 1U) % 12U;
				}
				else if (*_it == 'b' && alts[(c - 'A')].flat == true) {
					++_it;
					value = (value + 11U) % 12U;
				}
			}

			// -- negative octave ---------------------------------------------




			//constexpr ml::u8 note_limit = max / 12U;
			//
			//if (num > note_limit) {
			//	this->push_error<"note value overflow">();
			//}

			//num *= 12U;

			//if (num > (max - value)) {
			//	this->push_error<"note value overflow">();
			//}

			//value += num;

			continue;
		}


		if (c >= '0' && c <= '9') {
			std::cout << "digit detected: " << ml::dbc{c} << "\n";
			ml::uint base  = 10U;
			ml::uint value = static_cast<ml::uint>(c - '0');
			++_it;

			if (c == '0' && _it < _end) {
				switch (*_it) {
					case 'b':
						base = 2U;
						++_it;
						break;

					case 'x':
						base = 16U;
						++_it;
						break;

					case 'o':
						base = 8U;
						++_it;
						break;
					default:
				}
			}


			continue;
		}

		ml::u8 quote{'\''};

		switch (c) {

			case ' ':
			case '\t':
				++_it;
				continue;

			case '\n':
				++_line;
				++_it;
				_column = 0U;
				continue;

			case '\r':
				_it += (++_it < _end && *_it == '\n') ? 1U : 0U;
				++_line;
				_column = 0U;
				continue;


			// -- strings -----------------------------------------------------

			case '"':
				quote = '"';

			case '\'': {
				const ml::u8* begin = _it;
				++_it;

				while (_it < _end
					&& *_it != quote
					&& *_it != '\n'
					&& *_it != '\r')
					++_it;

				if (_it < _end && *_it == quote) {
					++_it;
				}
				else {
					self::push_error<"unterminated string">();
				}
				self::push_token<tk::string>(_line, begin, _it);
				_column += static_cast<ml::uint>(_it - begin);
				continue;
			}

			// -- comments ----------------------------------------------------

			case '#': {

				const ml::u8* begin = _it;

				do {
					++_it;
				} while (_it < _end
					 && *_it != '\n'
					 && *_it != '\r');

				self::push_token<tk::comment>(_line, begin, _it);

				// no need to update column here
				// because newline will reset it
				continue;
			}

			case '[':
				self::push_byte_token<tk::bracket_open>();
				break;
			case ']':
				self::push_byte_token<tk::bracket_close>();
				break;
			case '@':
				self::push_byte_token<tk::at_sign>();
				break;
			case '/':
				self::push_byte_token<tk::slash>();
				break;
			case '.':
				self::push_byte_token<tk::dot>();
				break;

			default:
				self::push_error<"unknown character">();
		}
		++_it;

	}

	for (const auto& t : _tokens) {
		std::cout << "token: " << (unsigned)t.type << " [" << t.line << "] -> ";
		for (const ml::u8* p = t.begin; p < t.end; ++p) {
			//std::cout << *p;
			std::cout << ml::dbc{*p};
		}
		std::cout << "\n";
	}

	for (const auto& e : _errors) {
		std::cout << e << '\n';
	}
}
