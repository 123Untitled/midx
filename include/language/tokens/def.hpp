#ifndef language_tokens_hpp
#define language_tokens_hpp

#include "language/lexer/lexeme.hpp"


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


		public:

			// buffer lexeme
			lx::lexeme lexeme;
			// file range
			tk::range range;

			chunk(void) noexcept = default;
			chunk(const lx::lexeme& lexeme, const tk::range& range) noexcept
			: lexeme{lexeme}, range{range} {
			}
			chunk(const tk::chunk&) noexcept = default;
			auto operator=(const tk::chunk&) noexcept -> tk::chunk& = default;
	};

	struct token final {
		// token id
		tk::id id;
		// start index in groups
		mx::usz start;
		// count
		mx::usz count;

		auto operator==(const tk::id type) const noexcept -> bool {
			return this->id == type;
		}
	};


	/* token ids */
	enum : tk::id {


		// identifier [a-z][a-zA-Z0-9_]*
		identifier,

		// separator ;
		separator,



		// tempo ^2 \2
		tempo_fast,
		tempo_slow,

		// modulo %
		modulo,

		// parallel |
		parallel,

		// crossfade ><
		crossfade,


		// track separator
		track_separator,

		// parameter
		parameter,

		// references
		reference,
		param_reference,

		// numbers
		note,
		decimal,
		floating,

		// () [] {}
		priority_open,
		priority_close,
		permutation_open,
		permutation_close,
		condition_open,
		condition_close,

		// comment
		comment,

		// invalid
		invalid,

		// end of tokens
		end_of_tokens,

		// total number of tokens
		max_tokens
	};

		// + - * /
		//add,
		//subtract,
		//multiply,
		//divide,

	// end of tokens
	static tk::token eot {
		tk::end_of_tokens, 0U, 0U
	};


		   // 7 () []   grouping/postfix N/A

		   // 6 * /     infix    left
		   // 5 + -     infix    left

		   // 4 ^n \n   prefix   right

		   // 3 :param  prefix   right

		   // 2 ><      infix    left
		   // 1 |       infix    (n-aire)

		   // 0 =       infix    right



	/* token names */
	constexpr const char* names[] {

		// identifier
		"identifier",

		// separator ;
		"separator",


		// tempo ^2 \2
		"tempo_fast",
		"tempo_slow",

		// modulo %
		"modulo",

		// parallel |
		"parallel",

		// crossfade ><
		"crossfade",


		// track separator
		"track_separator",

		// parameter :param
		"parameter",

		// references
		"reference",
		"param_reference",

		// numbers
		"note",
		"decimal",
		"floating",

		// () [] {}
		"priority_open",
		"priority_close",
		"permutation_open",
		"permutation_close",
		"condition_open",
		"condition_close",

		// comment
		"comment",

		// invalid
		"invalid",

		// end_of_tokens
		"end_of_tokens"

	}; // names

	/* max name length */
	constexpr mx::usz max_name_len =
		[](void) static constexpr noexcept -> mx::usz {

			mx::usz max = 0U;
			for (mx::u8 i = 0U; i <= tk::end_of_tokens; ++i) {

				const auto len =
					[](const char* str) static constexpr noexcept -> mx::usz {

						mx::usz l = 0U;
						while (str[l] != '\0')
							++l;
						return l;
					}(tk::names[i]);

				max = (len > max) ? len : max;
			}
			return max;
	}();


	constexpr const char* colors[] {

		// identifier
		"\x1b[37m",

		// separator ;
		"\x1b[90m",


		// tempo ^2 \2
		"\x1b[32m",
		"\x1b[32m",

		// modulo %
		"\x1b[36m",


		// parallel |
		"\x1b[36m",

		// crossfade ><
		"\x1b[36m",


		// block start
		"\x1b[90m",

		// parameter
		"\x1b[35m",

		// references
		"\x1b[34m",
		"\x1b[94m",

		// numbers
		"\x1b[31m",
		"\x1b[31m",
		"\x1b[31m",

		// () [] {}
		"\x1b[90m",
		"\x1b[90m",
		"\x1b[90m",
		"\x1b[90m",
		"\x1b[90m",
		"\x1b[90m",

		// comment
		"\x1b[90m",

		// invalid
		"\x1b[0m",

		// end_of_tokens
		"\x1b[0m"
	};


	constexpr const char* highlight[] {

		// identifier
		"Identifier",
		// separator ;
		"Delimiter",


		// tempo ^2 \2
		"Type",
		"Type",

		// modulo %
		"Type",


		// parallel |
		"Operator",

		// crossfade ><
		"Operator",


		// block start
		"Comment",

		// parameter
		"Keyword",

		// references
		"Function",
		"Function",

		// numbers
		"Number",
		"Number",
		"Number",

		// () [] {}
		"Delimiter",
		"Delimiter",
		"Delimiter",
		"Delimiter",
		"Delimiter",
		"Delimiter",

		// comment
		"Comment",

		// invalid
		//"DiagnosticUnderlineError",
		"Underlined",

		// end_of_tokens
		"None"
	};

} // namespace tk


/* operator<< overload */
inline auto operator<<(std::ostream& os, const tk::token& t) -> std::ostream& {
	os << tk::colors[t.id] << tk::names[t.id] << "\x1b[0m";
	return os;
}

#endif // language_tokens_hpp
