#ifndef protocol_header_hpp
#define protocol_header_hpp

#include "protocol/method.hpp"


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
			  _method{static_cast<pc::method>(_to_le_u32(static_cast<mx::u32>(method)))},
			  _length{_to_le_u32(length)} {
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

			static auto _to_le_u32(const mx::u32 value) noexcept -> mx::u32 {
				#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
					return __builtin_bswap32(value);
				#else
					return value;
				#endif
			}


		public:


			auto method(void) const noexcept -> pc::method {
				return static_cast<pc::method>(_to_le_u32(static_cast<mx::u32>(_method)));
			}

			auto length(void) const noexcept -> mx::u32 {
				return _to_le_u32(_length);
			}



	} __attribute__((packed)); // struct header



	//constexpr bool is_little = [](void) constexpr static noexcept -> bool {
	//	union data {
	//		unsigned value;
	//		unsigned char bytes[sizeof(unsigned)];
	//	};
	//	constexpr data test {1U};
	//
	//	return test.bytes[0U] == 1U;
	//}();



} // namespace pc

#endif // protocol_header_hpp
