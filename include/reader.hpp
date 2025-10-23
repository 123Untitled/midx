#ifndef midilang_reader_hpp
#define midilang_reader_hpp

#include "types.hpp"
#include "diagnostics/system_error.hpp"
#include <unistd.h>


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- R E A D E R ---------------------------------------------------------

	template <ml::usz N>
	class reader final {


		public:

			// -- public types ------------------------------------------------

			/* size type */
			using size_type = decltype(N);


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::reader<N>;


			// -- private members ---------------------------------------------

			/* buffer */
			char _buffer[N];

			/* size */
			size_type _size;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			reader(void) noexcept
			: /* _buffer{} uninitialized */ _size{0U} {
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

			/* read */
			auto read(const int& descriptor) -> void {

				// read from file descriptor
				const auto status = ::read(descriptor, _buffer, N);

				// check for error
				if (status < 0) {

					_size = 0U; // need to differentiate between error and eof

					// skip non-fatal errors
					if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
						return;

					throw ml::system_error{"read"};
				}

				// cast size
				_size = static_cast<size_type>(status);
			}

			/* reset */
			auto reset(const int& descriptor) -> void {

				// read from file descriptor
				const auto status = ::lseek(descriptor, 0, SEEK_SET);

				// check for error
				if (status < 0)
					throw ml::system_error{"lseek"};

				// reset size
				_size = 0U;
			}


			// -- public accessors --------------------------------------------

			/* data */
			auto data(void) noexcept -> char (&)[N] {
				return _buffer;
			}

			/* const data */
			auto data(void) const noexcept -> const char (&)[N] {
				return _buffer;
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

} // namespace ml

#endif // midilang_reader_hpp
