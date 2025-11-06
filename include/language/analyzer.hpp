#ifndef language_analyzer_hpp
#define language_analyzer_hpp

#include "language/lexer.hpp"
#include "language/parser.hpp"
#include "language/tokens/token_list.hpp"
#include "language/ast/tree.hpp"
#include "language/diagnostic.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// --  A N A L Y Z E R ----------------------------------------------------

	class analyzer final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::analyzer;


			// -- private members ---------------------------------------------

			/* lexer */
			lx::lexer _lexer;

			/* parser */
			pr::parser _parser;

			/* token list */
			tk::token_list _tokens;

			/* ast */
			as::tree _tree;

			/* diagnostic */
			an::diagnostic _diagnostic;


			std::string _highlights;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			analyzer(void);


			// -- public methods ----------------------------------------------

			/* analyze */
			auto analyze(void) -> void;

			/* analyze from string */
			auto analyze(const std::string&) -> void;

			/* highlights */
			auto highlights(void) const noexcept -> const std::string& {
				return _highlights;
			}

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
