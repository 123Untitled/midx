#ifndef player_hpp
#define player_hpp

#include "time/clock.hpp"
#include "monitoring/watcher.hpp"
#include "midi/midi_engine.hpp"


namespace as { class tree; }


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- forward declarations ------------------------------------------------

	class monitor;


	// -- P L A Y E R ---------------------------------------------------------

	class player final : public mx::watcher {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::player;


			// -- private members ---------------------------------------------

			/* clock */
			mx::clock _clock;

			/* tree reference */
			const as::tree* _tree;

			/* midi engine */
			mx::midi_engine _engine;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			player(const mx::monitor&);

			/* destructor */
			~player(void) noexcept = default;


			// -- public methods ----------------------------------------------

			/* start */
			auto start(void) -> void;

			/* stop */
			auto stop(void) -> void;

			/* switch tree */
			auto switch_tree(const as::tree&) noexcept -> void;


			// -- public overrides --------------------------------------------

			/* on event */
			auto on_event(mx::application&, const struct ::kevent&) -> void override final;

			/* ident */
			auto ident(void) const noexcept -> int override final;

	}; // class player

} // namespace mx

#endif // player_hpp
