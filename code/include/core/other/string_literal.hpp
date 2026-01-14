#pragma once

#include "core/types.hpp"
#include "core/type_traits/index_sequence.hpp"


// -- V E  N A M E S P A C E --------------------------------------------------

namespace ve {


	// -- S T R I N G  L I T E R A L ------------------------------------------

	template <ms::u32 ___size>
	struct literal final {


		// -- assertions ------------------------------------------------------

		/* check for null size */
		static_assert(___size > 0U,
			"literal: empty string literals are not allowed");


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = ve::literal<___size>;


		public:

			// -- public types ------------------------------------------------

			/* size type */
			using size_type = decltype(___size);


			// -- public members ----------------------------------------------

			/* data */
			char data[___size];


			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			constexpr literal(void) noexcept = default;

			/* array constructor */
			template <size_type ___sz>
			constexpr literal(const char (&___str)[___sz]) noexcept
			//: literal{___str, ms::make_index_sequence<___sz>()} {
			{
				for (size_type i = 0U; i < ___sz && i < ___size; ++i) {
					data[i] = ___str[i];
				}
			}


		private:

			/* sequence constructor */
			//template <size_type... indices>
			//constexpr literal(const char (&___str)[___size], ve::index_sequence<indices...>) noexcept
			//: data{___str[indices]...} {
			//}


		public:

			/* copy constructor */
			constexpr literal(const ___self&) noexcept = default;

			/* move constructor */
			constexpr literal(___self&&) noexcept = default;

			/* destructor */
			constexpr ~literal(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			constexpr auto operator=(const ___self&) noexcept -> ___self& = default;

			/* move assignment operator */
			constexpr auto operator=(___self&&) noexcept -> ___self& = default;


			// -- public accessors --------------------------------------------

			/* length */
			constexpr auto length(void) const noexcept -> size_type {
				size_type size = 0U;
				for (size_type i = 0U; i < ___size; ++i) {
					if (data[i] == '\0')
						break;
					++size;
				}
				return size;
			}


			// -- public comparison operators ---------------------------------

			/* equality operator */
			template <size_type ___sz>
			constexpr auto operator==(const ve::literal<___sz>& other) const noexcept -> bool {
				return ___self::_compare(data, other.data) == 0;
			}

			/* inequality operator */
			template <size_type ___sz>
			constexpr auto operator!=(const ve::literal<___sz>& other) const noexcept -> bool {
				return ___self::_compare(data, other.data) != 0;
			}

			/* less than operator */
			template <size_type ___sz>
			constexpr auto operator<(const ve::literal<___sz>& other) const noexcept -> bool {
				return ___self::_compare(data, other.data) < 0;
			}

			/* greater than operator */
			template <size_type ___sz>
			constexpr auto operator>(const ve::literal<___sz>& other) const noexcept -> bool {
				return ___self::_compare(data, other.data) > 0;
			}

			/* less than or equal to operator */
			template <size_type ___sz>
			constexpr auto operator<=(const ve::literal<___sz>& other) const noexcept -> bool {
				return ___self::_compare(data, other.data) <= 0;
			}

			/* greater than or equal to operator */
			template <size_type ___sz>
			constexpr auto operator>=(const ve::literal<___sz>& other) const noexcept -> bool {
				return ___self::_compare(data, other.data) >= 0;
			}


			/* equality operator */
			template <size_type ___sz>
			constexpr auto operator==(const char (&other)[___sz]) const noexcept -> bool {
				return ___self::_compare(data, other) == 0;
			}

			/* inequality operator */
			template <size_type ___sz>
			constexpr auto operator!=(const char (&other)[___sz]) const noexcept -> bool {
				return ___self::_compare(data, other) != 0;
			}

			/* less than operator */
			template <size_type ___sz>
			constexpr auto operator<(const char (&other)[___sz]) const noexcept -> bool {
				return ___self::_compare(data, other) < 0;
			}

			/* greater than operator */
			template <size_type ___sz>
			constexpr auto operator>(const char (&other)[___sz]) const noexcept -> bool {
				return ___self::_compare(data, other) > 0;
			}

			/* less than or equal to operator */
			template <size_type ___sz>
			constexpr auto operator<=(const char (&other)[___sz]) const noexcept -> bool {
				return ___self::_compare(data, other) <= 0;
			}

			/* greater than or equal to operator */
			template <size_type ___sz>
			constexpr auto operator>=(const char (&other)[___sz]) const noexcept -> bool {
				return ___self::_compare(data, other) >= 0;
			}


		private:

			/* compare implementation */
			template <size_type size, size_type index>
			static constexpr auto _compare_impl(const char (&lhs)[size], const char (&rhs)[size]) noexcept -> signed int {

				if constexpr (index == size) {
					return 0;
				} else {
					return (lhs[index] == rhs[index])
						? (___self::_compare_impl<size, index + 1U>(lhs, rhs))
						: (lhs[index] > rhs[index] ? +1 : -1);
				}
			}

			/* compare */
			template <size_type lsize, size_type rsize>
			static constexpr auto _compare(const char (&lhs)[lsize], const char (&rhs)[rsize]) noexcept -> signed int {

				// check for unequal sizes
				if constexpr (lsize != rsize) {
					return (lsize > rsize) ? +1 : -1;
				}
				else {
					// return comparison
					return ___self::_compare_impl<lsize, 0U>(lhs, rhs);
				}
			}
	};


	// -- deduction guides ----------------------------------------------------

	template <ms::size_t ___size>
	literal(const char (&)[___size]) -> literal<___size>;


	/*
		struct ___impl_copy final {

			// copy
			template <ve::literal lit, ms::u32 size>
			static consteval auto copy(char (&buffer)[size], ms::u32& offset) noexcept -> void {
				for (ms::u32 i = 0U; i < lit.length(); ++i)
					buffer[offset++] = lit.data[i];
			}

		};
		*/

	/* merge */
		/*
	template <ve::literal... literals>
	consteval auto merge(void) noexcept -> ve::literal<((... + literals.length()) + 1U)> {

		// create buffer
		char buffer[(... + literals.length()) + 1U]{};

		// copy
		ms::u32 offset = 0U;
		((___impl_copy::copy<literals>(buffer, offset)), ...);

		// null-terminate
		buffer[offset] = '\0';

		// return literal
		return ve::literal{buffer};
	}
	*/


} // namespace ve
