#ifndef ml_application_hpp
#define ml_application_hpp

#include "monitoring/monitor.hpp"
#include "monitoring/project_watcher.hpp"
#include "data/project.hpp"
#include "player.hpp"


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

			ml::project _project;
			ml::player _player;

			ml::monitor _monitor;

			ml::project_watcher _watcher;



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
			auto project(void) noexcept -> ml::project& {
				return _project;
			}

			/* const project */
			auto project(void) const noexcept -> const ml::project& {
				return _project;
			}

			/* player */
			auto player(void) noexcept -> ml::player& {
				return _player;
			}

			/* const player */
			auto player(void) const noexcept -> const ml::player& {
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

			/* exit */
			auto exit(void) noexcept -> void {
				_running = false;
			}


			// -- public static methods ---------------------------------------

			static auto run(const char*) -> void;
	};
} // namespace ml

#endif // ml_application_hpp
