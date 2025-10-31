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

		if (c >= 32 && c < 127) {
			os << c;
			return os;
		}
		os << "\x1b[32m";
		switch (c) {
			case '\n': { os << "\\n"; break; }
			case '\r': { os << "\\r"; break; }
			case '\t': { os << "\\t"; break; }
			case '\v': { os << "\\v"; break; }
			case '\b': { os << "\\b"; break; }
			case '\f': { os << "\\f"; break; }
			case '\a': { os << "\\a"; break; }
			default: {
				os << "\\x" << std::hex
				<< static_cast<unsigned int>(static_cast<unsigned char>(c));
			}
		}
		os << "\x1b[0m";
		return os;
	}
}



#endif //ml_debug_chars_hpp
