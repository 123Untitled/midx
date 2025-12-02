#include "language/highlight_tracker.hpp"


// -- H I G H L I G H T  T R A C K E R ----------------------------------------

// -- private methods ---------------------------------------------------------

/* emit highlight JSON
   helper to generate JSON for a single highlight */
static auto emit_highlight(std::string& out,
						   const tk::const_token_view& tv,
						   const char* group) -> void {

	tv.for_each_chunk(
		[](const tk::chunk& ck, std::string& s, const char* group) static -> void {
			const auto& r = ck.range;

			s.append("{\"l\":");
			s.append(std::to_string(r.ln));
			s.append(",\"s\":");
			s.append(std::to_string(r.cs));
			s.append(",\"e\":");
			s.append(std::to_string(r.ce));
			s.append(",\"g\":\"");
			s.append(group);
			s.append("\"},");
		}, out, group
	);
}


// -- public methods ----------------------------------------------------------

/* mark active
   mark a highlight as active with expiration time */
auto mx::highlight_tracker::mark_active(const mx::usz token,
										const char* group) -> void {
	if (_tokens == nullptr)
		return;
	(this->*_mark)(token, group);
}

/* mark active check
   mark a highlight as active, checking if it already exists */
auto mx::highlight_tracker::_mark_active_check(const mx::usz token,
											   const char* group) -> void {

	// check if token already exists
	const auto it = _old.find(token);
	if (it == _old.end())
		_mark = &self::_mark_active_force;
	_now[token] = group;
}

/* mark active force
   mark a highlight as active, forcing addition */
auto mx::highlight_tracker::_mark_active_force(const mx::usz token,
											   const char* group) -> void {
	_now[token] = group;
}


/* has changes
   check if there are any changes recorded */
auto mx::highlight_tracker::has_changes(void) const noexcept -> bool {
	return _mark == &self::_mark_active_force;
}


#include "string_pool.hpp"


/* generate json
   generate JSON output for current changes */
auto mx::highlight_tracker::generate_json(void) -> std::string {

	// get string from pool
	auto json = mx::string_pool::query();

	json.append("{\"type\":\"animation\",\"highlights\":[");

	// process added highlights
	for (const auto& [token, group] : _now) {
		emit_highlight(
			json,
			_tokens->filtered_view(token),
			group
		);
	}
	// remove trailing comma
	json.back() = ']';
	json.append("}\r\n");

	return json;
}
