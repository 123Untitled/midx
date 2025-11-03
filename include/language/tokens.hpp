#ifndef language_tokens_hpp
#define language_tokens_hpp

#include "types.hpp"
#include "language/lexer/lexeme.hpp"


// -- T K  N A M E S P A C E --------------------------------------------------

namespace tk {


	/* token id */
	using id = ml::u8;


	/* token ids */
	enum : tk::id {
		bracket_open,
		bracket_close,
		at_sign,
		slash,
		dot,
		text,
		string,
		note,
		binary,
		octal,
		decimal,
		hexadecimal,
		end_of_tokens,
		comment,
		whitespace,
		invalid,
		nested_block,
		param_dot,
		parameter
	};

	constexpr const char* token_names[] {
		"bracket_open",
		"bracket_close",
		"at_sign",
		"slash",
		"dot",
		"identifier",
		"string",
		"note",
		"binary",
		"octal",
		"decimal",
		"hexadecimal",
		"end_of_tokens",
		"comment",
		"whitespace",
		"invalid",
		"nested_block",
		"param_dot",
		"param"
	};
	constexpr const char* token_to_highlight[] {
		"Operator",
		"Operator",
		"Operator",
		"Operator",
		"Operator",
		"Identifier",
		"String",
		"Number",
		"Number",
		"Number",
		"Number",
		"Number",

		// end_of_tokens
		"Normal",
		"Comment",
		"Whitespace",
		"Underlined",
		"Comment",
		"Keyword",
		"Keyword"
	};


	// -- T O K E N -----------------------------------------------------------

	struct token final {
		tk::id     id;
		lx::lexeme lexeme;
		ml::uint line;
		ml::uint col_head;
		ml::uint col_tail;
	}; // class token


} // namespace tk

#endif // language_tokens_hpp
