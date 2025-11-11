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

auto ml::analyzer::analyze(std::string& data) -> void {

	_data = std::move(data);

	const ml::byte_range br{
		reinterpret_cast<const ml::u8*>(_data.data()),
		reinterpret_cast<const ml::u8*>(_data.data() + _data.size())
	};

		_tokens.clear();
		  _tree.clear();
	_diagnostic.clear();
		 _model.clear();
	   _builder.clear();

	// lex
	_lexer.lex(br, _tokens, _diagnostic);

	// parse
	_parser.parse(_tokens, _diagnostic, _tree);

	_builder.build(_tree, _model, _diagnostic);

	//_tree.debug();
	//_tokens.debug(br);




	std::stringstream ss;

	// --- premier objet : highlights ---
	ss << "{\"type\":\"highlight\",\"highlights\":[";

	bool first = true;
	for (mx::usz i = 0U; i < _tokens.size(); ++i) {
		const auto& t           = _tokens[i];
		const char* group       = tk::token_to_highlight[t.id];

		if (!first)
			ss << ",";
		first = false;

		ss << "{\"l\":" << t.line
		   << ",\"s\":" << t.col_head
		   << ",\"e\":" << t.col_tail
		   << ",\"g\":\"" << group << "\"}";
	}

	ss << "]}\r\n";

	// --- deuxième objet : diagnostics ---
	ss << "{\"type\":\"diagnostic\",\"diagnostics\":[";

	first = true;
	for (const auto& e : _diagnostic._entries) {

		if (!first)
			ss << ",";
		first = false;

		ss << "{\"m\":\"" << e.msg
		   << "\",\"l\":" << e.line
			<< ",\"s\":" << e.col_start
			<< ",\"e\":" << e.col_end
			<< "}";
	}

	ss << "]}\r\n";

	_highlights = ss.str();

	//std::string json = ss.str();

	//_highlights = "Content-Length: " + std::to_string(json.size()) + "\r\n\r\n" + json;
}
