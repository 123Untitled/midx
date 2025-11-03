#ifndef monitoring_signal_hpp
#define monitoring_signal_hpp

#include "monitoring/watcher.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- forward declarations ------------------------------------------------

	/* monitor */
	class monitor;


	// -- S I G N A L ---------------------------------------------------------

	class signal final : public ml::watcher {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::signal;


			// -- private members ---------------------------------------------

			/* pipe */
			int _fds[2U];


			// -- private static members --------------------------------------

			/* write end */
			static int _wr;


			// -- private static methods --------------------------------------

			/* signal handler */
			static auto _handler(const int) noexcept -> void;


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			signal(void) = delete;

			/* monitor constructor */
			signal(const ml::monitor&);



			// -- public overrides --------------------------------------------

			/* on event */
			auto on_event(ml::application&, const struct ::kevent&) -> void override final;

			/* make add */
			auto make_add(void) noexcept -> struct ::kevent override final;

			/* make del */
			auto make_del(void) const noexcept -> struct ::kevent override final;

	}; // class signal


} // namespace ml

#endif // monitoring_signal_hpp
