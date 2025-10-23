#ifndef midilang_player_hpp
#define midilang_player_hpp

#include "time/clock.hpp"
#include <vector>
#include "coremidi/eventlist.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- forward declarations ------------------------------------------------

	/* application */
	class application;


	// -- P L A Y E R ---------------------------------------------------------

	class player final : public ml::clockable {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::player;


			// -- private members ---------------------------------------------

			/* clock */
			ml::clock _clock;

			/* midi sources */
			std::vector<cm::source> _srcs;

			/* event list */
			cm::eventlist _evs;

			/* thread */
			::pthread_t _thread;

			bool _running;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			player(void);

			/* destructor */
			~player(void) noexcept;


			// -- public methods ----------------------------------------------

			/* start */
			auto start(void) -> void;

			/* stop */
			auto stop(void) noexcept -> void;


			// -- public overrides --------------------------------------------

			/* start */
			auto clock_start(void) -> void override;

			/* tick */
			auto clock_tick(const ml::u64&) -> void override;

			/* stop */
			auto clock_stop(void) -> void override;


		private:

			// -- private static methods --------------------------------------

			/* entrypoint */
			static auto _entrypoint(void*) -> void*;

	}; // class player

} // namespace ml

#endif // midilang_player_hpp
