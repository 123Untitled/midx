#include "language/ast/nodes/group.hpp"
#include "language/ast/play_ctx.hpp"
#include "language/ast/tree.hpp"


// -- G R O U P ---------------------------------------------------------------

// -- public methods ----------------------------------------------------------

/* play
   play group node */
auto as::group::play(as::play_ctx& ctx) const -> void {

	auto time = header.mod(ctx.fr.time * ctx.fr.speed);

		  auto it  = range.start;
    const auto end = range.end();

    for (; it < end; ++it) {
		const auto node = ctx.tree.remap_index(it);
        const auto& dur = ctx.tree.header(node).dur;

		if (time < dur) {
			ctx.push(node, (time / ctx.fr.speed), ctx.fr.speed);
			return;
		}
		time -= dur;
	}
}
