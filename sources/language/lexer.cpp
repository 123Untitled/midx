#include "language/lexer.hpp"
#include "language/diagnostic.hpp"
#include "language/tokens/token_list.hpp"
#include "language/lexer/char_class.hpp"


// -- L E X E R ---------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
lx::lexer::lexer(void) noexcept
: _head{nullptr}, _limit{nullptr}, _mark{nullptr},
  _line{0U},      _base{0U},       _cursor{0U},
  _tokens{nullptr},
  _diag{nullptr} {
}


// -- public methods ----------------------------------------------------------

/* lex */
auto lx::lexer::lex(const ml::byte_range& br, tk::token_list& tokens, an::diagnostic& diag) -> void {

	_head   = br.begin;
	_limit  = br.end;
	_line   = 0U;
	_base   = 0U;
	_tokens = &tokens;
	_diag   = &diag;

	self::_lex();
}


// -- private methods ---------------------------------------------------------

/* push token */
//template <bool parse, tk::is_token_class T>
template <bool parse, tk::id id>
auto lx::lexer::push_token(void) -> void {

	const ml::usz _size = (_head - _mark);
	_cursor = _base + _size;

	tk::token to{id, lx::lexeme{_mark, _size},
				 _line, _base, _cursor
	};

	if constexpr (parse)
		_tokens->push_parse_token(to);
	else
		_tokens->push_token(to);
	_base = _cursor;
}

/* push byte token */
//template <tk::is_token_class T>
template <tk::id id>
auto lx::lexer::push_byte_token(void) -> void {
	_tokens->push_parse_token(
		tk::token{id, lx::lexeme{_head, 1U},
				  _line, _base, _base + 1U
		}
	);
}

/* push error */
template <ml::literal E>
auto lx::lexer::push_error(void) -> void {

	_diag->push_error(
		E.data,
		_line,
		_base,
		_base + 1U // will be changed after
	);
}

/* lex */
auto lx::lexer::_lex(void) -> void {


	while (_head < _limit) {

		start:
		ml::u8 quote{'\''};
		const ml::u8 c = *_head;


		// -- text ------------------------------------------------------------

		if (cc::is_lower(*_head)) {
			identifier:

			_mark = _head;

			do {
				++_head;
			} while (_head < _limit && cc::is_lower(*_head));

			self::push_token<true, tk::text>();
			continue;
		}


		// -- notes -----------------------------------------------------------

		if (cc::is_note(*_head)) {

			constexpr ml::u8 SHARP = 0U;
			constexpr ml::u8 FLAT  = 1U;
			constexpr bool alts[][2U] {
				{true,  true}, {false, true},
				{true, false}, {true,  true},
				{false, true}, {true, false},
				{true,  true}
			};

			_mark = _head;
			++_head;

			if (_head < _limit) {
				if (*_head == '#') {
					if (alts[c - 'A'][SHARP] == false) {
						_cursor = _base + 1U;
						// it's a comment, not an alteration
						self::push_token<true, tk::note>();
						goto comment;
					}
					++_head;
				}
				else if (*_head == 'b') {
					if (alts[c - 'A'][FLAT] == false) {
						_cursor = _base + 1U;
						// it's an identifier, not an alteration
						self::push_token<true, tk::note>();
						goto identifier;
					}

					++_head;
				}
			}

			while (_head < _limit && cc::is_digit(*_head))
				++_head;

			self::push_token<true, tk::note>();
			continue;
		}



		if (cc::is_digit(c)) {

			_mark = _head;
			++_head;

			if (c == '0' && (_head < _limit)) {

				switch (*_head) {
					case 'b':
						do {
							++_head;
						} while (_head < _limit && cc::is_binary(*_head));

						self::push_token<true, tk::binary>();
						continue;

					case 'x':
						do {
							++_head;
						} while (_head < _limit && cc::is_hex(*_head));

						self::push_token<true, tk::hexadecimal>();
						continue;

					case 'o':
						do {
							++_head;
						} while (_head < _limit && cc::is_octal(*_head));

						self::push_token<true, tk::octal>();
						continue;

					default:
						// it's just a zero
				}
			}

			while (_head < _limit && cc::is_digit(*_head))
				++_head;

			self::push_token<true, tk::decimal>();
			continue;
		}



		switch (c) {

			case ' ':
			case '\t':
				++_head;
				++_base;
				continue;

			case '\n':
				++_head;
				++_line;
				_base = 0U;
				continue;

			case '\r':
				_head += (++_head < _limit && *_head == '\n') ? 1U : 0U;
				++_line;
				_base = 0U;
				continue;


			// -- strings -----------------------------------------------------

			case '"':
				quote = '"';

			case '\'': {
				_mark = _head;
				++_head;

				while (_head < _limit
				   && *_head != quote
				   && *_head != '\n'
				   && *_head != '\r')
					++_head;

				if (_head < _limit && *_head == quote)
					++_head;
				else
					self::push_error<"unterminated string">();

				self::push_token<true, tk::string>();
				continue;
			}

			// -- comments ----------------------------------------------------

			case '#': {
				comment:

				_mark = _head;

				do {
					++_head;
				} while (_head < _limit
					 && *_head != '\n'
					 && *_head != '\r');

				self::push_token<false, tk::comment>();
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
		++_head;
		++_base;
	}

	// push end of tokens
	_tokens->push_parse_token(
		tk::token{
			tk::end_of_tokens,
			lx::lexeme{_head, 0U},
			_line,
			_base,
			_base
		}
	);
}
