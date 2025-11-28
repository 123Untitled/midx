#include "language/ast/nodes/atomics.hpp"
#include "language/ast/play_ctx.hpp"
#include "language/ast/tree.hpp"


// -- A T O M I C S -----------------------------------------------------------

// -- public methods ----------------------------------------------------------

/* play
   play atomics node */
auto as::atomics::play(as::play_ctx& ctx) const -> void {

	const auto time = header.mod(ctx.fr.time * ctx.fr.speed);
	const auto step = header.dur.num != 1U ? time.num / time.den : 0U;
	const auto hash = ctx.hashes.hash_combine(ctx.fr.hash, step);

	bool edge = true;

	const auto it = ctx.hashes.find(hash);

	if (it != ctx.hashes.end())
		edge = time < it->second;

	ctx.hashes[hash] = time;

	auto& ev = ctx.events.back();
	const auto value = ctx.tree.value_at(value_start + step);


	if (param == pa::trig) {

		if (edge && value != 0)
			ev.trig();
	}
	else {
		if (param == pa::gate) {
		}
		ev.add(param, value);
	}

	const auto tv = ctx.tokens.filtered_view(token_start + step);
	highlight(ctx.hi, tv, "IncSearch");
}
