#ifndef midilang_system_error_hpp
#define midilang_system_error_hpp

#include <exception>
#include <string.h>
#include <errno.h>


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- S Y S T E M  E R R O R ----------------------------------------------

	class system_error : public std::exception {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::system_error;


			// -- private members ---------------------------------------------

			/* what */
			char _what[1024U];


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			system_error(void) = delete;

			/* where constructor */
			system_error(const char* where)
			/* uninitialized */ {

				char* ptr = _what;
				char* end = (_what + sizeof(_what)) - 1U; // reserve space for null terminator

				if (where != nullptr) {
					const ::size_t wlen = ::strlen(where);

					// check space
					if ((ptr + (wlen + 2U)) < end) {
						::memcpy(ptr, where, wlen);
						ptr += wlen;
						ptr[0U] = ':';
						ptr[1U] = ' ';
						ptr += 2U;
					}
					else {
						// truncate
						::memcpy(ptr, where, end - ptr);
						ptr = end;
					}
				}

				const char* msg = ::strerror(errno);

				if (msg != nullptr) {
					const ::size_t mlen = ::strlen(msg);

					// check space
					if ((ptr + mlen) < end) {
						::memcpy(ptr, msg, mlen);
						ptr += mlen;
					}
					else {
						// truncate
						::memcpy(ptr, msg, end - ptr);
						ptr = end;
					}
				}

				// null terminate
				ptr[0U] = '\0';
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
				return _what;
			}

	}; // class system_error

} // namespace ml

#endif // midilang_system_error_hpp
