#ifndef system_error_hpp
#define system_error_hpp

#include "core/string/inplace_string.hpp"
#include <exception>
#include <string.h>
#include <errno.h>


// -- M L  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- S Y S T E M  E R R O R ----------------------------------------------

	class system_error : public std::exception {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::system_error;


			// -- private members ---------------------------------------------

			/* message */
			mx::inplace_string<1024U> _msg;


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			system_error(void) = delete;

			/* where constructor */
			template <unsigned N>
			system_error(const char (&where)[N]) noexcept
			: _msg{} {

				const char* what;

				if (errno == 0)
					what = "No error";
				else
					what = ::strerror(errno);

				_msg.assign(where, ": ", what != nullptr ? what : "Unknown error");
			}

			/* deleted copy constructor */
			system_error(const self&) = delete;

			/* deleted move constructor */
			system_error(self&&) = delete;

			/* destructor */
			~system_error(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- public overrides --------------------------------------------

			/* what */
			auto what(void) const noexcept -> const char* override {
				return _msg.data();
			}

	}; // class system_error

} // namespace mx

#endif // system_error_hpp
