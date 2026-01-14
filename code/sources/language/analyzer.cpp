#include "language/analyzer.hpp"
#include "string_pool.hpp"


// -- A N A L Y Z E R ---------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
mx::analyzer::analyzer(void)
: _data{},
  _tokens{},
  _lexer{}, _parser{},
  _tree{},
  _diagnostic{} {
}


// -- public modifiers --------------------------------------------------------

auto mx::analyzer::analyze(mx::string&& data) -> void {

	mx::string_pool::store(std::move(_data));
	_data = std::move(data);

		_tokens.clear();
	_diagnostic.clear();

	// lex
	_lexer.lex(_data, _tokens, _diagnostic);

	_tokens.debug();

	// parse
	_parser.parse(_tokens, _tree, _diagnostic);
}


/* highlights */
auto mx::analyzer::highlights(void) -> mx::string {

	auto str = mx::string_pool::query();

	// highlights object
	str.append("{\"type\":\"highlight\",\"highlights\":[");

	_tokens.for_each(
		[](const tk::const_token_view& tv, mx::string& str) static -> void {

			const auto& group = tk::highlight[tv.id()];

			tv.for_each_chunk(
				[](const tk::chunk& ch, mx::string& str, const char* group) static -> void {

					str.append("{\"l\":", ch.range.ln,
							   ",\"s\":", ch.range.cs,
							   ",\"e\":", ch.range.ce,
							   ",\"g\":\"", group, "\"},");

			}, str, group);
	}, str);

	// remove last comma and close array
	if (str.back() == ',')
		str.pop_back();
	// close object
	str.append("]}\r\n");



	// diagnostics object
	str.append("{\"type\":\"diagnostic\",\"diagnostics\":[");

	for (const auto& e : _diagnostic._entries) {

		str.append("{\"m\":\"", e.msg,
				"\",\"l\":", e.range.ln,
				",\"s\":",   e.range.cs,
				",\"e\":",   e.range.ce,
				"},"
				);
	}

	// remove last comma and close array
	if (str.back() == ',')
		str.pop_back();
	// close object
	str.append("]}\r\n");

	return str;
}
