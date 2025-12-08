#ifndef ml_application_hpp
#define ml_application_hpp

#include "monitoring/monitor.hpp"
#include "monitoring/server.hpp"
#include "protocol.hpp"
#include "player.hpp"
#include "language/analyzer.hpp"
#include "monitoring/signal.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace mx {



	// -- A P P L I C A T I O N -----------------------------------------------

	class application final {


		private:


			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::application;


			// -- private members ---------------------------------------------

			bool _running;

			mx::monitor _monitor;

			mx::signal _signal;

			mx::server _server;


			mx::protocol _protocol;

			mx::analyzer _analyzers[2U];

			mx::usz _active;

			mx::player _player;



			// -- private lifecycle -------------------------------------------

			/* default constructor */
			application(void);


			// -- private methods ---------------------------------------------

			/* run */
			auto _run(void) -> void;


		public:

			// -- public accessors --------------------------------------------

			/* re-parse */
			auto reparse(mx::string&&) -> void;

			/* exit */
			auto exit(void) noexcept -> void;

			/* toggle */
			auto toggle(void) -> void;


			// -- public accessors --------------------------------------------

			/* player */
			auto player(void) noexcept -> mx::player& {
				return _player;
			}

			/* const player */
			auto player(void) const noexcept -> const mx::player& {
				return _player;
			}

			/* monitor */
			auto monitor(void) noexcept -> mx::monitor& {
				return _monitor;
			}

			/* const monitor */
			auto monitor(void) const noexcept -> const mx::monitor& {
				return _monitor;
			}

			/* server */
			auto server(void) noexcept -> mx::server& {
				return _server;
			}

			/* const server */
			auto server(void) const noexcept -> const mx::server& {
				return _server;
			}

			/* protocol */
			auto protocol(void) noexcept -> mx::protocol& {
				return _protocol;
			}

			/* const protocol */
			auto protocol(void) const noexcept -> const mx::protocol& {
				return _protocol;
			}


			// -- public static methods ---------------------------------------

			static auto run(void) -> void;
	};
} // namespace mx

#endif // ml_application_hpp
