#ifndef ml_system_unix_descriptor_hpp
#define ml_system_unix_descriptor_hpp

#include "diagnostics/system_error.hpp"

#include <unistd.h>
#include <fcntl.h>


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- U N I X  D E S C R I P T O R ----------------------------------------

	class unix_descriptor {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::unix_descriptor;


			// -- private constants -------------------------------------------

			enum : int { INVALID_FD = -1 };


			// -- private members ---------------------------------------------

			/* file descriptor */
			int _fd;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			unix_descriptor(void) noexcept
			: _fd{INVALID_FD} {
			}

			/* descriptor constructor */
			explicit unix_descriptor(const int fd) noexcept
			: _fd{fd} {
			}

			/* deleted copy constructor */
			unix_descriptor(const self&) = delete;

			/* move constructor */
			unix_descriptor(self&& other) noexcept
			: _fd{other._fd} {
				other._fd = INVALID_FD;
			}

			/* destructor */
			~unix_descriptor(void) noexcept {

				// close file descriptor
				if (_fd != INVALID_FD)
					static_cast<void>(::close(_fd));
			}


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&& other) noexcept -> self& {

				// check for self assignment
				if (this == &other)
					return *this;

				// close file descriptor
				if (_fd != INVALID_FD)
					static_cast<void>(::close(_fd));

				// move file descriptor
					  _fd = other._fd;
				other._fd = INVALID_FD;

				// done
				return *this;
			}

			/* descriptor assignment operator */
			auto operator=(const int fd) noexcept -> self& {

				// close file descriptor
				if (_fd != INVALID_FD)
					static_cast<void>(::close(_fd));

				// assign new descriptor
				_fd = fd;

				// done
				return *this;
			}


			// -- public methods ----------------------------------------------

			/* close */
			auto close(void) -> void {

				// check if valid
				if (_fd == INVALID_FD)
					return;

				// close file descriptor
				if (::close(_fd) == -1)
					throw ml::system_error{"close"};

				// invalidate
				_fd = INVALID_FD;
			}


			// -- public conversion operators ---------------------------------

			/* int conversion operator */
			operator int(void) const noexcept {
				return _fd;
			}

			/* bool conversion operator */
			explicit operator bool(void) const noexcept {
				return _fd != INVALID_FD;
			}


			// -- public operators --------------------------------------------

			/* not operator */
			auto operator!(void) const noexcept -> bool {
				return _fd == INVALID_FD;
			}


			// -- public accessors --------------------------------------------

			/* fd */
			auto fd(void) const noexcept -> int {
				return _fd;
			}

			/* is open */
			auto is_open(void) const noexcept -> bool {
				return _fd != INVALID_FD;
			}

			/* is closed */
			auto is_closed(void) const noexcept -> bool {
				return _fd == INVALID_FD;
			}

	}; // class unix_descriptor


	template <typename... Tp>
	auto open(const char* path, const Tp&... args) -> ml::unix_descriptor {

		const int fd = ::open(path, args...);

		if (fd == -1)
			throw ml::system_error{"open"};

		return ml::unix_descriptor{fd};
	}


	template <typename... Tp>
	auto try_open(const char* path, const Tp&... args) -> ml::unix_descriptor {

		const int fd = ::open(path, args...);

		if (fd != -1)
			return ml::unix_descriptor{fd};

		if (errno == ENOENT)
			return ml::unix_descriptor{};

		throw ml::system_error{"open"};
	}


} // namespace ml

#endif // ml_system_unix_descriptor_hpp
