#ifndef ml_debug_chars_hpp
#define ml_debug_chars_hpp

#include "types.hpp"
#include <iostream>


namespace ml {
	class dbc final {
		private:
			ml::u8 _c;

		public:
			dbc(const char c) noexcept
			: _c{static_cast<ml::u8>(c)} {
			}
			dbc(const ml::u8 c) noexcept
			: _c{c} {
			}
			friend auto operator<<(std::ostream&, const dbc&) -> std::ostream&;
	};

	// override << operator for char printing
	inline auto operator<<(std::ostream& os, const dbc& dbchar) -> std::ostream& {

		const char c = static_cast<char>(dbchar._c);

		if (c == 32) {
			os << "' '";
			return os;
		}
		if (c > 32 && c < 127) {
			os << c;
			return os;
		}
		switch (c) {
			case '\n': { os << "\\n"; return os; }
			case '\r': { os << "\\r"; return os; }
			case '\t': { os << "\\t"; return os; }
			case '\v': { os << "\\v"; return os; }
			case '\b': { os << "\\b"; return os; }
			case '\f': { os << "\\f"; return os; }
			case '\a': { os << "\\a"; return os; }
			default: {
				os << "\\x" << std::hex
				<< static_cast<unsigned int>(static_cast<unsigned char>(c));
				return os;
			}
		}
	}
}



#endif //ml_debug_chars_hpp
