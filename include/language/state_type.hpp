#ifndef ml_language_state_type_hpp
#define ml_language_state_type_hpp


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	/* state type */
	enum state_type : unsigned {

		S_EX_BRACKET_OPEN,
		S_EX_KEYWORD,
		S_ON_KEYWORD,
		S_EX_BRACKET_CLOSE,

		S_ON_COMMENT,

		S_EX_NUMERATOR,
		S_ON_NUMERATOR,
		S_EX_DENOMINATOR,
		S_ON_DENOMINATOR,


		S_EX_VALUE,

		S_ON_NOTE,
		S_ON_ALTERATION,
		S_ON_OCTAVE,

		S_EX_PREFIX,
		S_EX_HEX,
		S_EX_BIN,
		S_EX_OCT,

		S_ON_DEC,
		S_ON_HEX,
		S_ON_BIN,
		S_ON_OCT,



		S_MAX_TYPE,

		S_NONE = S_MAX_TYPE,

		S_ERROR = S_MAX_TYPE
	};

} // namespace ml

#endif // ml_language_state_type_hpp
