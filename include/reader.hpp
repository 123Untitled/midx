#ifndef midilang_reader_hpp
#define midilang_reader_hpp

#include "types.hpp"
#include "diagnostics/system_error.hpp"
#include <unistd.h>
#include <sys/socket.h>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- R E A D E R ---------------------------------------------------------

	template <mx::usz N>
	class reader final {


		public:

			// -- public types ------------------------------------------------

			/* size type */
			using size_type = decltype(N);


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::reader<N>;


			// -- private members ---------------------------------------------

			/* buffer */
			mx::u8 _buffer[N];

			/* size */
			size_type _size;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			reader(void) noexcept
			: /* uninitialized */ _size{0U} {
			}

			/* copy constructor */
			reader(const self&) noexcept = default;

			/* move constructor */
			reader(self&&) noexcept = default;

			/* destructor */
			~reader(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public modifiers --------------------------------------------

			/* recv */
			auto recv(const int& socket, const int flags = 0) -> bool {

				// read from socket
				const auto status = ::recv(socket, _buffer, N, flags);

				// check for error
				if (status < 0) {

					_size = 0U; // need to differentiate between error and eof

					// skip non-fatal errors
					if (errno == EAGAIN
					 || errno == EWOULDBLOCK
					 || errno == EINTR)
						return false;

					// throw exception
					throw mx::system_error{"recv"};
				}

				// cast size
				_size = static_cast<size_type>(status);

				return true;
			}


			// -- public accessors --------------------------------------------

			/* const data */
			auto data(void) const noexcept -> const mx::u8 (&)[N] {
				return _buffer;
			}

			/* begin */
			auto begin(void) const noexcept -> const mx::u8* {
				return _buffer;
			}

			/* end */
			auto end(void) const noexcept -> const mx::u8* {
				return _buffer + _size;
			}

			/* size */
			auto size(void) const noexcept -> size_type {
				return _size;
			}

			/* capacity */
			constexpr auto capacity(void) const noexcept -> size_type {
				return N;
			}

			/* empty */
			auto empty(void) const noexcept -> bool {
				return _size == 0U;
			}

			/* eof */
			auto eof(void) const noexcept -> bool {
				return _size == 0U;
			}

	}; // class reader

} // namespace mx

#endif // midilang_reader_hpp
