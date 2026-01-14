#ifndef unix_pipe_hpp
#define unix_pipe_hpp

#include "diagnostics/system_error.hpp"
#include <unistd.h>
#include <fcntl.h>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- U N I X  P I P E ----------------------------------------------------

	class unix_pipe final {


		private:
			
			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::unix_pipe;


			// -- private constants -------------------------------------------

			enum : unsigned {
				RD, WR
			};


			// -- private members ---------------------------------------------

			/* read end */
			int _rd;

			/* write end */
			int _wr;


			// -- private lifecycle -------------------------------------------

			/* pipe constructor */
			unix_pipe(const int (&fds)[2U]) noexcept
			: _rd{fds[RD]}, _wr{fds[WR]} {
			}


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			unix_pipe(void) noexcept
			: _rd{-1}, _wr{-1} {
			}

			/* deleted copy constructor */
			unix_pipe(const self&) = delete;

			/* move constructor */
			unix_pipe(self&& other) noexcept
			: _rd{other._rd},
			  _wr{other._wr} {

				other._rd = -1;
				other._wr = -1;
			}

			/* destructor */
			~unix_pipe(void) noexcept {

				if (_rd != -1)
					static_cast<void>(::close(_rd));
				if (_wr != -1)
					static_cast<void>(::close(_wr));
			}


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&& other) noexcept -> self& {

				if (this == &other)
					return *this;

				if (_rd != -1)
					static_cast<void>(::close(_rd));
				if (_wr != -1)
					static_cast<void>(::close(_wr));

				_rd = other._rd;
				_wr = other._wr;

				other._rd = -1;
				other._wr = -1;

				return *this;
			}


			// -- public accessors --------------------------------------------

			/* read end */
			auto rd(void) const noexcept -> int {
				return _rd;
			}

			/* write end */
			auto wr(void) const noexcept -> int {
				return _wr;
			}


			// -- public methods ----------------------------------------------

			/* non blocking write end */
			auto non_blocking_wr(void) const -> void {
				const int flags = ::fcntl(_wr, F_GETFL, 0);

				if (flags == -1)
					throw mx::system_error{"fcntl"};

				if (::fcntl(_wr, F_SETFL, flags | O_NONBLOCK) == -1)
					throw mx::system_error{"fcntl"};
			}

			/* close write end */
			auto close_wr(void) noexcept -> void {
				if (_wr == -1)
					return;
				static_cast<void>(::close(_wr));
				_wr = -1;
			}

			/* close read end */
			auto close_rd(void) noexcept -> void {
				if (_rd == -1)
					return;
				static_cast<void>(::close(_rd));
				_rd = -1;
			}


		// -- friends ---------------------------------------------------------

		/* pipe as friend */
		friend auto pipe(void) -> mx::unix_pipe;

	}; // class pipe



	// -- non-member functions ------------------------------------------------

	/* pipe */
	inline auto pipe(void) -> mx::unix_pipe {

		int fds[2U];

		if (::pipe(fds) != 0)
			throw mx::system_error{"pipe"};

		return mx::unix_pipe{fds};
	}

} // namespace mx

#endif // unix_pipe_hpp
