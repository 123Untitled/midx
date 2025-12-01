#include "language/analyzer.hpp"


// -- A N A L Y Z E R ---------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
mx::analyzer::analyzer(void)
: _data{},
  _lexer{}, _parser{},
  _tokens{},
  _tree{},
  _diagnostic{} {
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

}

#include "string_pool.hpp"

/* highlights */
auto mx::analyzer::highlights(void) -> std::string {

	auto str = mx::string_pool::query();

	// first object : highlights
	str.append("{\"type\":\"highlight\",\"highlights\":[");
	bool first = true;

	_tokens.for_each(
		[](const tk::const_token_view& tv, std::string& str, bool& first) static -> void {
			const auto& group = tk::highlight[tv.id()];
			tv.for_each_chunk(
				[](const tk::chunk& ch, std::string& str, bool& first, const char* group) static -> void {
					if (!first)
						str.push_back(',');
					else
						first = false;

					str.append("{\"l\":");
					str.append(std::to_string(ch.range.ln));
					str.append(",\"s\":");
					str.append(std::to_string(ch.range.cs));
					str.append(",\"e\":");
					str.append(std::to_string(ch.range.ce));
					str.append(",\"g\":\"");
					str.append(group);
					str.append("\"}");
			}, str, first, group);
	}, str, first);

	str.append("]}\r\n");



	// second object : diagnostics
	str.append("{\"type\":\"diagnostic\",\"diagnostics\":[");

	first = true;
	for (const auto& e : _diagnostic._entries) {

		if (!first)
			str.push_back(',');
		first = false;

		str.append("{\"m\":\"");
		str.append(e.msg);
		str.append("\",\"l\":");
		str.append(std::to_string(e.range.ln));
		str.append(",\"s\":");
		str.append(std::to_string(e.range.cs));
		str.append(",\"e\":");
		str.append(std::to_string(e.range.ce));
		str.append("}");
	}

	str.append("]}\r\n");

	return str;
}
