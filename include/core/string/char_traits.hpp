#pragma once

#include "core/types.hpp"
#include "core/type_traits/traits.hpp"
#include "core/string/is_string.hpp"

#include <cstring>
#include <cwchar>



// -- M X  N A M E S P A C E --------------------------------------------------

namespace ms {


	// -- C H A R  T R A I T S ------------------------------------------------

	template <typename C>
	struct char_traits {


		// -- assertions ------------------------------------------------------

		/* requires char type */
		static_assert(ms::is_character<C>,
					  "char_traits requires a character type (char, wchar_t, char8_t, char16_t, char32_t)");


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
			using self = ms::char_traits<C>;


			// -- private static methods --------------------------------------

			/* length */
			static auto _length(const_pointer str) noexcept -> size_type {
				if constexpr (ms::is_same<C, char>)
					return ::strlen(str);
				if constexpr (ms::is_same<C, wchar_t>)
					return ::wcslen(str);
				if constexpr (ms::is_same<C, char8_t>)
					return ::strlen(reinterpret_cast<const char*>(str));
				if constexpr (ms::is_same<C, char16_t>)
					return 0U; // TODO: implement
				if constexpr (ms::is_same<C, char32_t>)
					return 0U; // TODO: implement
			}

			/* copy */
			static auto _copy(pointer dst, const_pointer src, size_type count) noexcept -> void {
				if constexpr (ms::is_same<C, char>)
					static_cast<void>(::memcpy(dst, src, count));
				else if constexpr (ms::is_same<C, wchar_t>)
					static_cast<void>(::wmemcpy(dst, src, count));
				else if constexpr (ms::is_same<C, char8_t>)
					static_cast<void>(::memcpy(dst, src, count));
				else if constexpr (ms::is_same<C, char16_t>)
					static_cast<void>(::memcpy(dst, src, count * sizeof(char16_t)));
				else if constexpr (ms::is_same<C, char32_t>)
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



	namespace impl {

		template <typename, bool, bool, bool, bool>
		struct char_type_of;

		/* character specialization */
		template <typename T>
		struct char_type_of<T, true, false, false, false> {
			using type = T;
			non_instantiable(char_type_of);
		};

		/* array specialization */
		template <typename T>
		struct char_type_of<T, false, true, false, false> {
			using type = ms::remove_cve<T>;
			non_instantiable(char_type_of);
		};

		/* pointer specialization */
		template <typename T>
		struct char_type_of<T, false, false, true, false> {
			using type = ms::remove_cvp<T>;
			non_instantiable(char_type_of);
		};

		/* string specialization */
		template <typename T>
		struct char_type_of<T, false, false, false, true> {
			using type = typename T::char_type;
			non_instantiable(char_type_of);
		};

	} // namespace impl

	/* char type of */
	template <typename T>
	using char_type_of = typename ms::impl::char_type_of<T,
		  ms::is_character<T>,
		  ms::is_array<T>,
		  ms::is_pointer<T>,
		  ms::is_string_like<T>>::type;


	/* cstr of */
	template <typename T>
	constexpr auto cstr_of(const T& str)
		noexcept -> typename ms::char_traits<char_type_of<T>>::const_pointer {
		if constexpr (ms::is_string_like<T>) {
			return str.cstr();
		}
		else if constexpr (ms::is_array<T> || ms::is_pointer<T>) {
			return str;
		}
	}





	// -- I S  C H A R  T R A I T S -------------------------------------------

	namespace impl {


		/* is char traits false */
		template <typename>
		struct is_char_traits final {
			static constexpr bool value = false;
			non_instantiable(is_char_traits);
		};

		/* is char traits true */
		template <typename C>
		struct is_char_traits<ms::char_traits<C>> final {
			static constexpr bool value = true;
			non_instantiable(is_char_traits);
		};

		/* is char traits of */
		template <typename, typename>
		struct is_char_traits_of final {
			static constexpr bool value = false;
			non_instantiable(is_char_traits_of);
		};

		/* is char traits of true */
		template <typename C>
		struct is_char_traits_of<ms::char_traits<C>, C> final {
			static constexpr bool value = true;
			non_instantiable(is_char_traits_of);
		};

	} // namespace impl


	/* is char traits */
	template <typename T>
	concept is_char_traits = ms::impl::is_char_traits<T>::value;

	/* is char traits of */
	template <typename T, typename C>
	concept is_char_traits_of = ms::impl::is_char_traits_of<T, C>::value;

} // namespace ms
