#ifndef language_analyzer_hpp
#define language_analyzer_hpp

#include "language/lexer/context.hpp"
#include "language/lexer.hpp"
#include "language/parser/context.hpp"
#include "language/parser.hpp"


//`@#()"'}{-+/[]|_=
//~!$%^&*\:?><;.,
//
//
//# -- project ------------------------------------------------------
//
//[project]
//= title "My Song"
//= bpm 128
//
//
//# -- components ---------------------------------------------------

// track components
// [no] note
// [ve] velocity
// [ga] gate
// [tr] trigger
// [oc] octave
// [se] semitone
// [pr] probability
// [ch] channel
// [si] signature


// [project]
// [track]
// [pattern]
// [play]


//[] a-z ' :
//
//
//[label] 'name'
//: key  `specialized parser` jusqu'a un [ ou : et 'name' redefini le name ou est ignore ?
//
//[note] 'theme'
//: seq @1/1 ((C4 E4) G4 A3) _ _ B4
//
//[prob] 'rand'
//: seq @1/8 100  50|75   25
//
//
//.[track] 'lead'
//	: note @1/16 (C2 _ _ (D3 E4)) >16< "theme"
//	: trigger @1/4   1 0 0 0 1 0 _ 1
//
//[track] 'bass'
//= note "theme"
//= trigger @1/8 10101010
//
//[pattern] 'intro'
//: duration 4 bars
//: tracks
//	{"bass"
//	 "lead"}
//
//[play]
//: repeat true
//: timeline "intro"*2



// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// --  A N A L Y Z E R ----------------------------------------------------

	class analyzer final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::analyzer;


			// -- private members ---------------------------------------------

			/* lexer context */
			lx::context _context;

			/* lexer */
			lx::lexer _lexer;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			analyzer(void);


			// -- public methods ----------------------------------------------

			/* analyze */
			auto analyze(const ml::mapped_file&, ml::project&) -> void;

	}; // class analyzer

} // namespace ml

#endif // language_analyzer_hpp






	//
	//
	//for (; _it < _end && _error.empty(); ++_it) {
	//
	//	// get next transition
	//	_tr = &ml::state_machine<self>[_tr->state][
	//		_ctable[static_cast<unsigned char>(*_it)]];
	//
	//	//if (_error.empty() == false) {
	//	//	break;
	//	//}
	//
	//	// execute action
	//	//_tr->execute(*this);
	//
	//}
	//
	//if (_error.empty()) {
	//
	//	// trigger end of buffer
	//	_tr = &ml::state_machine<self>[_tr->state][C_EOB];
	//
	//	// execute action
	//	//_tr->execute(*this);
	//
	//	return true;
	//}
	//
	//


	// reset parser state

	//	_tr = &ml::state_machine<self>[S_EX_BRACKET_OPEN][C_EOB];
	//  _last = _tr->state;
	//
	// _count = 0U; // maybe not needed
	//
	//   _num = 1U;
	//   _den = 16U;
	// _value = 0U;
	//
	//  _line = 1U;
	//_column = 1U;
	//
	// _buffer.clear();
	//  _error.clear();
	//
	//for (auto& key : _keywords)
	//	key.occured = false;

 // _tr{&ml::state_machine<self>[S_EX_BRACKET_OPEN][C_EOB]},
 // _last{_tr->state},
 // _keywords{
	//{"channel",  &self::_push_channel,  &self::_signature_channel},
	//{"trigger",  &self::_push_trigger,  &self::_signature_trigger},
	//{"note",     &self::_push_note,     &self::_signature_note   },
	//{"gate",     &self::_push_gate,     &self::_signature_gate   },
	//{"octave",   &self::_push_octave,   &self::_signature_octave },
	//{"velocity", &self::_push_velocity, &self::_signature_velocity}
 // },
 // _keyword{nullptr},
 // _count{0U}, _buffer{}, _num{1U}, _den{16U}, _value{0U},


//auto ml::parser::_skip(void) noexcept -> void {
//	++_column;
//}
//
///* newline */
//auto ml::parser::_newline(void) noexcept -> void {
//	++_line;
//	_column = 1U;
//}
//
//auto ml::parser::_comment(void) noexcept -> void {
//	_last = _tr->state;
//	_tr   = &ml::state_machine<self>[S_ON_COMMENT][C_EOB];
//	++_column;
//}
//
//auto ml::parser::_uncomment(void) noexcept -> void {
//	_tr = &ml::state_machine<self>[_last][C_EOB];
//	self::_newline();
//}
//
//auto ml::parser::_set_error(void) noexcept -> void {
//	_error = std::string{_tr->error}
//		   + " at line "
//		   + std::to_string(_line)
//		   + " column "
//		   + std::to_string(_column);
//	_it = _end;
//	std::cout << _error << std::endl;
//}
//
//auto ml::parser::_set_error(const char* msg) noexcept -> void {
//	_error = std::string{msg}
//		   + " at line "
//		   + std::to_string(_line)
//		   + " column "
//		   + std::to_string(_column);
//	_it = _end;
//	std::cout << _error << std::endl;
//}
//
//auto ml::parser::_increment(void) noexcept -> void {
//	++_count;
//	++_column;
//}
//
//auto ml::parser::_append(void) -> void {
//	_buffer.append(reinterpret_cast<const char*>(_it - _count), _count);
//	_count = 0U;
//	++_column;
//}
//
//auto ml::parser::_search_keyword(void) -> void {
//
//
//	for (auto& key : _keywords) {
//
//		if (_buffer != key.key)
//			continue;
//
//		if (key.occured) {
//			std::string er = _buffer + " already occured";
//			self::_set_error(er.data());
//			return;
//		}
//
//		// set occured
//		key.occured = true;
//
//		// set current keyword
//		_keyword = &key;
//
//		// set next transition
//		_tr = &ml::state_machine<self>[S_EX_VALUE][C_EOB];
//
//		// set signature
//		(this->*_keyword->signature)();
//
//		_buffer.clear();
//		return;
//	}
//
//	std::string er = _buffer + " is not a keyword";
//	// error
//	self::_set_error(er.data());
//}
//
//auto ml::parser::_append_and_keyword(void) -> void {
//	self::_append();
//	self::_search_keyword();
//}
//
//auto ml::parser::_push_trigger(void) -> void {
//	_context.track()._triggers.push(_value);
//}
//
//auto ml::parser::_push_channel(void) -> void {
//	_context.track()._channels.push(_value);
//}
//
//auto ml::parser::_push_note(void) -> void {
//	_context.track()._notes.push(_value);
//}
//
//auto ml::parser::_push_gate(void) -> void {
//	_context.track()._gates.push(_value);
//}
//
//auto ml::parser::_push_octave(void) -> void {
//	//_context.track()._octaves.push(_value);
//}
//
//auto ml::parser::_push_velocity(void) -> void {
//	_context.track()._velocities.push(_value);
//}
//
//auto ml::parser::_signature_trigger(void) -> void {
//	_context.track()._triggers.signature().assign(_num, _den);
//}
//
//auto ml::parser::_signature_channel(void) -> void {
//	_context.track()._channels.signature().assign(_num, _den);
//}
//
//auto ml::parser::_signature_note(void) -> void {
//	_context.track()._notes.signature().assign(_num, _den);
//}
//
//auto ml::parser::_signature_gate(void) -> void {
//	_context.track()._gates.signature().assign(_num, _den);
//}
//
//auto ml::parser::_signature_octave(void) -> void {
//	//_context.track()._octaves.signature().assign(_num, _den);
//}
//
//auto ml::parser::_signature_velocity(void) -> void {
//	_context.track()._velocities.signature().assign(_num, _den);
//}
//
//auto ml::parser::_init_signature(void) noexcept -> void {
//	_num = 0U;
//	_den = 0U;
//}
//
//
//template <unsigned char offset, unsigned char base, typename T>
//auto ml::parser::_new_digit(T& value) noexcept -> void {
//
//	// check multiplication overflow
//	//constexpr auto max = ml::limits<T>::max();
//	constexpr auto max = std::numeric_limits<T>::max();
//	constexpr auto mul_limit = max / base;
//
//	if (value > mul_limit) {
//		self::_set_error("overflow");
//		return;
//	}
//
//	value *= base;
//
//	// get digit
//	const unsigned char digit = (static_cast<unsigned char>(*_it) - offset);
//
//	if (digit > (max - value)) {
//		self::_set_error("overflow");
//		return;
//	}
//
//	value += digit;
//}
//
///* numerator */
//auto ml::parser::_numerator(void) -> void {
//	self::_new_digit<0x30U, 10U>(_num);
//}
//
///* denominator */
//auto ml::parser::_denominator(void) -> void {
//	self::_new_digit<0x30U, 10U>(_den);
//}
//
//
///* add value */
//auto ml::parser::_add_value(void) noexcept -> void {
//	// call push function
//	(this->*_keyword->push)();
//	_value = 0U;
//}
//
//
///* note */
//auto ml::parser::_note(void) noexcept -> void {
//
//	constexpr unsigned char def = 60U;
//
//	constexpr unsigned char notes[] {
//		// A
//		9U + def,
//		// B
//		11U + def,
//		// C
//		def,
//		// D
//		2U + def,
//		// E
//		4U + def,
//		// F
//		5U + def,
//		// G
//		7U + def
//	};
//
//	// assume *_it is [A-G]
//	_value = notes[(*_it - 0x41U)];
//}
//
//
//auto ml::parser::_sharp(void) noexcept -> void {
//	_value += 1U;
//}
//
//auto ml::parser::_flat(void) noexcept -> void {
//	_value = (_value + 11U) % 12U;
//}
//
//auto ml::parser::_octave(void) noexcept -> void {
//
//	unsigned char digit = *_it - 0x30U;
//	_value -= 60U;
//	_value += 24U + (digit * 12U);
//}
//
//
///* decimal */
//auto ml::parser::_decimal(void) -> void {
//	self::_new_digit<0x30U, 10U>(_value);
//}
//
///* lower hex */
//auto ml::parser::_lower_hex(void) noexcept -> void {
//	self::_new_digit<0x57U, 16U>(_value);
//}
//
///* upper hex */
//auto ml::parser::_upper_hex(void) noexcept -> void {
//	self::_new_digit<0x37U, 16U>(_value);
//}
//
///* digit hex */
//auto ml::parser::_digit_hex(void) noexcept -> void {
//	self::_new_digit<0x30U, 16U>(_value);
//}
//
///* binary */
//auto ml::parser::_bin(void) noexcept -> void {
//	self::_new_digit<0x30U, 2U>(_value);
//}
//
///* octal */
//auto ml::parser::_octal(void) noexcept -> void {
//	self::_new_digit<0x30U, 8U>(_value);
//}


			//auto _skip(void) noexcept -> void;
			//
			//auto _comment(void) noexcept -> void;
			//
			//auto _uncomment(void) noexcept -> void;
			//
			//auto _set_error(void) noexcept -> void;
			//
			//auto _set_error(const char*) noexcept -> void;
			//
			//auto _increment(void) noexcept -> void;
			//
			//auto _append(void) -> void;
			//
			//auto _search_keyword(void) -> void;
			//
			//auto _append_and_keyword(void) -> void;
			//
			//auto _numerator(void) -> void;
			//
			//auto _denominator(void) -> void;
			//
			//auto _newline(void) noexcept -> void;
			//
			//auto _add_value(void) noexcept -> void;
			//
			//auto _decimal(void) -> void;
			//
			//auto _note(void) noexcept -> void;
			//
			//auto _sharp(void) noexcept -> void;
			//
			//auto _flat(void) noexcept -> void;
			//
			//auto _octave(void) noexcept -> void;
			//
			//auto _lower_hex(void) noexcept -> void;
			//auto _upper_hex(void) noexcept -> void;
			//auto _digit_hex(void) noexcept -> void;
			//
			//auto _bin(void) noexcept -> void;
			//
			//auto _octal(void) noexcept -> void;
			//
			//auto _init_signature(void) noexcept -> void;
			//
			//
			//
			//template <unsigned char, unsigned char, typename T>
			//auto _new_digit(T&) noexcept -> void;
			//
			//
			//
			//auto _push_trigger(void) -> void;
			//auto _push_channel(void) -> void;
			//auto _push_note(void) -> void;
			//auto _push_gate(void) -> void;
			//auto _push_octave(void) -> void;
			//auto _push_velocity(void) -> void;
			//
			//auto _signature_trigger(void) -> void;
			//auto _signature_channel(void) -> void;
			//auto _signature_note(void) -> void;
			//auto _signature_gate(void) -> void;
			//auto _signature_octave(void) -> void;
			//auto _signature_velocity(void) -> void;



			/* keywords */
			//keyword_stuff _keywords[6U];

			/* current keyword */
			//keyword_stuff* _keyword;

			/* count */
			//ml::usz _count;

			/* buffer */
			//std::string _buffer;


			/* error */
			//std::string _error;


			/* num */
			//ml::usz _num;

			/* den */
			//ml::usz _den;

			/* value */
			//unsigned char _value;


	//template <typename T>
	//static ml::transition state_machine[S_MAX_TYPE][C_MAX_TYPE] {
	//
	//
	//	// -- S_EX_BRACKET_OPEN ---------------------------------------
	//	{
	//		// C_INVALID
	//		{"invalid character"},
	//
	//
	//		// C_NEWLINE
	//		{S_EX_BRACKET_OPEN,
	//		&T::_newline},
	//
	//		// C_WHITESPACE
	//		{S_EX_BRACKET_OPEN},
	//
	//
	//		// C_UPPER_HEX
	//		{"unexpected character"},
	//
	//		// C_UPPER_G
	//		{"unexpected character"},
	//
	//		// C_UPPER_OTHER
	//		{"unexpected character"},
	//
	//
	//		// C_LOWER_HEX
	//		{"unexpected character"},
	//
	//		// C_LOWER_X
	//		{"unexpected character"},
	//
	//		// C_LOWER_B
	//		{"unexpected character"},
	//
	//		// C_LOWER_O
	//		{"unexpected character"},
	//
	//		// C_LOWER_OTHER
	//		{"unexpected character"},
	//
	//
	//		// C_ZERO_DIGIT
	//		{"unexpected character"},
	//
	//		// C_ONE_DIGIT
	//		{"unexpected character"},
	//
	//		// C_OCTAL_DIGIT
	//		{"unexpected character"},
	//
	//		// C_DECIMAL_DIGIT
	//		{"unexpected character"},
	//
	//
	//		// C_BRACKET_OPEN
	//		{S_EX_KEYWORD},
	//
	//		// C_BRACKET_CLOSE
	//		{"unexpected character"},
	//
	//
	//		// C_COMMENT
	//		{S_EX_BRACKET_OPEN,
	//		&T::_comment},
	//
	//		// C_SEPARATOR
	//		{"unexpected character"},
	//
	//		// C_AT_SIGN
	//		{S_EX_NUMERATOR,
	//		&T::_init_signature},
	//
	//		// C_SLASH
	//		{"unexpected character"},
	//
	//
	//		// C_EOB
	//		{S_EX_BRACKET_OPEN},
	//	},
	//
	//
	//	// -- S_EX_KEYWORD --------------------------------------------
	//	{
	//		// C_INVALID
	//		{"invalid character"},
	//
	//
	//		// C_NEWLINE
	//		{S_EX_KEYWORD,
	//		&T::_newline},
	//
	//		// C_WHITESPACE
	//		{S_EX_KEYWORD},
	//
	//
	//		// C_UPPER_HEX
	//		{S_ON_KEYWORD,
	//		&T::_increment},
	//
	//		// C_UPPER_G
	//		{S_ON_KEYWORD,
	//		&T::_increment},
	//
	//		// C_UPPER_OTHER
	//		{S_ON_KEYWORD,
	//		&T::_increment},
	//
	//
	//		// C_LOWER_HEX
	//		{S_ON_KEYWORD,
	//		&T::_increment},
	//
	//		// C_LOWER_X
	//		{S_ON_KEYWORD,
	//		&T::_increment},
	//
	//		// C_LOWER_B
	//		{S_ON_KEYWORD,
	//		&T::_increment},
	//
	//		// C_LOWER_O
	//		{S_ON_KEYWORD,
	//		&T::_increment},
	//
	//		// C_LOWER_OTHER
	//		{S_ON_KEYWORD,
	//		&T::_increment},
	//
	//
	//		// C_ZERO_DIGIT
	//		{"unexpected character"},
	//
	//		// C_ONE_DIGIT
	//		{"unexpected character"},
	//
	//		// C_OCTAL_DIGIT
	//		{"unexpected character"},
	//
	//		// C_DECIMAL_DIGIT
	//		{"unexpected character"},
	//
	//
	//		// C_BRACKET_OPEN
	//		{"unexpected character"},
	//
	//		// C_BRACKET_CLOSE
	//		{"empty keyword"},
	//
	//
	//		// C_COMMENT
	//		{S_EX_KEYWORD,
	//		&T::_comment},
	//
	//		// C_SEPARATOR
	//		{"unexpected character"},
	//
	//		// C_AT_SIGN
	//		{"unexpected character"},
	//
	//		// C_SLASH
	//		{"unexpected character"},
	//
	//
	//		// C_EOB
	//		{S_EX_KEYWORD},
	//	},
	//
	//
	//	// -- S_ON_KEYWORD --------------------------------------------
	//	{
	//		// C_INVALID
	//		{"invalid character"},
	//
	//
	//		// C_NEWLINE
	//		{S_EX_BRACKET_CLOSE,
	//		&T::_append,
	//		&T::_newline},
	//
	//		// C_WHITESPACE
	//		{S_EX_BRACKET_CLOSE,
	//		&T::_append},
	//
	//
	//		// C_UPPER_HEX
	//		{S_ON_KEYWORD,
	//		&T::_increment},
	//
	//		// C_UPPER_G
	//		{S_ON_KEYWORD,
	//		&T::_increment},
	//
	//		// C_UPPER_OTHER
	//		{S_ON_KEYWORD,
	//		&T::_increment},
	//
	//
	//		// C_LOWER_HEX
	//		{S_ON_KEYWORD,
	//		&T::_increment},
	//
	//		// C_LOWER_X
	//		{S_ON_KEYWORD,
	//		&T::_increment},
	//
	//		// C_LOWER_B
	//		{S_ON_KEYWORD,
	//		&T::_increment},
	//
	//		// C_LOWER_O
	//		{S_ON_KEYWORD,
	//		&T::_increment},
	//
	//		// C_LOWER_OTHER
	//		{S_ON_KEYWORD,
	//		&T::_increment},
	//
	//
	//		// C_ZERO_DIGIT
	//		{"unexpected character"},
	//
	//		// C_ONE_DIGIT
	//		{"unexpected character"},
	//
	//		// C_OCTAL_DIGIT
	//		{"unexpected character"},
	//
	//		// C_DECIMAL_DIGIT
	//		{"unexpected character"},
	//
	//
	//		// C_BRACKET_OPEN
	//		{"unexpected character"},
	//
	//		// C_BRACKET_CLOSE
	//		{S_EX_VALUE,
	//		&T::_append_and_keyword},
	//
	//
	//		// C_COMMENT
	//		{S_EX_BRACKET_CLOSE,
	//		&T::_append,
	//		&T::_comment},
	//
	//		// C_SEPARATOR
	//		{"unexpected character"},
	//
	//		// C_AT_SIGN
	//		{"unexpected character"},
	//
	//		// C_SLASH
	//		{"unexpected character"},
	//
	//
	//		// C_EOB
	//		{S_ON_KEYWORD,
	//		&T::_append},
	//	},
	//
	//
	//
	//	// -- S_EX_BRACKET_CLOSE --------------------------------------
	//	{
	//		// C_INVALID
	//		{"invalid character"},
	//
	//
	//		// C_NEWLINE
	//		{S_EX_BRACKET_CLOSE,
	//		&T::_newline},
	//
	//		// C_WHITESPACE
	//		{S_EX_BRACKET_CLOSE},
	//
	//
	//		// C_UPPER_HEX
	//		{"unexpected character"},
	//
	//		// C_UPPER_G
	//		{"unexpected character"},
	//
	//		// C_UPPER_OTHER
	//		{"unexpected character"},
	//
	//		// C_LOWER_HEX
	//		{"unexpected character"},
	//
	//
	//		// C_LOWER_X
	//		{"unexpected character"},
	//
	//		// C_LOWER_B
	//		{"unexpected character"},
	//
	//		// C_LOWER_O
	//		{"unexpected character"},
	//
	//		// C_LOWER_OTHER
	//		{"unexpected character"},
	//
	//
	//		// C_ZERO_DIGIT
	//		{"unexpected character"},
	//
	//		// C_ONE_DIGIT
	//		{"unexpected character"},
	//
	//		// C_OCTAL_DIGIT
	//		{"unexpected character"},
	//
	//		// C_DECIMAL_DIGIT
	//		{"unexpected character"},
	//
	//
	//		// C_BRACKET_OPEN
	//		{"unexpected character"},
	//
	//		// C_BRACKET_CLOSE
	//		{S_EX_VALUE,
	//		&T::_search_keyword},
	//
	//
	//		// C_COMMENT
	//		{S_EX_BRACKET_CLOSE,
	//		&T::_comment},
	//
	//		// C_SEPARATOR
	//		{"unexpected character"},
	//
	//		// C_AT_SIGN
	//		{"unexpected character"},
	//
	//		// C_SLASH
	//		{"unexpected character"},
	//
	//		// C_EOB
	//		{S_EX_BRACKET_CLOSE},
	//	},
	//
	//
	//	// -- S_ON_COMMENT --------------------------------------------
	//	{
	//		// C_INVALID
	//		{S_ON_COMMENT},
	//
	//
	//		// C_NEWLINE
	//		{S_NONE, // state not used
	//		&T::_uncomment},
	//
	//		// C_WHITESPACE
	//		{S_ON_COMMENT},
	//
	//
	//		// C_UPPER_HEX
	//		{S_ON_COMMENT},
	//
	//		// C_UPPER_G
	//		{S_ON_COMMENT},
	//
	//		// C_UPPER_OTHER
	//		{S_ON_COMMENT},
	//
	//
	//		// C_LOWER_HEX
	//		{S_ON_COMMENT},
	//
	//		// C_LOWER_X
	//		{S_ON_COMMENT},
	//
	//		// C_LOWER_B
	//		{S_ON_COMMENT},
	//
	//		// C_LOWER_O
	//		{S_ON_COMMENT},
	//
	//		// C_LOWER_OTHER
	//		{S_ON_COMMENT},
	//
	//
	//		// C_ZERO_DIGIT
	//		{S_ON_COMMENT},
	//
	//		// C_ONE_DIGIT
	//		{S_ON_COMMENT},
	//
	//		// C_OCTAL_DIGIT
	//		{S_ON_COMMENT},
	//
	//		// C_DECIMAL_DIGIT
	//		{S_ON_COMMENT},
	//
	//
	//		// C_BRACKET_OPEN
	//		{S_ON_COMMENT},
	//
	//		// C_BRACKET_CLOSE
	//		{S_ON_COMMENT},
	//
	//
	//		// C_COMMENT
	//		{S_ON_COMMENT},
	//
	//		// C_SEPARATOR
	//		{S_ON_COMMENT},
	//
	//		// C_AT_SIGN
	//		{S_ON_COMMENT},
	//
	//		// C_SLASH
	//		{S_ON_COMMENT},
	//
	//
	//		// C_EOB
	//		{S_ON_COMMENT},
	//	},
	//
	//
	//	// -- S_EX_NUMERATOR ----------------------------------------
	//	{
	//		// C_INVALID
	//		{"signature ill-formed"},
	//
	//
	//		// C_NEWLINE
	//		{"signature ill-formed"},
	//
	//		// C_WHITESPACE
	//		{"signature ill-formed"},
	//
	//
	//		// C_UPPER_HEX
	//		{"signature ill-formed"},
	//
	//		// C_UPPER_G
	//		{"signature ill-formed"},
	//
	//		// C_UPPER_OTHER
	//		{"signature ill-formed"},
	//
	//
	//		// C_LOWER_HEX
	//		{"signature ill-formed"},
	//
	//		// C_LOWER_X
	//		{"signature ill-formed"},
	//
	//		// C_LOWER_B
	//		{"signature ill-formed"},
	//
	//		// C_LOWER_O
	//		{"signature ill-formed"},
	//
	//		// C_LOWER_OTHER
	//		{"signature ill-formed"},
	//
	//
	//		// C_ZERO_DIGIT
	//		{S_ON_NUMERATOR,
	//		&T::_numerator},
	//
	//		// C_ONE_DIGIT
	//		{S_ON_NUMERATOR,
	//		&T::_numerator},
	//
	//		// C_OCTAL_DIGIT
	//		{S_ON_NUMERATOR,
	//		&T::_numerator},
	//
	//		// C_DECIMAL_DIGIT
	//		{S_ON_NUMERATOR,
	//		&T::_numerator},
	//
	//
	//		// C_BRACKET_OPEN
	//		{"signature ill-formed"},
	//
	//		// C_BRACKET_CLOSE
	//		{"signature ill-formed"},
	//
	//
	//		// C_COMMENT
	//		{"signature ill-formed"},
	//
	//		// C_SEPARATOR
	//		{"signature ill-formed"},
	//
	//		// C_AT_SIGN
	//		{"signature ill-formed"},
	//
	//		// C_SLASH
	//		{"signature ill-formed"},
	//
	//
	//		// C_EOB
	//		{S_EX_NUMERATOR},
	//	},
	//
	//
	//	// -- S_ON_NUMERATOR ------------------------------------------
	//	{
	//		// C_INVALID
	//		{"signature ill-formed"},
	//
	//
	//		// C_NEWLINE
	//		{"signature ill-formed"},
	//
	//		// C_WHITESPACE
	//		{"signature ill-formed"},
	//
	//
	//		// C_UPPER_HEX
	//		{"signature ill-formed"},
	//
	//		// C_UPPER_G
	//		{"signature ill-formed"},
	//
	//		// C_UPPER_OTHER
	//		{"signature ill-formed"},
	//
	//
	//		// C_LOWER_HEX
	//		{"signature ill-formed"},
	//
	//		// C_LOWER_X
	//		{"signature ill-formed"},
	//
	//		// C_LOWER_B
	//		{"signature ill-formed"},
	//
	//		// C_LOWER_O
	//		{"signature ill-formed"},
	//
	//		// C_LOWER_OTHER
	//		{"signature ill-formed"},
	//
	//
	//		// C_ZERO_DIGIT
	//		{S_ON_NUMERATOR,
	//		&T::_numerator},
	//
	//		// C_ONE_DIGIT
	//		{S_ON_NUMERATOR,
	//		&T::_numerator},
	//
	//		// C_OCTAL_DIGIT
	//		{S_ON_NUMERATOR,
	//		&T::_numerator},
	//
	//		// C_DECIMAL_DIGIT
	//		{S_ON_NUMERATOR,
	//		&T::_numerator},
	//
	//
	//		// C_BRACKET_OPEN
	//		{"signature ill-formed"},
	//
	//		// C_BRACKET_CLOSE
	//		{"signature ill-formed"},
	//
	//
	//		// C_COMMENT
	//		{"signature ill-formed"},
	//
	//		// C_SEPARATOR
	//		{"signature ill-formed"},
	//
	//		// C_AT_SIGN
	//		{"signature ill-formed"},
	//
	//		// C_SLASH
	//		{S_EX_DENOMINATOR},
	//
	//
	//		// C_EOB
	//		{S_ON_NUMERATOR},
	//	},
	//
	//
	//	// -- S_EX_DENOMINATOR --------------------------------------
	//	{
	//		// C_INVALID
	//		{"signature ill-formed"},
	//
	//
	//		// C_NEWLINE
	//		{"signature ill-formed"},
	//
	//		// C_WHITESPACE
	//		{"signature ill-formed"},
	//
	//
	//		// C_UPPER_HEX
	//		{"signature ill-formed"},
	//
	//		// C_UPPER_G
	//		{"signature ill-formed"},
	//
	//		// C_UPPER_OTHER
	//		{"signature ill-formed"},
	//
	//
	//		// C_LOWER_HEX
	//		{"signature ill-formed"},
	//
	//		// C_LOWER_X
	//		{"signature ill-formed"},
	//
	//		// C_LOWER_B
	//		{"signature ill-formed"},
	//
	//		// C_LOWER_O
	//		{"signature ill-formed"},
	//
	//		// C_LOWER_OTHER
	//		{"signature ill-formed"},
	//
	//
	//		// C_ZERO_DIGIT
	//		{S_ON_DENOMINATOR,
	//		&T::_denominator},
	//
	//		// C_ONE_DIGIT
	//		{S_ON_DENOMINATOR,
	//		&T::_denominator},
	//
	//		// C_OCTAL_DIGIT
	//		{S_ON_DENOMINATOR,
	//		&T::_denominator},
	//
	//		// C_DECIMAL_DIGIT
	//		{S_ON_DENOMINATOR,
	//		&T::_denominator},
	//
	//
	//		// C_BRACKET_OPEN
	//		{"signature ill-formed"},
	//
	//		// C_BRACKET_CLOSE
	//		{"signature ill-formed"},
	//
	//
	//		// C_COMMENT
	//		{"signature ill-formed"},
	//
	//		// C_SEPARATOR
	//		{"signature ill-formed"},
	//
	//		// C_AT_SIGN
	//		{"signature ill-formed"},
	//
	//		// C_SLASH
	//		{"signature ill-formed"},
	//
	//
	//		// C_EOB
	//		{S_EX_DENOMINATOR},
	//	},
	//
	//
	//	// -- S_ON_DENOMINATOR ----------------------------------------
	//	{
	//		// C_INVALID
	//		{"signature ill-formed"},
	//
	//
	//		// C_NEWLINE
	//		{S_EX_BRACKET_OPEN,
	//		&T::_newline},
	//
	//		// C_WHITESPACE
	//		{S_EX_BRACKET_OPEN},
	//
	//
	//		// C_UPPER_HEX
	//		{"unexpected character"},
	//
	//		// C_UPPER_G
	//		{"unexpected character"},
	//
	//		// C_UPPER_OTHER
	//		{"unexpected character"},
	//
	//
	//		// C_LOWER_HEX
	//		{"unexpected character"},
	//
	//		// C_LOWER_X
	//		{"unexpected character"},
	//
	//		// C_LOWER_B
	//		{"unexpected character"},
	//
	//		// C_LOWER_O
	//		{"unexpected character"},
	//
	//		// C_LOWER_OTHER
	//		{"unexpected character"},
	//
	//
	//		// C_ZERO_DIGIT
	//		{S_ON_DENOMINATOR,
	//		&T::_denominator},
	//
	//		// C_ONE_DIGIT
	//		{S_ON_DENOMINATOR,
	//		&T::_denominator},
	//
	//		// C_OCTAL_DIGIT
	//		{S_ON_DENOMINATOR,
	//		&T::_denominator},
	//
	//		// C_DECIMAL_DIGIT
	//		{S_ON_DENOMINATOR,
	//		&T::_denominator},
	//
	//
	//		// C_BRACKET_OPEN
	//		{S_EX_KEYWORD},
	//
	//		// C_BRACKET_CLOSE
	//		{"unexpected character"},
	//
	//
	//		// C_COMMENT
	//		{S_EX_BRACKET_OPEN,
	//		&T::_comment},
	//
	//		// C_SEPARATOR
	//		{"unexpected character"},
	//
	//		// C_AT_SIGN
	//		{S_EX_NUMERATOR,
	//		&T::_init_signature},
	//
	//		// C_SLASH
	//		{"unexpected character"},
	//
	//		// C_EOB
	//		{S_ON_DENOMINATOR},
	//	},
	//
	//
	//	// -- S_EX_VALUE ----------------------------------------------
	//	{
	//		// C_INVALID
	//		{"invalid character"},
	//
	//
	//		// C_NEWLINE
	//		{S_EX_VALUE,
	//		&T::_newline},
	//
	//		// C_WHITESPACE
	//		{S_EX_VALUE},
	//
	//
	//		// C_UPPER_HEX
	//		{S_ON_NOTE,
	//		&T::_note},
	//
	//		// C_UPPER_G
	//		{S_ON_NOTE,
	//		&T::_note},
	//
	//		// C_UPPER_OTHER
	//		{"unexpected character"},
	//
	//
	//		// C_LOWER_HEX
	//		{"unexpected character"},
	//
	//		// C_LOWER_X
	//		{"unexpected character"},
	//
	//		// C_LOWER_B
	//		{"unexpected character"},
	//
	//		// C_LOWER_O
	//		{"unexpected character"},
	//
	//		// C_LOWER_OTHER
	//		{"unexpected character"},
	//
	//
	//		// C_ZERO_DIGIT
	//		{S_EX_PREFIX},
	//
	//
	//		// C_ONE_DIGIT
	//		{S_ON_DEC,
	//		&T::_decimal},
	//
	//		// C_OCTAL_DIGIT
	//		{S_ON_DEC,
	//		&T::_decimal},
	//
	//		// C_DECIMAL_DIGIT
	//		{S_ON_DEC,
	//		&T::_decimal},
	//
	//
	//		// C_BRACKET_OPEN
	//		{S_EX_KEYWORD},
	//
	//		// C_BRACKET_CLOSE
	//		{"unexpected character"},
	//
	//
	//		// C_COMMENT
	//		{S_EX_VALUE,
	//		&T::_comment},
	//
	//		// C_SEPARATOR
	//		{S_EX_VALUE},
	//
	//		// C_AT_SIGN
	//		{S_EX_NUMERATOR,
	//		&T::_init_signature},
	//
	//		// C_SLASH
	//		{"unexpected character"},
	//
	//		// C_EOB
	//		{S_EX_VALUE},
	//	},
	//
	//
	//	// -- S_ON_NOTE -----------------------------------------------
	//	{
	//		// C_INVALID
	//		{"invalid character"},
	//
	//
	//		// C_NEWLINE
	//		{S_EX_VALUE,
	//		&T::_add_value,
	//		&T::_newline},
	//
	//		// C_WHITESPACE
	//		{S_EX_VALUE,
	//		&T::_add_value},
	//
	//
	//		// C_UPPER_HEX
	//		{S_ON_NOTE,
	//		&T::_add_value,
	//		&T::_note},
	//
	//		// C_UPPER_G
	//		{S_ON_NOTE,
	//		&T::_add_value,
	//		&T::_note},
	//
	//		// C_UPPER_OTHER
	//		{"alteration or octave expected"},
	//
	//
	//		// C_LOWER_HEX
	//		{"alteration or octave expected"},
	//
	//		// C_LOWER_X
	//		{"alteration or octave expected"},
	//
	//
	//		// C_LOWER_B
	//		{S_ON_ALTERATION,
	//		&T::_flat},
	//
	//
	//		// C_LOWER_O
	//		{"alteration or octave expected"},
	//
	//		// C_LOWER_OTHER
	//		{"alteration or octave expected"},
	//
	//
	//		// C_ZERO_DIGIT
	//		{S_ON_OCTAVE,
	//		&T::_octave},
	//
	//		// C_ONE_DIGIT
	//		{S_ON_OCTAVE,
	//		&T::_octave},
	//
	//		// C_OCTAL_DIGIT
	//		{S_ON_OCTAVE,
	//		&T::_octave},
	//
	//		// C_DECIMAL_DIGIT
	//		{S_ON_OCTAVE,
	//		&T::_octave},
	//
	//
	//		// C_BRACKET_OPEN
	//		{S_EX_KEYWORD,
	//		&T::_add_value},
	//
	//		// C_BRACKET_CLOSE
	//		{"alteration or octave expected"},
	//
	//
	//		// C_COMMENT
	//		{S_ON_ALTERATION,
	//		&T::_sharp},
	//
	//		// C_SEPARATOR
	//		{S_EX_VALUE,
	//		&T::_add_value},
	//
	//		// C_AT_SIGN
	//		{S_EX_NUMERATOR,
	//		&T::_add_value,
	//		&T::_init_signature},
	//
	//		// C_SLASH
	//		{"alteration or octave expected"},
	//
	//		// C_EOB
	//		{S_ON_NOTE}
	//	},
	//
	//
	//	// -- S_ON_ALTERATION -----------------------------------------
	//	{
	//		// C_INVALID
	//		{"invalid character"},
	//
	//
	//		// C_NEWLINE
	//		{S_EX_VALUE,
	//		&T::_add_value,
	//		&T::_newline},
	//
	//		// C_WHITESPACE
	//		{S_EX_VALUE,
	//		&T::_add_value},
	//
	//
	//		// C_UPPER_HEX
	//		{S_ON_NOTE,
	//		&T::_add_value,
	//		&T::_note},
	//
	//		// C_UPPER_G
	//		{S_ON_NOTE,
	//		&T::_add_value,
	//		&T::_note},
	//
	//		// C_UPPER_OTHER
	//		{"octave expected"},
	//
	//
	//		// C_LOWER_HEX
	//		{"octave expected"},
	//
	//		// C_LOWER_X
	//		{"octave expected"},
	//
	//		// C_LOWER_B
	//		{"octave expected"},
	//
	//		// C_LOWER_O
	//		{"octave expected"},
	//
	//		// C_LOWER_OTHER
	//		{"octave expected"},
	//
	//
	//		// C_ZERO_DIGIT
	//		{S_ON_OCTAVE,
	//		&T::_octave},
	//
	//		// C_ONE_DIGIT
	//		{S_ON_OCTAVE,
	//		&T::_octave},
	//
	//		// C_OCTAL_DIGIT
	//		{S_ON_OCTAVE,
	//		&T::_octave},
	//
	//		// C_DECIMAL_DIGIT
	//		{S_ON_OCTAVE,
	//		&T::_octave},
	//
	//
	//		// C_BRACKET_OPEN
	//		{S_EX_KEYWORD,
	//		&T::_add_value},
	//
	//		// C_BRACKET_CLOSE
	//		{"octave expected"},
	//
	//
	//		// C_COMMENT
	//		{S_EX_VALUE,
	//		&T::_add_value,
	//		&T::_comment},
	//
	//		// C_SEPARATOR
	//		{S_EX_VALUE,
	//		&T::_add_value},
	//
	//		// C_AT_SIGN
	//		{S_EX_NUMERATOR,
	//		&T::_add_value,
	//		&T::_init_signature},
	//
	//		// C_SLASH
	//		{"octave expected"},
	//
	//		// C_EOB
	//		{S_ON_ALTERATION}
	//	},
	//
	//
	//	// -- S_ON_OCTAVE ---------------------------------------------
	//	{
	//		// C_INVALID
	//		{"invalid character"},
	//
	//
	//		// C_NEWLINE
	//		{S_EX_VALUE,
	//		&T::_add_value,
	//		&T::_newline},
	//
	//		// C_WHITESPACE
	//		{S_EX_VALUE,
	//		&T::_add_value},
	//
	//
	//		// C_UPPER_HEX
	//		{S_ON_NOTE,
	//		&T::_add_value,
	//		&T::_note},
	//
	//		// C_UPPER_G
	//		{S_ON_NOTE,
	//		&T::_add_value,
	//		&T::_note},
	//
	//		// C_UPPER_OTHER
	//		{"unexpected character"},
	//
	//
	//		// C_LOWER_HEX
	//		{"unexpected character"},
	//
	//		// C_LOWER_X
	//		{"unexpected character"},
	//
	//		// C_LOWER_B
	//		{"unexpected character"},
	//
	//		// C_LOWER_O
	//		{"unexpected character"},
	//
	//		// C_LOWER_OTHER
	//		{"unexpected character"},
	//
	//
	//		// C_ZERO_DIGIT
	//		{S_ON_OCTAVE,
	//		&T::_octave},
	//
	//		// C_ONE_DIGIT
	//		{S_ON_OCTAVE,
	//		&T::_octave},
	//
	//		// C_OCTAL_DIGIT
	//		{S_ON_OCTAVE,
	//		&T::_octave},
	//
	//		// C_DECIMAL_DIGIT
	//		{S_ON_OCTAVE,
	//		&T::_octave},
	//
	//
	//		// C_BRACKET_OPEN
	//		{S_EX_KEYWORD,
	//		&T::_add_value},
	//
	//		// C_BRACKET_CLOSE
	//		{"unexpected character"},
	//
	//
	//		// C_COMMENT
	//		{S_EX_VALUE,
	//		&T::_add_value,
	//		&T::_comment},
	//
	//		// C_SEPARATOR
	//		{S_EX_VALUE,
	//		&T::_add_value},
	//
	//		// C_AT_SIGN
	//		{S_EX_NUMERATOR,
	//		&T::_add_value,
	//		&T::_init_signature},
	//
	//		// C_SLASH
	//		{"unexpected character"},
	//
	//		// C_EOB
	//		{S_ON_OCTAVE}
	//	},
	//
	//
	//	// -- S_EX_PREFIX ---------------------------------------------
	//	{
	//		// C_INVALID
	//		{"invalid character"},
	//
	//		// C_NEWLINE
	//		{S_EX_VALUE,
	//		&T::_newline,
	//		&T::_add_value},
	//
	//		// C_WHITESPACE
	//		{S_EX_VALUE,
	//		&T::_add_value},
	//
	//
	//		// C_UPPER_HEX
	//		{S_ON_NOTE,
	//		&T::_add_value,
	//		&T::_note},
	//
	//		// C_UPPER_G
	//		{S_ON_NOTE,
	//		&T::_add_value,
	//		&T::_note},
	//
	//		// C_UPPER_OTHER
	//		{"unexpected character"},
	//
	//
	//		// C_LOWER_HEX
	//		{S_ON_KEYWORD,
	//		&T::_add_value,
	//		&T::_increment}, // chain 0 and keyword
	//
	//		// C_LOWER_X
	//		{S_EX_HEX}, // no keyword starts with 'x'
	//
	//		// C_LOWER_B
	//		{S_EX_BIN}, // no keyword starts with 'b'
	//
	//		// C_LOWER_O
	//		{S_EX_OCT}, // can't chain 0 and 'octave' keyword
	//
	//		// C_LOWER_OTHER
	//		{S_ON_KEYWORD,
	//		&T::_add_value,
	//		&T::_increment}, // chain 0 and keyword
	//
	//
	//		// C_ZERO_DIGIT
	//		{S_ON_DEC}, // no need to add digit 0
	//
	//		// C_ONE_DIGIT
	//		{S_ON_DEC,
	//		&T::_decimal},
	//
	//		// C_OCTAL_DIGIT
	//		{S_ON_DEC,
	//		&T::_decimal},
	//
	//		// C_DECIMAL_DIGIT
	//		{S_ON_DEC,
	//		&T::_decimal},
	//
	//
	//		// C_BRACKET_OPEN
	//		{S_EX_KEYWORD,
	//		&T::_add_value},
	//
	//		// C_BRACKET_CLOSE
	//		{"unexpected character"},
	//
	//
	//		// C_COMMENT
	//		{S_EX_VALUE,
	//		&T::_add_value,
	//		&T::_comment},
	//
	//		// C_SEPARATOR
	//		{S_EX_VALUE,
	//		&T::_add_value},
	//
	//		// C_AT_SIGN
	//		{S_EX_NUMERATOR,
	//		&T::_add_value,
	//		&T::_init_signature},
	//
	//		// C_SLASH
	//		{"unexpected character"},
	//
	//		// C_EOB
	//		{S_EX_PREFIX},
	//	},
	//
	//
	//
	//	// -- S_EX_HEX ------------------------------------------------
	//
	//	{
	//		// C_INVALID
	//		{"invalid character"},
	//
	//
	//		// C_NEWLINE
	//		{"hex value expected"},
	//
	//		// C_WHITESPACE
	//		{"hex value expected"},
	//
	//
	//		// C_UPPER_HEX
	//		{S_ON_HEX,
	//		&T::_upper_hex},
	//
	//		// C_UPPER_G
	//		{"hex value expected"},
	//
	//		// C_UPPER_OTHER
	//		{"hex value expected"},
	//
	//
	//		// C_LOWER_HEX
	//		{S_ON_HEX,
	//		&T::_lower_hex},
	//
	//		// C_LOWER_X
	//		{"hex value expected"},
	//
	//		// C_LOWER_B
	//		{S_ON_HEX,
	//		&T::_lower_hex},
	//
	//		// C_LOWER_O
	//		{"hex value expected"},
	//
	//		// C_LOWER_OTHER
	//		{"hex value expected"},
	//
	//
	//		// C_ZERO_DIGIT
	//		{S_ON_HEX,
	//		&T::_digit_hex},
	//
	//
	//		// C_ONE_DIGIT
	//		{S_ON_HEX,
	//		&T::_digit_hex},
	//
	//		// C_OCTAL_DIGIT
	//		{S_ON_HEX,
	//		&T::_digit_hex},
	//
	//		// C_DECIMAL_DIGIT
	//		{S_ON_HEX,
	//		&T::_digit_hex},
	//
	//
	//		// C_BRACKET_OPEN
	//		{"hex value expected"},
	//
	//		// C_BRACKET_CLOSE
	//		{"hex value expected"},
	//
	//
	//		// C_COMMENT
	//		{"hex value expected"},
	//
	//		// C_SEPARATOR
	//		{"hex value expected"},
	//
	//		// C_AT_SIGN
	//		{"hex value expected"},
	//
	//		// C_SLASH
	//		{"hex value expected"},
	//
	//		// C_EOB
	//		{S_EX_HEX},
	//	},
	//
	//
	//	// -- S_EX_BIN ------------------------------------------------
	//
	//	{
	//		// C_INVALID
	//		{"invalid character"},
	//
	//
	//		// C_NEWLINE
	//		{"bin value expected"},
	//
	//		// C_WHITESPACE
	//		{"bin value expected"},
	//
	//
	//		// C_UPPER_HEX
	//		{"bin value expected"},
	//
	//		// C_UPPER_G
	//		{"bin value expected"},
	//
	//		// C_UPPER_OTHER
	//		{"bin value expected"},
	//
	//
	//		// C_LOWER_HEX
	//		{"bin value expected"},
	//
	//		// C_LOWER_X
	//		{"bin value expected"},
	//
	//		// C_LOWER_B
	//		{"bin value expected"},
	//
	//		// C_LOWER_O
	//		{"bin value expected"},
	//
	//		// C_LOWER_OTHER
	//		{"bin value expected"},
	//
	//
	//		// C_ZERO_DIGIT
	//		{S_ON_BIN,
	//		&T::_bin},
	//
	//		// C_ONE_DIGIT
	//		{S_ON_BIN,
	//		&T::_bin},
	//
	//
	//		// C_OCTAL_DIGIT
	//		{"bin value expected"},
	//
	//		// C_DECIMAL_DIGIT
	//		{"bin value expected"},
	//
	//
	//		// C_BRACKET_OPEN
	//		{"bin value expected"},
	//
	//		// C_BRACKET_CLOSE
	//		{"bin value expected"},
	//
	//
	//		// C_COMMENT
	//		{"bin value expected"},
	//
	//		// C_SEPARATOR
	//		{"bin value expected"},
	//
	//		// C_AT_SIGN
	//		{"bin value expected"},
	//
	//		// C_SLASH
	//		{"bin value expected"},
	//
	//		// C_EOB
	//		{S_EX_BIN},
	//	},
	//
	//
	//	// -- S_EX_OCT ------------------------------------------------
	//
	//	{
	//		// C_INVALID
	//		{"octal digit expected"},
	//
	//
	//		// C_NEWLINE
	//		{"octal digit expected"},
	//
	//		// C_WHITESPACE
	//		{"octal digit expected"},
	//
	//
	//		// C_UPPER_HEX
	//		{"octal digit expected"},
	//
	//		// C_UPPER_G
	//		{"octal digit expected"},
	//
	//		// C_UPPER_OTHER
	//		{"octal digit expected"},
	//
	//
	//		// C_LOWER_HEX
	//		{"octal digit expected"},
	//
	//		// C_LOWER_X
	//		{"octal digit expected"},
	//
	//		// C_LOWER_B
	//		{"octal digit expected"},
	//
	//		// C_LOWER_O
	//		{"octal digit expected"},
	//
	//		// C_LOWER_OTHER
	//		{"octal digit expected"},
	//
	//
	//		// C_ZERO_DIGIT
	//		{S_ON_OCT,
	//		&T::_octal},
	//
	//		// C_ONE_DIGIT
	//		{S_ON_OCT,
	//		&T::_octal},
	//
	//		// C_OCTAL_DIGIT
	//		{S_ON_OCT,
	//		&T::_octal},
	//
	//
	//		// C_DECIMAL_DIGIT
	//		{"octal digit expected"},
	//
	//
	//		// C_BRACKET_OPEN
	//		{"octal digit expected"},
	//
	//		// C_BRACKET_CLOSE
	//		{"octal digit expected"},
	//
	//
	//		// C_COMMENT
	//		{"octal digit expected"},
	//
	//		// C_SEPARATOR
	//		{"octal digit expected"},
	//
	//		// C_AT_SIGN
	//		{"octal digit expected"},
	//
	//		// C_SLASH
	//		{"octal digit expected"},
	//
	//		// C_EOB
	//		{S_EX_OCT},
	//	},
	//
	//
	//	// -- S_ON_DEC ------------------------------------------------
	//	{
	//		// C_INVALID
	//		{"invalid character"},
	//
	//
	//		// C_NEWLINE
	//		{S_EX_VALUE,
	//		&T::_add_value,
	//		&T::_newline},
	//
	//		// C_WHITESPACE
	//		{S_EX_VALUE,
	//		&T::_add_value},
	//
	//
	//		// C_UPPER_HEX
	//		{"decimal digit expected"},
	//
	//		// C_UPPER_G
	//		{"decimal digit expected"},
	//
	//		// C_UPPER_OTHER
	//		{"decimal digit expected"},
	//
	//
	//		// C_LOWER_HEX
	//		{"decimal digit expected"},
	//
	//		// C_LOWER_X
	//		{"decimal digit expected"},
	//
	//		// C_LOWER_B
	//		{"decimal digit expected"},
	//
	//		// C_LOWER_O
	//		{"decimal digit expected"},
	//
	//		// C_LOWER_OTHER
	//		{"decimal digit expected"},
	//
	//
	//		// C_ZERO_DIGIT
	//		{S_ON_DEC,
	//		&T::_decimal},
	//
	//
	//		// C_ONE_DIGIT
	//		{S_ON_DEC,
	//		&T::_decimal},
	//
	//		// C_OCTAL_DIGIT
	//		{S_ON_DEC,
	//		&T::_decimal},
	//
	//		// C_DECIMAL_DIGIT
	//		{S_ON_DEC,
	//		&T::_decimal},
	//
	//
	//		// C_BRACKET_OPEN
	//		{S_EX_KEYWORD,
	//		&T::_add_value},
	//
	//		// C_BRACKET_CLOSE
	//		{"decimal digit expected"},
	//
	//
	//		// C_COMMENT
	//		{S_EX_VALUE,
	//		&T::_add_value,
	//		&T::_comment},
	//
	//		// C_SEPARATOR
	//		{S_EX_VALUE,
	//		&T::_add_value},
	//
	//		// C_AT_SIGN
	//		{S_EX_NUMERATOR,
	//		&T::_add_value,
	//		&T::_init_signature},
	//
	//		// C_SLASH
	//		{"decimal digit expected"},
	//
	//		// C_EOB
	//		{S_ON_DEC},
	//	},
	//
	//
	//	// -- S_ON_HEX ------------------------------------------------
	//	{
	//		// C_INVALID
	//		{"invalid character"},
	//
	//
	//		// C_NEWLINE
	//		{S_EX_VALUE,
	//		&T::_add_value,
	//		&T::_newline},
	//
	//		// C_WHITESPACE
	//		{S_EX_VALUE,
	//		&T::_add_value},
	//
	//
	//		// C_UPPER_HEX
	//		{S_ON_HEX,
	//		&T::_upper_hex},
	//
	//		// C_UPPER_G
	//		{"hex value expected"},
	//
	//		// C_UPPER_OTHER
	//		{"hex value expected"},
	//
	//
	//		// C_LOWER_HEX
	//		{S_ON_HEX,
	//		&T::_lower_hex},
	//
	//		// C_LOWER_X
	//		{"hex value expected"},
	//
	//		// C_LOWER_B
	//		{S_ON_HEX,
	//		&T::_lower_hex},
	//
	//		// C_LOWER_O
	//		{"hex value expected"},
	//
	//		// C_LOWER_OTHER
	//		{"hex value expected"},
	//
	//
	//		// C_ZERO_DIGIT
	//		{S_ON_HEX,
	//		&T::_digit_hex},
	//
	//		// C_ONE_DIGIT
	//		{S_ON_HEX,
	//		&T::_digit_hex},
	//
	//		// C_OCTAL_DIGIT
	//		{S_ON_HEX,
	//		&T::_digit_hex},
	//
	//		// C_DECIMAL_DIGIT
	//		{S_ON_HEX,
	//		&T::_digit_hex},
	//
	//
	//		// C_BRACKET_OPEN
	//		{S_EX_KEYWORD,
	//		&T::_add_value},
	//
	//		// C_BRACKET_CLOSE
	//		{"hex value expected"},
	//
	//
	//		// C_COMMENT
	//		{S_EX_VALUE,
	//		&T::_add_value,
	//		&T::_comment},
	//
	//		// C_SEPARATOR
	//		{S_EX_VALUE,
	//		&T::_add_value},
	//
	//		// C_AT_SIGN
	//		{S_EX_NUMERATOR,
	//		&T::_add_value,
	//		&T::_init_signature},
	//
	//		// C_SLASH
	//		{"hex value expected"},
	//
	//		// C_EOB
	//		{S_ON_HEX},
	//	},
	//
	//
	//	// -- S_ON_BIN ------------------------------------------------
	//	{
	//		// C_INVALID
	//		{"invalid character"},
	//
	//
	//		// C_NEWLINE
	//		{S_EX_VALUE,
	//		&T::_add_value,
	//		&T::_newline},
	//
	//		// C_WHITESPACE
	//		{S_EX_VALUE,
	//		&T::_add_value},
	//
	//
	//		// C_UPPER_HEX
	//		{"binary digit expected"},
	//
	//		// C_UPPER_G
	//		{"binary digit expected"},
	//
	//		// C_UPPER_OTHER
	//		{"binary digit expected"},
	//
	//
	//		// C_LOWER_HEX
	//		{"binary digit expected"},
	//
	//		// C_LOWER_X
	//		{"binary digit expected"},
	//
	//		// C_LOWER_B
	//		{"binary digit expected"},
	//
	//		// C_LOWER_O
	//		{"binary digit expected"},
	//
	//		// C_LOWER_OTHER
	//		{"binary digit expected"},
	//
	//
	//		// C_ZERO_DIGIT
	//		{S_ON_BIN,
	//		&T::_bin},
	//
	//		// C_ONE_DIGIT
	//		{S_ON_BIN,
	//		&T::_bin},
	//
	//
	//		// C_OCTAL_DIGIT
	//		{"binary digit expected"},
	//
	//		// C_DECIMAL_DIGIT
	//		{"binary digit expected"},
	//
	//
	//		// C_BRACKET_OPEN
	//		{S_EX_KEYWORD,
	//		&T::_add_value},
	//
	//		// C_BRACKET_CLOSE
	//		{"binary digit expected"},
	//
	//
	//		// C_COMMENT
	//		{S_EX_VALUE,
	//		&T::_add_value,
	//		&T::_comment},
	//
	//		// C_SEPARATOR
	//		{S_EX_VALUE,
	//		&T::_add_value},
	//
	//		// C_AT_SIGN
	//		{S_EX_NUMERATOR,
	//		&T::_add_value,
	//		&T::_init_signature},
	//
	//		// C_SLASH
	//		{"binary digit expected"},
	//
	//		// C_EOB
	//		{S_ON_BIN},
	//	},
	//
	//
	//	// -- S_ON_OCT ------------------------------------------------
	//	{
	//		// C_INVALID
	//		{"invalid character"},
	//
	//
	//		// C_NEWLINE
	//		{S_EX_VALUE,
	//		&T::_add_value,
	//		&T::_newline},
	//
	//		// C_WHITESPACE
	//		{S_EX_VALUE,
	//		&T::_add_value},
	//
	//
	//		// C_UPPER_HEX
	//		{"octal digit expected"},
	//
	//		// C_UPPER_G
	//		{"octal digit expected"},
	//
	//		// C_UPPER_OTHER
	//		{"octal digit expected"},
	//
	//
	//		// C_LOWER_HEX
	//		{"octal digit expected"},
	//
	//		// C_LOWER_X
	//		{"octal digit expected"},
	//
	//		// C_LOWER_B
	//		{"octal digit expected"},
	//
	//		// C_LOWER_O
	//		{"octal digit expected"},
	//
	//		// C_LOWER_OTHER
	//		{"octal digit expected"},
	//
	//
	//		// C_ZERO_DIGIT
	//		{S_ON_OCT,
	//		&T::_octal},
	//
	//		// C_ONE_DIGIT
	//		{S_ON_OCT,
	//		&T::_octal},
	//
	//		// C_OCTAL_DIGIT
	//		{S_ON_OCT,
	//		&T::_octal},
	//
	//		// C_DECIMAL_DIGIT
	//		{"octal digit expected"},
	//
	//
	//		// C_BRACKET_OPEN
	//		{S_EX_KEYWORD,
	//		&T::_add_value},
	//
	//		// C_BRACKET_CLOSE
	//		{"octal digit expected"},
	//
	//
	//		// C_COMMENT
	//		{S_EX_VALUE,
	//		&T::_add_value,
	//		&T::_comment},
	//
	//		// C_SEPARATOR
	//		{S_EX_VALUE,
	//		&T::_add_value},
	//
	//		// C_AT_SIGN
	//		{S_EX_NUMERATOR,
	//		&T::_add_value,
	//		&T::_init_signature},
	//
	//		// C_SLASH
	//		{"octal digit expected"},
	//
	//		// C_EOB
	//		{S_ON_OCT},
	//	},
	//
	//};
