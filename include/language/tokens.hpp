#ifndef language_tokens_hpp
#define language_tokens_hpp

//#include "types.hpp"
#include "language/lexer/lexeme.hpp"

#include <iostream>

/*
tk::raw
tk::seg

tk::raw
tk::syn

tk::raw
tk::sym

tk::raw
tk::mod

tk::raw
tk::ast

*/


// -- T K  N A M E S P A C E --------------------------------------------------

namespace tk {

	/* token id */
	using id = mx::u8;



	struct range final {
		// line number
		mx::usz ln;
		// column start
		mx::usz cs;
		// column end
		mx::usz ce;
	};

	struct chunk final {
		// buffer lexeme
		lx::lexeme lexeme;
		// file range
		tk::range range;
		// next chunk index
		mx::usz next;
	};

	struct token final {
		// token id
		tk::id id;
		mx::u8 reserved[sizeof(mx::usz) - sizeof(tk::id)];
		// start index in _chunks
		mx::usz start;
	};








	// -- R A W  N A M E S P A C E --------------------------------------------

	namespace raw {


		/* token ids */
		enum : tk::id {

			// = + - * /
			equal,
			plus,
			hyphen,
			asterisk,
			slash,

			/* . & : ^ \ */
			dot,
			ampersand,
			colon,
			caret,
			backslash,

			// () [] {}
			round_open,
			round_close,
			square_open,
			square_close,
			curly_open,
			curly_close,

			text,

			note,
			binary,
			octal,
			decimal,
			hexadecimal,

			comment,
			invalid,
			end_of_tokens
		};

		constexpr const char* names[] {

			// = + - * /
			"equal",
			"plus",
			"hyphen",
			"asterisk",
			"slash",

			/* . & : ^ \ */
			"dot",
			"ampersand",
			"colon",
			"caret",
			"backslash",

			// () [] {}
			"round_open",
			"round_close",
			"square_open",
			"square_close",
			"curly_open",
			"curly_close",

			"text",

			"note",
			"binary",
			"octal",
			"decimal",
			"hexadecimal",

			"comment",
			"invalid",
			"end_of_tokens",

		}; // names

		constexpr const char* colors[] {

			// = + - * /
			"\x1b[33m",
			"\x1b[33m",
			"\x1b[33m",
			"\x1b[33m",
			"\x1b[33m",

			/* . & : ^ \ */
			"\x1b[33m",
			"\x1b[33m",
			"\x1b[33m",
			"\x1b[33m",
			"\x1b[33m",

			// () [] {}
			"\x1b[90m",
			"\x1b[90m",
			"\x1b[90m",
			"\x1b[90m",
			"\x1b[90m",
			"\x1b[90m",

			// text
			"\x1b[37m",

			// note bin, oct, dec, hex
			"\x1b[35m",
			"\x1b[35m",
			"\x1b[35m",
			"\x1b[35m",
			"\x1b[35m",

			// comment
			"\x1b[90m",

			// invalid
			"\x1b[0m",
			// end_of_tokens
			"\x1b[0m"
		};


		constexpr const char* vim_highlight[] {
			// = + - * /
			"Operator",
			"Operator",
			"Operator",
			"Operator",
			"Operator",

			/* . & : ^ \ */
			"Operator",
			"Operator",
			"Operator",
			"Operator",
			"Operator",

			// () [] {}
			"Delimiter",
			"Delimiter",
			"Delimiter",
			"Delimiter",
			"Delimiter",
			"Delimiter",

			// text
			"Identifier",

			"Number",
			"Number",
			"Number",
			"Number",
			"Number",

			// comment
			"Comment",

			// invalid
			"Normal",
			// end_of_tokens
			"Normal"
		};



		struct range final {
			// line number
			mx::usz ln;
			// column start
			mx::usz cs;
			// column end
			mx::usz ce;
		};


		// -- T O K E N -----------------------------------------------------------

		struct token final {


			private:
				
				// -- private types -----------------------------------------------

				/* self type */
				using self = tk::raw::token;


			public:

				tk::id id;
				tk::raw::range range;
				lx::lexeme lexeme;

		}; // class token

	} // namespace raw



	// -- forward declarations ------------------------------------------------

	/* map iterator */
	class const_map_iterator;
	class tokens;


	// -- S Y N  N A M E S P A C E --------------------------------------------

	namespace syn {


		enum : tk::id {

			// tempo ^2 \2
			tempo_fast,
			tempo_slow,


			// parallel |
			parallel,

			// crossfade ><
			crossfade,

			// assignment =
			assignment,


			// identifier
			identifier,

			// parameter .text
			parameter,

			// reference &text
			reference,

			// numbers
			number_positive,
			number_negative,

			// () [] {}
			priority_open,
			priority_close,
			permutation_open,
			permutation_close,
			condition_open,
			condition_close,


			end_of_tokens
		};


		constexpr const char* names[] {

			// tempo ^2 \2
			"tempo_fast",
			"tempo_slow",

			// parallel |
			"parallel",

			// crossfade ><
			"crossfade",

			// assignment =
			"assignment",

			// identifier
			"identifier",

			// parameter .text
			"parameter",

			// reference &text
			"reference",

			// numbers
			"number_positive",
			"number_negative",

			// () [] {}
			"priority_open",
			"priority_close",
			"permutation_open",
			"permutation_close",
			"condition_open",
			"condition_close",

			"end_of_tokens"

		}; // names

		constexpr const char* colors[] {

			// tempo ^2 \2
			"\x1b[32m",
			"\x1b[32m",

			// parallel |
			"\x1b[36m",

			// crossfade ><
			"\x1b[36m",

			// assignment =
			"\x1b[33m",

			// identifier
			"\x1b[37m",

			// parameter .text
			"\x1b[35m",

			// reference &text
			"\x1b[34m",


			// numbers
			"\x1b[31m",
			"\x1b[31m",


			// () [] {}
			"\x1b[90m",
			"\x1b[90m",
			"\x1b[90m",
			"\x1b[90m",
			"\x1b[90m",
			"\x1b[90m",

			// end_of_tokens
			"\x1b[0m"
		};


		constexpr const char* vim_highlight[] {

			// tempo ^2 \2
			"Constant",
			"Constant",


			// parallel |
			"Operator",

			// crossfade ><
			"Operator",

			// assignment =
			"Operator",

			// identifier
			"Identifier",

			// parameter .text
			"Type",

			// reference &text
			"String",

			// numbers
			"Number",
			"Number",


			// () [] {}
			"Delimiter",
			"Delimiter",
			"Delimiter",
			"Delimiter",
			"Delimiter",
			"Delimiter",

			// end_of_tokens
			"Normal"


		};

		// -- T O K E N -------------------------------------------------------

		struct token final {


			// -- friends -----------------------------------------------------

			/* tokens as friend */
			friend class tk::tokens;


			private:
				
				// -- private types -------------------------------------------

				/* self type */
				using self = tk::syn::token;


				// -- private lifecycle ---------------------------------------

				/* constructor */
				token(const tk::id,
					  const mx::usz,
					  const mx::usz) noexcept;


			public:

				// -- public members ------------------------------------------

				/* token id */
				tk::id id;

				/* start index */
				mx::usz start;

				/* count */
				mx::usz count;


				// -- public lifecycle ----------------------------------------

				/* deleted default constructor */
				token(void) = delete;

				/* iterators constructor */
				//token(const tk::id,
				//	  const tk::const_map_iterator&,
				//	  const tk::const_map_iterator&) noexcept;

		}; // class token

	} // namespace syn

} // namespace tk



// overload output operator
inline auto operator<<(std::ostream& os, const tk::raw::token& t) -> std::ostream& {

	os << "[" << tk::raw::colors[t.id] << tk::raw::names[t.id] << "\x1b[0m - ";
	const char* ptr = (const char*)t.lexeme.data;
	if (ptr == nullptr) {
		os << "nullptr]";
		return os;
	}
	for (mx::usz i = 0U; i < t.lexeme.size; ++i)
		os << ptr[i];
	os << "]";
	return os;
}

#endif // language_tokens_hpp
