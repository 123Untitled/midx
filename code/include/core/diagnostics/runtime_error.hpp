#pragma once

#include "core/diagnostics/exception.hpp"
#include "core/string/inplace_string.hpp"


// -- M S  N A M E S P A C E --------------------------------------------------

namespace ms {


	// -- R U N T I M E  E R R O R --------------------------------------------

	class runtime_error final : public ms::exception {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ms::runtime_error;


			// -- private members ---------------------------------------------

			/* buffer */
			ms::inplace_wstring<4096U> _buffer;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			runtime_error(void) noexcept
			: _buffer{L"unknown error"} {
			}

			/* variadic constructor */
			template <typename... Ts> requires (sizeof...(Ts) > 0U)
			runtime_error(const Ts&... args) noexcept
			: _buffer{args...} {
			}

			/* copy constructor */
			runtime_error(const self&) noexcept = default;

			/* move constructor */
			runtime_error(self&&) noexcept = default;

			/* destructor */
			~runtime_error(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public overrides --------------------------------------------

			/* what */
			auto what(void) const noexcept -> const wchar_t* override final {
				return _buffer.data();
			}

			/* code */
			auto code(void) const noexcept -> int override final {
				return -1; // runtime_error does not have a specific error code
			}

	}; // class runtime_error

} // namespace ms
