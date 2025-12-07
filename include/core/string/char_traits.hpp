#ifndef core_string_char_traits_hpp
#define core_string_char_traits_hpp

#include "core/types.hpp"
#include "core/type_traits/traits.hpp"
#include "core/string/is_string.hpp"

#include <cstring>
#include <cwchar>



// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	constexpr auto strlen(const char* str) noexcept -> mx::usz {
		if consteval {
			mx::usz len = 0U;
			while (str[len] != '\0')
				++len;
			return len;
		}
		else {
			return ::strlen(str);
		}
	}


	// -- C H A R  T R A I T S ------------------------------------------------

	template <typename C>
	struct char_traits {


		public:

			// -- public types ------------------------------------------------

			/* char type */
			using char_type       = C;

			/* reference type */
			using reference       = char_type&;

			/* const reference type */
			using const_reference = const char_type&;

			/* pointer type */
			using pointer         = char_type*;

			/* const pointer type */
			using const_pointer   = const char_type*;

			/* size type */
			using size_type       = mx::usz;


			// -- public lifecycle --------------------------------------------

			/* default class */
			noexcept_default_class(char_traits);


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::char_traits<C>;


			// -- private static methods --------------------------------------

			/* length */
			static auto _length(const_pointer str) noexcept -> size_type {
				if constexpr (mx::is_same<C, char>)
					return ::strlen(str);
				if constexpr (mx::is_same<C, wchar_t>)
					return ::wcslen(str);
				if constexpr (mx::is_same<C, char8_t>)
					return ::strlen(reinterpret_cast<const char*>(str));
				if constexpr (mx::is_same<C, char16_t>)
					return 0U; // TODO: implement
				if constexpr (mx::is_same<C, char32_t>)
					return 0U; // TODO: implement
			}

			/* copy */
			static auto _copy(pointer dst, const_pointer src, size_type count) noexcept -> void {
				if constexpr (mx::is_same<C, char>)
					static_cast<void>(::memcpy(dst, src, count));
				else if constexpr (mx::is_same<C, wchar_t>)
					static_cast<void>(::wmemcpy(dst, src, count));
				else if constexpr (mx::is_same<C, char8_t>)
					static_cast<void>(::memcpy(dst, src, count));
				else if constexpr (mx::is_same<C, char16_t>)
					static_cast<void>(::memcpy(dst, src, count * sizeof(char16_t)));
				else if constexpr (mx::is_same<C, char32_t>)
					static_cast<void>(::memcpy(dst, src, count * sizeof(char32_t)));
			}


			// -- private static members --------------------------------------

			/* empty array */
			static constexpr char_type _empty[1U] {
				static_cast<char_type>(0x00)
			};


		public:

			// -- public constants --------------------------------------------

			/* empty */
			static constexpr const_pointer empty = _empty;

			/* null character */
			static constexpr char_type null = *_empty;


			// -- public static methods ---------------------------------------

			/* length */
			static constexpr auto length(const_pointer str) noexcept -> size_type {
				if !consteval {
					return self::_length(str);
				}
				else {
					size_type i = 0U;
					while (str[i] != null)
						++i;
					return i;
				}
			}

			/* copy */
			static constexpr auto copy(pointer dst, const_pointer src, size_type count) noexcept -> void {
				if !consteval {
					self::_copy(dst, src, count);
				}
				else {
					for (size_type i = 0U; i < count; ++i)
						dst[i] = src[i];
				}
			}

			/* copy safe */
			static constexpr auto copy_safe(pointer dst,
											pointer end,
											const_pointer src,
											size_type count) noexcept -> size_type {
				const auto left = static_cast<size_type>(end - dst);
				const auto size = count < left ? count : left;
				self::copy(dst, src, size);
				return size;
			}


	}; // struct char_traits

} // namespace mx

#endif // core_string_char_traits_hpp
