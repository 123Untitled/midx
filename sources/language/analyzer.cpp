#include "language/analyzer.hpp"

#include <unistd.h>
#include <sstream>

// -- P A R S E R -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
ml::analyzer::analyzer(void)
: _lexer{}, _parser{} {
}


// -- public modifiers --------------------------------------------------------

auto ml::analyzer::analyze(const std::string& data) -> void {

	const ml::byte_range br{
		reinterpret_cast<const ml::u8*>(data.data()),
		reinterpret_cast<const ml::u8*>(data.data() + data.size())
	};

	_tokens.clear();

	// lex
	_lexer.lex(br, _tokens);

	_tokens.debug(br);

	// parse
	_parser.parse(br, _tokens);



	//{
	//	"type": "highlight",
	//		"highlights": [
	//		{ "line": 0, "col_start": 0, "col_end": 5, "group": "Keyword" },
	//		{ "line": 1, "col_start": 0, "col_end": 3, "group": "Comment" }
	//		]
	//}

	std::stringstream ss;
	ss << "{\n\t\"type\": \"highlight\",\n\t\"highlights\": [\n";

	bool first = true;

	_tokens.for_each([](const tk::token& t, std::stringstream& ss, bool& first) static -> void {

			const char* group = tk::token_to_highlight[t.id];
			const ml::usz line = t.line;
			const ml::usz col_start = t.col_head;
			const ml::usz col_end = t.col_tail;

			if (first == false)
				ss << ",\n";
			else
				first = false;

			ss << "\t\t{ \"line\": " << line
			   << ", \"col_start\": " << col_start
			   << ", \"col_end\": " << col_end
			   << ", \"group\": \"" << group << "\" }";

	}, ss, first);

	ss << "\n\t]\n}\n";


	_highlights = ss.str();
}
