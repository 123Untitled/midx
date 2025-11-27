#include "language/analyzer.hpp"

#include <unistd.h>
#include <sstream>


// -- P A R S E R -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
mx::analyzer::analyzer(void)
: _data{},
  _lexer{}, _parser{},
  _tokens{},
  _tree{},
  _diagnostic{},
  _highlights{} {
}


// -- public modifiers --------------------------------------------------------

auto mx::analyzer::analyze(std::string&& data) -> void {

	_data = std::move(data);

		_tokens.clear();
	_diagnostic.clear();

	// lex
	_lexer.lex(_data, _tokens, _diagnostic);

	_tokens.debug();

	// parse
	_parser.parse(_tokens, _tree, _diagnostic);


	std::stringstream ss;

	// --- premier objet : highlights ---
	//ss << "{\"type\":\"highlight\",\"highlights\":[";

	//bool first = true;
	//for (mx::usz i = 0U; i < _tokens.raw_size(); ++i) {
	//	const auto& t           = _tokens[i];
	//	const char* group       = tk::vim_highlight[t.id];
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

	//_tokens.for_each

	ss << "{\"type\":\"highlight\",\"highlights\":[";
	bool first = true;

	_tokens.for_each(
		[](const tk::const_token_view& tv, std::stringstream& ss, bool& first) static -> void {
			const auto& group = tk::highlight[tv.id()];
			tv.for_each_chunk(
				[](const tk::chunk& ch, std::stringstream& ss, bool& first, const char* group) static -> void {
					if (!first)
						ss << ",";
					else
						first = false;
					ss << "{\"l\":"   << ch.range.ln
					   << ",\"s\":"   << ch.range.cs
					   << ",\"e\":"   << ch.range.ce
					   << ",\"g\":\"" << group
					   << "\"}";
			}, ss, first, group);
	}, ss, first);

	ss << "]}\r\n";



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
