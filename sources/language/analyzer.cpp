#include "language/analyzer.hpp"

#include <unistd.h>
#include <sstream>

#include "language/syntax/syntax_layout.hpp"


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
		  _tree.clear();
	_diagnostic.clear();

	// lex
	_lexer.lex(br, _tokens, _diagnostic);

	// parse
	_parser.parse(_tokens, _diagnostic, _tree);

	//_tree.debug();

	sx::builder b;

	b.build(_tree, _diagnostic);

	//_tokens.debug(br);




	std::stringstream ss;

	// ouvre le tableau JSON racine
	ss << "[";

	// --- premier objet : highlights ---
	ss << "{\"type\":\"highlight\",\"highlights\":[";

	bool first = true;
	_tokens.for_each([](const tk::token& t, std::stringstream& ss, bool& first) static -> void {
			const char* group = tk::token_to_highlight[t.id];
			const ml::usz line = t.line;
			const ml::usz col_start = t.col_head;
			const ml::usz col_end = t.col_tail;

			if (!first) ss << ",";
			first = false;

			ss << "{\"line\":" << line
			<< ",\"col_start\":" << col_start
			<< ",\"col_end\":" << col_end
			<< ",\"group\":\"" << group << "\"}";
			}, ss, first);

	ss << "]},"; // ferme le 1er objet

	// --- deuxième objet : diagnostics ---
	ss << "{\"type\":\"diagnostic\",\"diagnostics\":[";

	first = true;
	for (const auto& e : _diagnostic._entries) {
		const char* level_str = (e.level == an::diagnostic::level_type::ERROR)
			? "error" : "warning";

		if (!first) ss << ",";
		first = false;

		ss << "{\"severity\":\"" << level_str
			<< "\",\"message\":\"" << e.msg
			<< "\",\"line\":" << e.line
			<< ",\"col_start\":" << e.col_start
			<< ",\"col_end\":" << e.col_end
			<< "}";
	}

	ss << "]}]";


	_highlights = ss.str();

	//_diag_json = _diagnostic.to_json();
}
