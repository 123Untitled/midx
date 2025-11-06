#ifndef language_tokens_hpp
#define language_tokens_hpp

#include "types.hpp"
#include "language/lexer/lexeme.hpp"

#include <iostream>

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
		specifier,
		param_dot,
		parameter
	};

	constexpr const char* token_names[] {
		"bracket_open",
		"bracket_close",
		"at_sign",
		"slash",
		"dot",
		"text",
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
		"specifier",
		"param_dot",
		"parameter"
	};

	constexpr const char* color_operator = "\x1b[33m";
	constexpr const char* color_identifier = "\x1b[37m";
	constexpr const char* color_string = "\x1b[32m";
	constexpr const char* color_number = "\x1b[35m";
	constexpr const char* color_comment = "\x1b[90m";
	constexpr const char* color_keyword = "\x1b[34m";
	constexpr const char* color_type = "\x1b[36m";
	constexpr const char* color_normal = "\x1b[0m";

	constexpr const char* token_colors[] {
		color_comment,
		color_comment,
		color_operator,
		color_operator,
		color_operator,
		color_keyword,
		//color_normal,
		color_string,
		color_number,
		color_number,
		color_number,
		color_number,
		color_number,
		color_normal,
		color_comment,
		color_normal,
		"\x1b[4m", // underlined
		color_comment,
		color_type,
		color_keyword,
		color_keyword
	};

	// specifier rouge
	// identifier bleu
	// number yellow

	constexpr const char* token_to_highlight[] {
		"Comment",
		"Comment",
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
		"Type",
		//"Keyword",
		//"Keyword"
		"Statement",
		"Statement"
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

	// overload output operator
inline auto operator<<(std::ostream& os, const tk::token& t) -> std::ostream& {

	os << "[" << tk::token_colors[t.id] << tk::token_names[t.id] << "\x1b[0m - ";
	const char* ptr = (const char*)t.lexeme.data;
	if (ptr == nullptr) {
		os << "nullptr]";
		return os;
	}
	for (ml::usz i = 0U; i < t.lexeme.size; ++i)
		os << ptr[i];
	os << "]";
	return os;
}

#endif // language_tokens_hpp
