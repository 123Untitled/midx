#pragma once

#include "core/type_traits/relationships_and_property_queries/is_one_of.hpp"
#include "core/type_traits/type_modifications/remove_cv.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace ms {


	// -- I S  I N T E G R A L ------------------------------------------------

	/* is integral */
	template <typename T>
	concept is_integral = ms::is_one_of<ms::remove_cv<T>,
		char, wchar_t, char8_t, char16_t, char32_t,
		signed char, unsigned char,
		short, unsigned short,
		int, unsigned int,
		long, unsigned long,
		long long, unsigned long long,
		bool>;

} // namespace ms
