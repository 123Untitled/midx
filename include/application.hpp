#ifndef ml_application_hpp
#define ml_application_hpp

#include "monitoring/monitor.hpp"
#include "monitoring/project_watcher.hpp"
#include "monitoring/server.hpp"
#include "protocol.hpp"
//#include "data/project.hpp"
#include "player.hpp"
#include "language/analyzer.hpp"
#include "monitoring/signal.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {



	// -- A P P L I C A T I O N -----------------------------------------------

	class application final {


		private:


			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::application;


			// -- private members ---------------------------------------------

			bool _running;


			//ml::project _project;
			mx::player _player;

			ml::monitor _monitor;

			ml::signal _signal;

			//ml::project_watcher _watcher;

			ml::server _server;

			ml::protocol _protocol;

			ml::analyzer _analyzers[2U];

			mx::usz _active;



			// -- private lifecycle -------------------------------------------

			/* deleted default constructor */
			application(void) = delete;

			/* path constructor */
			application(const char*);


			// -- private methods ---------------------------------------------

			/* run */
			auto _run(void) -> void;


		public:

			// -- public accessors --------------------------------------------

			/* project */
			//auto project(void) noexcept -> ml::project& {
			//	return _project;
			//}
			//
			///* const project */
			//auto project(void) const noexcept -> const ml::project& {
			//	return _project;
			//}

			auto reparse(void) -> void;

			/* player */
			auto player(void) noexcept -> mx::player& {
				return _player;
			}

			/* const player */
			auto player(void) const noexcept -> const mx::player& {
				return _player;
			}

			/* monitor */
			auto monitor(void) noexcept -> ml::monitor& {
				return _monitor;
			}

			/* const monitor */
			auto monitor(void) const noexcept -> const ml::monitor& {
				return _monitor;
			}

			/* server */
			auto server(void) noexcept -> ml::server& {
				return _server;
			}

			/* const server */
			auto server(void) const noexcept -> const ml::server& {
				return _server;
			}

			/* protocol */
			auto protocol(void) noexcept -> ml::protocol& {
				return _protocol;
			}

			/* const protocol */
			auto protocol(void) const noexcept -> const ml::protocol& {
				return _protocol;
			}

			/* exit */
			auto exit(void) noexcept -> void {
				_running = false;
			}


			// -- public static methods ---------------------------------------

			static auto run(const char*) -> void;
	};
} // namespace ml

#endif // ml_application_hpp
