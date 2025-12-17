#ifndef terminal_tty_hpp
#define terminal_tty_hpp

#include <termios.h>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- T T Y ---------------------------------------------------------------

	class tty final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::tty;


			// -- private members ---------------------------------------------

			/* terminal attributes */
			const struct ::termios _origin;
				  struct ::termios _raw;


			// -- private static methods --------------------------------------

			/* get */
			static auto _get(void) -> struct ::termios;

			/* set */
			static auto _set(const struct ::termios&) -> void;

			/* setup raw mode */
			static auto _setup_raw(struct ::termios&) noexcept -> void;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			tty(void);

			/* deleted copy constructor */
			tty(const self&) = delete;

			/* deleted move constructor */
			tty(self&&) = delete;

			/* destructor */
			~tty(void) noexcept;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- public methods ----------------------------------------------

			/* raw */
			auto blocking_raw(void) -> void;

			/* non-blocking */
			auto non_blocking_raw(void) -> void;

			/* restore */
			auto restore(void) -> void;

	}; // class tty

} // namespace mx

#endif // terminal_tty_hpp
