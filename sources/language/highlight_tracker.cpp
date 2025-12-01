#include "language/highlight_tracker.hpp"
#include <algorithm>


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
auto mx::highlight_tracker::mark_active(const mx::usz token_index,
										const char* group,
										const mx::frac& expire) -> void {

	if (_tokens == nullptr)
		return;

	// check if token already exists
	auto it = _map.find(token_index);

	if (it != _map.end()) {

		const auto& hi = *it->second;

		// token exists, update if new expire is longer
		if (expire > hi.expire) {
			// remove old entry
			_set.erase(hi);

			//std::cout << "\x1b[33mUpdating\x1b[0m: " << token_index << " to expire at " << expire << "\n";

			// insert updated entry
			auto inserted = _set.emplace(expire, token_index);
			it->second = &(*inserted.first);
		}
		else {
			//std::cout << "\x1b[34mIgnoring\x1b[0m: " << token_index << " with expire at " << expire << "\n";
		}
		return;
	}

	//std::cout << "\x1b[32mAdding\x1b[0m: " << token_index << " to expire at " << expire << "\n";
	// new token
	const auto inserted = _set.emplace(expire, token_index);
	_map[token_index] = &(*inserted.first);

	_added.push_back({token_index, group});
}

/* has changes
   check if there are any changes recorded */
auto mx::highlight_tracker::has_changes(void) const noexcept -> bool {
	return _removed.empty() == false
		  || _added.empty() == false;
}

auto mx::highlight_tracker::update(const mx::frac& time) -> void {

	// loop over timeouts
	for (auto it = _set.begin(); it != _set.end(); ) {

		if (it->expire > time)
			break;

		_removed.push_back(it->token_index);

		// remove from map
		_map.erase(it->token_index);
		it = _set.erase(it);
	}
}

#include "string_pool.hpp"

/* generate json
   generate JSON output for current changes */
auto mx::highlight_tracker::generate_json(void) -> std::string {

	// get string from pool
	auto json = mx::string_pool::query();

	json.append("{\"type\":\"animation\",");

	// added
	if (_added.empty() == false) {

		json.append("\"added\":[");
		for (const auto& a : _added) {
			const auto tv = _tokens->filtered_view(a.token_index);
			emit_highlight(json, tv, a.group);
		}
		json.back() = ']'; // replace last comma
	}

	// removed
	if (_removed.empty() == false) {
		if (_added.empty() == false) {
			json.append(",");
		}

		json.append("\"removed\":[");
		for (const auto& r : _removed) {
			const auto tv = _tokens->filtered_view(r);
			emit_highlight(json, tv, "Removed");
		}
		json.back() = ']'; // replace last comma
	}

	json.append("}\r\n");

	// clear changes
	_added.clear();
	_removed.clear();

	return json;
}
