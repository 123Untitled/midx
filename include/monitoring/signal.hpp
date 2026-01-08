#ifndef monitoring_signal_hpp
#define monitoring_signal_hpp

#include "monitoring/watcher.hpp"
#include "system/unix_pipe.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- forward declarations ------------------------------------------------

	/* monitor */
	class monitor;


	// -- S I G N A L ---------------------------------------------------------

	class signal final : public mx::watcher {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::signal;


			// -- private members ---------------------------------------------

			/* pipe */
			mx::unix_pipe _pipe;


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
			signal(const mx::monitor&);

			/* destructor */
			~signal(void) noexcept = default;



			// -- public overrides --------------------------------------------

			/* on event */
			auto on_event(mx::application&, const struct ::kevent&) -> void override final;

			/* ident */
			auto ident(void) const noexcept -> int override final;

	}; // class signal

} // namespace mx

#endif // monitoring_signal_hpp
