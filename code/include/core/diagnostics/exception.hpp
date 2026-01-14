#pragma once

//#include "windows/types.hpp"
#include "core/types.hpp"
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <iostream>


// -- M S  N A M E S P A C E --------------------------------------------------

namespace ms {


	inline auto get_errno(void) noexcept -> int {
		return errno;
	}


	// -- E X C E P T I O N ---------------------------------------------------

	class exception {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ms::exception;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			exception(void) noexcept = default;

			/* copy constructor */
			exception(const self&) noexcept = default;

			/* move constructor */
			exception(self&&) noexcept = default;

			/* destructor */
			virtual ~exception(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public interface --------------------------------------------

			/* what */
			virtual auto what(void) const noexcept -> const wchar_t* = 0;

			/* code */
			virtual auto code(void) const noexcept -> int = 0;

	}; // class exception



	// -- S Y S T E M  E R R O R ----------------------------------------------

	class errno_exception final : public ms::exception {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ms::errno_exception;


			// -- private members ---------------------------------------------

			/* buffer */
			char _buffer[4096U];

			/* errno */
			int _errno;



		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			errno_exception(void) noexcept;

			/* where constructor */
			template <unsigned N>
			errno_exception(const char (&where)[N]) noexcept
			: _errno{get_errno()} /* _buffer{} uninitialized */ {

				// buffer size
				constexpr auto buffer_size = sizeof(_buffer);

				char buffer[1024U];
				// get errno string error
				auto ret = ::strerror(buffer, _errno);
				if (ret != 0) {
					// handle error
				}
				const char* err_str = buffer;

				// check error
				if (err_str == nullptr)
					err_str = "unknown error";

				// where length
				size_t w_len = N - (where[N - 1U] == '\0' ? 1U : 0U);

				// error length
				size_t err_len = ::strlen(err_str);

				// check out of bounds
				if (err_len + w_len + 3U > buffer_size)
					err_len = buffer_size - w_len - 3U;

				// offset pointer
				char* ptr = _buffer;

				// copy where
				::memcpy(ptr, where, w_len);
				ptr += w_len;

				// copy separator
				::memcpy(ptr, ": ", 2);
				ptr += 2;

				// copy error
				::memcpy(ptr, err_str, err_len);
				ptr += err_len;

				// null terminate
				*ptr = '\0';
			}

			/* copy constructor */
			errno_exception(const self&) noexcept = default;

			/* move constructor */
			errno_exception(self&&) noexcept = default;

			/* destructor */
			~errno_exception(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public overrides --------------------------------------------

			/* what */
			auto what(void) const noexcept -> const wchar_t* override final {

				// N EE D TO C O N V E R T !!!!!!!!!!!!!!!!!!!!!!!
				return (const wchar_t*)_buffer;
			}


			// -- public accessors --------------------------------------------

			/* code */
			auto code(void) const noexcept -> int override final {
				return _errno;
			}

	}; // class errno_exception





} // namespace ms
