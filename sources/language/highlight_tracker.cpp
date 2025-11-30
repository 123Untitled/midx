#include "language/highlight_tracker.hpp"
#include <sstream>


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

/* end frame
   finalize frame and generate differential JSON
   returns true if changes occurred */
auto mx::highlight_tracker::end_frame(std::string& out) -> bool {

	if (_tokens == nullptr)
		return false;

	bool changed = false;
	bool has_added = false;
	bool has_removed = false;

	std::string added;
	std::string removed;

	// find removed highlights (in _active but not in _pending)
	for (const auto& [hash, entry] : _active) {
		if (_pending.find(hash) == _pending.end()) {
			// emit removal JSON
			const auto tv = _tokens->filtered_view(entry.token_idx);
			emit_highlight(removed, tv, entry.group);
			has_removed = true;
			changed = true;
		}
	}

	// find added highlights (in _pending but not in _active)
	for (const auto& [hash, entry] : _pending) {
		if (_active.find(hash) == _active.end()) {
			// emit addition JSON
			const auto tv = _tokens->filtered_view(entry.token_idx);
			emit_highlight(added, tv, entry.group);
			has_added = true;
			changed = true;
		}
	}

	// build differential JSON if changes occurred
	if (changed) {
		// remove trailing commas
		if (has_added && !added.empty() && added.back() == ',')
			added.pop_back();
		if (has_removed && !removed.empty() && removed.back() == ',')
			removed.pop_back();

		// build JSON structure
		out.append("\"added\":[");
		out.append(added);
		out.append("],\"removed\":[");
		out.append(removed);
		out.append("]");
	}

	// swap maps for next frame (efficient, avoids reallocation)
	std::swap(_active, _pending);

	return changed;
}
