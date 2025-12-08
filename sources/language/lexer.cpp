#include "language/lexer.hpp"
#include "language/diagnostic.hpp"
#include "language/tokens/tokens.hpp"
#include "language/lexer/char_class.hpp"
#include "core/string/string.hpp"


// -- L E X E R ---------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
lx::lexer::lexer(void) noexcept
: _it{nullptr},
  _end{nullptr},
  _line{0U},
  _column{0U},
  _tokens{nullptr},
  _diag{nullptr} {
}


// -- public methods ----------------------------------------------------------

/* lex */
auto lx::lexer::lex(const mx::string& data, tk::tokens& tokens, an::diagnostic& diag) -> void {

		_it = reinterpret_cast<const mx::u8*>(data.data());
	   _end = _it + data.size();
	  _line = 0U;
	_column = 0U;
	_tokens = &tokens;
	  _diag = &diag;

	self::_lex();
}


// -- private methods ---------------------------------------------------------

/* error */
auto lx::lexer::error(const char* msg) -> void {
	_diag->push(msg, _line, _column, _column + 1U);
	_tokens->push_token(
		tk::invalid,
		self::byte_chunk()
	);
}

/* error */
auto lx::lexer::error(const char* msg, const tk::chunk& ck) -> void {
	_diag->push(msg, ck.range.ln, ck.range.cs, ck.range.ce);
	_tokens->push_token(
		tk::invalid,
		ck
	);
}

/* byte chunk */
auto lx::lexer::byte_chunk(void) noexcept -> tk::chunk {
	const tk::chunk ck {
				lx::lexeme{_it, 1U},
				tk::range{_line, _column, _column + 1U}
	};
	++_column;
	++_it;
	return ck;
}

/* new chunk */
auto lx::lexer::new_chunk(const mx::u8* mark) noexcept -> tk::chunk {
	const mx::usz size = (_it - mark);
	const mx::usz col_end = _column + size;
	tk::chunk c{
		lx::lexeme{mark, size},
		tk::range{_line, _column, col_end}
	};
	_column = col_end;
	return c;
}

/* push byte token */
auto lx::lexer::push_byte_token(const tk::id id) -> void {
	_tokens->push_filtered_token(id, self::byte_chunk());
}


auto lx::lexer::skip_ignored(void) -> void {

    while (_it < _end) {

		switch (*_it) {

			// skip blanks
			case ' ':
			case '\t': {
				do {
					++_column;
				} while (++_it < _end && cc::is_blank(*_it));
				break;
			}

			// skip linefeeds
			case '\n': {
				++_line;
				while (++_it < _end && *_it == '\n')
					++_line;
				_column = 0U;
				break;
			}

			// skip carriage returns
			case '\r': {
				do {
					_it += (++_it < _end && *_it == '\n')
						   ? 1U : 0U;
					++_line;
				} while (_it < _end && *_it == '\r');
				_column = 0U;
				break;
			}

			// skip comments
			case '~': {
				auto mark = _it;
				do {
					++_it;

					// check end
					if (_it >= _end)
						break;

					// include ending '~' if present
					if (*_it == '~') {
						++_it; break; }

				} while (*_it != '\n' && *_it != '\r');

				// push comment token
				_tokens->push_token(tk::comment,
									new_chunk(mark));
				break;
			}

			// nothing ignored
			default:
				return;
		}

	} // while

}


/* lex */
auto lx::lexer::_lex(void) -> void {


	while (_it < _end) {

		// skip ignored characters
		self::skip_ignored();

		if (_it >= _end)
			break;


		// lookup character

		switch (*_it) {

			// identifier
			// abcdefghijklmnopqrstuvwxyz_
			case 'a': case 'b': case 'c': case 'd':
			case 'e': case 'f': case 'g': case 'h':
			case 'i': case 'j': case 'k': case 'l':
			case 'm': case 'n': case 'o': case 'p':
			case 'q': case 'r': case 's': case 't':
			case 'u': case 'v': case 'w': case 'x':
			case 'y': case 'z': case '_':
				lex_identifier();
				continue;

			// notes A-G
			case 'C': case 'D': case 'E':
			case 'F': case 'G': case 'A': case 'B':
				lex_note();
				continue;

			// numbers 0-9
			case '0': case '1': case '2': case '3':
			case '4': case '5': case '6': case '7':
			case '8': case '9':
				lex_number();
				continue;

			// floating .123
			case '.':
				lex_floating();
				continue;

			// () [] {}
			case '(':
				push_byte_token(tk::priority_open);
				continue;

			case ')':
				push_byte_token(tk::priority_close);
				continue;

			case '[':
				push_byte_token(tk::permutation_open);
				continue;

			case ']':
				push_byte_token(tk::permutation_close);
				continue;

			case '{':
				push_byte_token(tk::condition_open);
				continue;

			case '}':
				push_byte_token(tk::condition_close);
				continue;



			// ;
			case ';':
				push_byte_token(tk::separator);
				continue;


			// reference
			case '&':
				lex_reference();
				continue;

			// parameter
			case ':':
				lex_parameter();
				continue;


			// parallel
			case '|':
				push_byte_token(tk::parallel);
				continue;


			// crossfade
			case '>': {

				const auto ck = byte_chunk();

				// skip ignored
				self::skip_ignored();

				if (_it < _end) {

					switch (*_it) {
						case '<': {
							_tokens->push_filtered_token(
								tk::crossfade,
								ck,
								self::byte_chunk()
							);
						}
						case '=': {
							//_tokens->push_filtered_token(
							//	tk::greater_equal,
							//	ck,
							//	self::byte_chunk()
							//);
						}
					}
					continue;
				}

				// push greater token
				//_tokens->push_filtered_token(
				//	tk::greater,
				//	ck
				//);

				continue;
			}

			case '^':
				lex_tempo(tk::tempo_fast);
				continue;
			case '\\':
				lex_tempo(tk::tempo_slow);
				continue;

			case '%':
				lex_modulo();
				continue;

			default:
				self::error("unknown character");
		}
	}

	// push end of tokens
	//_tokens->push_filtered_token(tk::end_of_tokens);
}


/* lex identifier */
auto lx::lexer::lex_identifier(void) -> void {
	const auto mark = _it;

	do {
		++_it;
	} while (_it < _end && (cc::is_ident(*_it)));

	_tokens->push_filtered_token(tk::identifier,
						   self::new_chunk(mark));
}

/* lex note */
auto lx::lexer::lex_note(void) -> void {

	const auto mark = _it;

	// check alterations
	if (++_it < _end && (*_it == '#' || *_it == 'b'))
		++_it;

	while (_it < _end && cc::is_digit(*_it))
		++_it;

	_tokens->push_filtered_token(tk::note,
					 self::new_chunk(mark));
}

/* lex number */
auto lx::lexer::lex_number(void) -> void {

	const mx::u8 first = *_it;

	const auto mark = _it;
	++_it;

	if (first == '0' && (_it < _end)) {

		switch (*_it) {

			case 'b': {
				do {
					++_it;
				} while (_it < _end && cc::is_binary(*_it));

				_tokens->push_filtered_token(
					tk::binary,
					self::new_chunk(mark)
				);
				return;
			}

			case 'o': {
				do {
					++_it;
				} while (_it < _end && cc::is_octal(*_it));

				_tokens->push_filtered_token(
					tk::octal,
					self::new_chunk(mark)
				);
				return;
			}

			case 'x': {
				do {
					++_it;
				} while (_it < _end && cc::is_hex(*_it));

				_tokens->push_filtered_token(
					tk::hexadecimal,
					self::new_chunk(mark)
				);
				return;
			}


			default:
				// it's just a zero
		}
	}

	while (_it < _end && cc::is_digit(*_it))
		++_it;

	tk::id id = tk::decimal;

	if (_it < _end) {

		if (*_it == '.') {
			id = tk::floating;

			do {
				++_it;
			} while (_it < _end && cc::is_digit(*_it));
		}
		//else if (*_it == '/') {
		//	id = tk::fraction;
		//
		//	++_it; // skip '/'
		//
		//	if (_it >= _end || !cc::is_digit(*_it)) {
		//		error("expected fraction denominator", self::new_chunk(mark));
		//		return;
		//	}
		//
		//	do {
		//		++_it;
		//	} while (_it < _end && cc::is_digit(*_it));
		//}
	}


	// push decimal token
	_tokens->push_filtered_token(
		id, self::new_chunk(mark)
	);
}

/* lex floating */
auto lx::lexer::lex_floating(void) -> void {

	// assume we are on '.'
	auto ck = byte_chunk();

	// mark start
	const auto mark = _it;

	while (_it < _end && cc::is_digit(*_it))
		++_it;

	// only a dot
	if (mark == _it) {
		error("expected floating value", ck);
		return;
	}

	// extend chunk
	const auto gap = static_cast<mx::usz>(_it - mark);
	ck.lexeme.size += gap;
	ck.range.ce    += gap;
	_column        += gap;

	_tokens->push_filtered_token(
		tk::floating, ck
	);
}


/* lex tempo */
auto lx::lexer::lex_tempo(const tk::id id) -> void {

	auto ck = byte_chunk();
	const auto eoc = _it;

	// skip ignored
	skip_ignored();

	if (_it >= _end || !cc::is_digit(*_it)) {
		error("expected tempo value", ck);
		return;
	}

	const auto mark = _it;

	do {
		++_it;
	} while (_it < _end && cc::is_digit(*_it));

	if (_it < _end && *_it == '.') {
		do {
			++_it;
		} while (_it < _end && cc::is_digit(*_it));
	}


	// check if we have skipped something
	//if (mark != eoc) {
		_tokens->push_filtered_token(
			id, ck, new_chunk(mark) // always split the chunk to parse the number
		);
		//return;
	//}

	// we can extend the previous chunk
	//const auto gap = static_cast<mx::usz>(_it - mark);
	//ck.lexeme.size += gap;
	//ck.range.ce    += gap;
	//_column        += gap;
	//
	//_tokens->push_filtered_token(id, ck);
}


/* lex modulo */
auto lx::lexer::lex_modulo(void) -> void {

	auto ck = byte_chunk();
	const auto eoc = _it;

	// skip ignored
	skip_ignored();

	if (_it >= _end || !cc::is_digit(*_it)) {
		error("expected modulo value", ck);
		return;
	}

	const auto mark = _it;

	do {
		++_it;
	} while (_it < _end && cc::is_digit(*_it));

	if (_it < _end && *_it == '.') {
		do {
			++_it;
		} while (_it < _end && cc::is_digit(*_it));
	}

	// push modulo token
	_tokens->push_filtered_token(
		tk::modulo, ck, new_chunk(mark) // always split the chunk to parse the number
	);
}


/* lex parameter */
auto lx::lexer::lex_parameter(void) -> void {

		  auto ck  = byte_chunk();
	const auto eoc = _it;

	// skip ignored
	self::skip_ignored();

	if (_it >= _end) {
		self::error("expected parameter name", ck);
		return;
	}

	// check block start
	if (*_it == ':') {

		// check if we have skipped something
		if (eoc != _it) {
			_tokens->push_filtered_token(
				tk::track_separator, ck, self::byte_chunk()
			);
			return;
		}

		// we can extend the previous chunk
		ck.lexeme.size += 1U;
		ck.range.ce    += 1U;
		++_column;
		++_it;
		_tokens->push_filtered_token(tk::track_separator, ck);
		return;
	}


	// check parameter reference
	if (*_it == '&') {

		// here we do not extend the chunk if possible
		// too complex...

		auto rf = byte_chunk();

		// skip ignored
		self::skip_ignored();


		if (_it == _end || !(cc::is_lower(*_it) || *_it == '_')) {
			_tokens->push_token(tk::param_reference, ck);
			self::error("expected param-ref name", rf);
			return;
		}

		// parse param-ref name
		const auto mark = _it;
		do {
			++_it;
		} while (_it < _end && cc::is_ident(*_it));

		// push param-ref token
		_tokens->push_filtered_token(
			tk::param_reference, ck, rf, self::new_chunk(mark)
		);

		return;
	}

	// check parameter name
	if (cc::is_lower(*_it) == true) {

		// parse parameter name
		const auto mark = _it;
		do {
			++_it;
		} while (_it < _end && cc::is_lower(*_it));


		// check if we have a skipped whitespaces
		if (mark != eoc) {
			_tokens->push_filtered_token(
				tk::parameter, ck, self::new_chunk(mark)
			);
			return;
		}

		// we can extend the previous chunk
		const auto gap = static_cast<mx::usz>(_it - eoc);
		ck.lexeme.size += gap;
		ck.range.ce    += gap;
		_column        += gap;

		// push token
		_tokens->push_filtered_token(tk::parameter, ck);
		return;
	}

}


/* lex reference */
auto lx::lexer::lex_reference(void) -> void {

	const auto ck = byte_chunk();

	// skip ignored
	self::skip_ignored();

	if (_it >= _end || !(cc::is_lower(*_it) || *_it == '_')) {
		self::error("expected reference name", ck);
		return;
	}

	// parse reference name
	const auto mark = _it;
	do {
		++_it;
	} while (_it < _end
		&& (cc::is_ident(*_it)));

	auto nk = self::new_chunk(mark);

	// push reference token
	_tokens->push_filtered_token(
		tk::reference, ck, nk
	);
}
