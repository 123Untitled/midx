#ifndef protocol_header_hpp
#define protocol_header_hpp

#include "protocol/method.hpp"
#include "core/endian.hpp"



namespace mx {


	template <typename T>
	constexpr auto bswap(const T value) noexcept -> T {

		// assertions
		static_assert(mx::is_integral<T>, "bswap requires an integral type");
		static_assert(sizeof(T) <= 8U, "bswap supports only 1, 2, 4, or 8 byte integral types");

		// 8-bit swap (no-op)
		if constexpr (sizeof(T) == 1U)
			return value;

		// 16-bit swap
		if constexpr (sizeof(T) == 2U)
			return static_cast<T>(__builtin_bswap16(static_cast<mx::u16>(value)));

		// 32-bit swap
		if constexpr (sizeof(T) == 4U)
			return static_cast<T>(__builtin_bswap32(static_cast<mx::u32>(value)));

		// 64-bit swap
		if constexpr (sizeof(T) == 8U)
			return static_cast<T>(__builtin_bswap64(static_cast<mx::u64>(value)));
	}

} // namespace mx



// -- P C  N A M E S P A C E --------------------------------------------------

namespace pc {




	// -- H E A D E R ---------------------------------------------------------

	struct header {


		private:

			// -- public members ----------------------------------------------

			mx::u8 _magic[4U];
			mx::u32 _reserved;
			pc::method _method;
			mx::u32 _length;


		public:

			// -- public lifecycle --------------------------------------------

			header(void) noexcept
			: _magic{'M', 'I', 'D', 'X'},
			  _reserved{0U},
			  _method{pc::method::unknown},
			  _length{0U} {
			}

			header(const pc::method method,
				   const mx::u32 length) noexcept
			: _magic{'M', 'I', 'D', 'X'},
			  _reserved{0U},
			  _method{_bswap(method)},
			  _length{_bswap(length)} {
			}



			// -- public methods ----------------------------------------------

			auto is_valid(void) const noexcept -> bool {
				return (_magic[0U] == 'M')
					&& (_magic[1U] == 'I')
					&& (_magic[2U] == 'D')
					&& (_magic[3U] == 'X')
					&& (method() < pc::method::max_types);
			}


		private:

			template <typename T>
			static auto _bswap(const T value) noexcept -> T {
				if constexpr (mx::endian::native == mx::endian::big)
					return mx::bswap(value);
				else
					return value;
			}


		public:


			auto method(void) const noexcept -> pc::method {
				return _bswap(_method);
			}

			auto length(void) const noexcept -> mx::u32 {
				return _bswap(_length);
			}


	} __attribute__((packed)); // struct header


} // namespace pc

#endif // protocol_header_hpp
