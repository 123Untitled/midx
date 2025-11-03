#ifndef system_socket_hpp
#define system_socket_hpp

#include "system/unix_descriptor.hpp"
#include <sys/socket.h>

//#include <netinet/in.h>
#include <sys/un.h>


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- A D D R E S S -------------------------------------------------------

	class address final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::address;


			// -- private members ---------------------------------------------

			/* address */
			::sockaddr_storage _addr;

			/* size */
			::socklen_t _size;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			constexpr address(void) noexcept
			: _addr{}, _size{sizeof(::sockaddr_storage)} {
			}

			/* local constructor */
			constexpr explicit address(const char* path) noexcept
			: _addr{}, _size{sizeof(::sockaddr_un)} {

				// interpret as local
				auto& addr = self::as_local();

				// set fields
				addr.sun_family = AF_UNIX;

				for (unsigned i = 0U; i < sizeof(addr.sun_path); ++i) {
					addr.sun_path[i] = path[i];
					if (path[i] == '\0')
						break;
				}
			}

			/* copy constructor */
			constexpr address(const self&) noexcept = default;

			/* move constructor */
			constexpr address(self&&) noexcept = default;

			/* destructor */
			constexpr ~address(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			constexpr auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			constexpr auto operator=(self&&) noexcept -> self& = default;


			// -- public accessors --------------------------------------------

			/* size */
			constexpr auto size(void) noexcept -> ::socklen_t& {
				return _size;
			}

			/* const size */
			constexpr auto size(void) const noexcept -> const ::socklen_t& {
				return _size;
			}

			/* as local */
			constexpr auto as_local(void) noexcept -> ::sockaddr_un& {
				return *static_cast<::sockaddr_un*>(static_cast<void*>(&_addr));
			}

			/* const as local */
			constexpr auto as_local(void) const noexcept -> const ::sockaddr_un& {
				return *static_cast<const ::sockaddr_un*>(static_cast<const void*>(&_addr));
			}

			/* as sockaddr */
			constexpr auto as_sockaddr(void) noexcept -> ::sockaddr& {
				return *static_cast<::sockaddr*>(static_cast<void*>(&_addr));
			}

			/* const as sockaddr */
			constexpr auto as_sockaddr(void) const noexcept -> const ::sockaddr& {
				return *static_cast<const ::sockaddr*>(static_cast<const void*>(&_addr));
			}


			// -- public modifiers --------------------------------------------

			/* clear */
			constexpr auto clear(void) noexcept -> void {
				_addr = {};
				_size = sizeof(::sockaddr_storage);
			}

	}; // class address


	// -- S O C K E T ---------------------------------------------------------

	class socket final : public ml::unix_descriptor {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::socket;


			// -- private lifecycle -------------------------------------------

			/* fd constructor */
			explicit socket(const int fd) noexcept
			: ml::unix_descriptor{fd} {
			}


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			socket(void) noexcept = default;

			/* socket constructor */
			socket(const int domain, const int type, const int protocol = 0)
			: ml::unix_descriptor{::socket(domain, type, protocol)} {

				if (_fd == -1)
					throw ml::system_error{"socket"};
			}

			/* deleted copy constructor */
			socket(const self&) = delete;

			/* move constructor */
			socket(self&&) noexcept = default;

			/* destructor */
			~socket(void) noexcept {

				// shutdown if open
				if (_fd != -1)
					static_cast<void>(::shutdown(_fd, SHUT_RDWR));
			}


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&& other) noexcept -> self& {

				if (this == &other)
					return *this;

				// shutdown if open
				if (_fd != -1)
					static_cast<void>(::shutdown(_fd, SHUT_RDWR));

				// move file descriptor
				ml::unix_descriptor::operator=(
						static_cast<ml::unix_descriptor&&>(other)
				);

				return *this;
			}


			// -- public methods ----------------------------------------------

			/* bind */
			auto bind(const ml::address& addr) -> void {
				const int res = ::bind(
					_fd,
					&addr.as_sockaddr(),
					addr.size()
				);

				if (res == -1)
					throw ml::system_error{"bind"};
			}

			/* listen */
			auto listen(const int backlog = SOMAXCONN) -> void {
				const int res = ::listen(_fd, backlog);

				if (res == -1)
					throw ml::system_error{"listen"};
			}

			/* accept */
			auto accept(void) -> ml::socket {

				const int nfd = ::accept(_fd, nullptr, nullptr);

				if (nfd == -1)
					throw ml::system_error{"accept"};

				return ml::socket{nfd};
			}

			/* non-blocking */
			auto non_blocking(void) -> void {

				int flags = ::fcntl(_fd, F_GETFL, 0);
				if (flags == -1)
					throw ml::system_error{"fcntl"};

				flags |= O_NONBLOCK;

				if (::fcntl(_fd, F_SETFL, flags) == -1)
					throw ml::system_error{"fcntl"};
			}

	}; // class socket

} // namespace ml

#endif // system_socket_hpp
