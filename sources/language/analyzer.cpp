#include "language/analyzer.hpp"

#include <unistd.h>
#include <sstream>


// -- P A R S E R -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
mx::analyzer::analyzer(void)
: _lexer{}, _parser{} {
}


// -- public modifiers --------------------------------------------------------

auto mx::analyzer::analyze(std::string&& data) -> void {

	_data = std::move(data);

		_tokens.clear();
		  _tree.clear();
	_diagnostic.clear();
		 _model.clear();
	   _builder.clear();

	// lex
	_lexer.lex(_data, _tokens, _diagnostic);

	//_tokens.debug();

	// parse
	_parser.parse(_tokens, _diagnostic, _tree);

	_tokens.debug_syn();

	//_builder.build(_tree, _model, _diagnostic);

	//_tree.debug();
	//_tokens.debug(br);




	std::stringstream ss;

	// --- premier objet : highlights ---
	//ss << "{\"type\":\"highlight\",\"highlights\":[";

	//bool first = true;
	//for (mx::usz i = 0U; i < _tokens.raw_size(); ++i) {
	//	const auto& t           = _tokens[i];
	//	const char* group       = tk::raw::vim_highlight[t.id];
	//
	//	if (!first)
	//		ss << ",";
	//	first = false;
	//
	//	ss << "{\"l\":" << t.range.ln
	//	   << ",\"s\":" << t.range.cs
	//	   << ",\"e\":" << t.range.ce
	//	   << ",\"g\":\"" << group << "\"}";
	//}
	//
	//ss << "]}\r\n";


	ss << "{\"type\":\"highlight\",\"highlights\":[";
	bool first = true;
	for (const auto& sv : _tokens.syn_tokens()) {

		const auto& t           = sv.token();
		const char* group       = tk::syn::vim_highlight[t.id];


		for (const auto& mt : sv) {
			if (!first)
				ss << ",";
			else
				first = false;
			ss << "{\"l\":" << mt.range.ln
			   << ",\"s\":" << mt.range.cs
			   << ",\"e\":" << mt.range.ce
			   << ",\"g\":\"" << group << "\"}";

		}
	}

	ss << "]}\r\n";

	int i = 0;

	i += 3;


	// --- deuxième objet : diagnostics ---
	ss << "{\"type\":\"diagnostic\",\"diagnostics\":[";

	first = true;
	for (const auto& e : _diagnostic._entries) {

		if (!first)
			ss << ",";
		first = false;

		ss << "{\"m\":\"" << e.msg
		   << "\",\"l\":" << e.range.ln
			<< ",\"s\":" << e.range.cs
			<< ",\"e\":" << e.range.ce
			<< "}";
	}

	ss << "]}\r\n";

	_highlights = ss.str();

	//std::string json = ss.str();

	//_highlights = "Content-Length: " + std::to_string(json.size()) + "\r\n\r\n" + json;
}
