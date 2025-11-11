#ifndef player_hpp
#define player_hpp

#include "time/clock.hpp"
#include <vector>
#include "coremidi/eventlist.hpp"

#include "monitoring/server.hpp"

#include "system/concurrency/mutex.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- forward declarations ------------------------------------------------

	class model;


	// -- P L A Y E R ---------------------------------------------------------

	class player final : public ml::clockable {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::player;


			// -- private members ---------------------------------------------

			/* mutex */
			mx::mutex _mutex;

			/* model reference */
			mx::model* _model;

			ml::server* _server;

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



			auto model(mx::model& m) noexcept -> void;

			auto server(ml::server& s) noexcept -> void {
				_server = &s;
			}


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


			// -- public accessors --------------------------------------------

			/* is playing */
			auto is_playing(void) const noexcept -> bool {
				return _clock.is_running();
			}

		private:

			// -- private static methods --------------------------------------

			/* entrypoint */
			static auto _entrypoint(void*) -> void*;

	}; // class player

} // namespace mx

#endif // player_hpp
