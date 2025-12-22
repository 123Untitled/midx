#ifndef math_hpp
#define math_hpp

#include "core/types.hpp"
#include <stdexcept>

#include <iostream>
#include <ostream>

namespace mx {
	struct frac;
}
inline auto operator<<(std::ostream& os, const mx::frac& f) -> std::ostream&;


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	/* gcd
	   compute the greatest common divisor of two numbers */
	constexpr auto gcd(mx::usz a, mx::usz b) noexcept -> mx::usz {
		while (b != 0U) {
			const mx::usz t = b;
			b = a % b;
			a = t;
		}
		return a;
	}

	/* gcd binary
	   compute the greatest common divisor of two numbers using the binary algorithm */
	constexpr auto gcd_binary(mx::usz a, mx::usz b) noexcept -> mx::usz {
		if (a == 0U)
			return b;
		if (b == 0U)
			return a;

		mx::usz shift;
		for (shift = 0U; ((a | b) & 1U) == 0U; ++shift) {
			a >>= 1U;
			b >>= 1U;
		}
		while ((a & 1U) == 0U)
			a >>= 1U;
		do {
			while ((b & 1U) == 0U)
				b >>= 1U;
			if (a > b) {
				const mx::usz t = b;
				b = a;
				a = t;
			}
			b = b - a;
		} while (b != 0U);
		return a << shift;
	}


	// slow gcd for test purposes
	constexpr auto gcd_test(mx::usz a, mx::usz b) noexcept -> mx::usz {

		while (a != b) {

			if (b > a) {
				b = b - a;
				continue;
			}
			// a > b
			const auto t = a - b;
			a = b;
			b = t;
		}
		return a;
	}



	/* lcm
	   compute the least common multiple of two numbers */
	constexpr auto lcm(mx::usz a, mx::usz b) noexcept -> mx::usz {
		if (a == 0U || b == 0U)
			return 0U;
		return (a / mx::gcd(a, b)) * b;
	}


	inline auto safe_mul(mx::usz a, mx::usz b) noexcept -> mx::usz {
		mx::usz res;
		while (__builtin_mul_overflow(a, b, &res))
			a /= 2U, b /= 2U;
		return res;
	}


	// -- F R A C T -----------------------------------------------------------

	struct frac final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::frac;

			/* value type */
			using value_type = mx::usz;


		public:

			// -- public members ----------------------------------------------

			/* numerator */
			value_type num;

			/* denominator */
			value_type den;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			constexpr frac(void) noexcept
			: num{0U}, den{1U} {
			}

			/* full constructor */
			constexpr frac(const value_type n, const value_type d) noexcept
			: num{n}, den{d} {
				reduce();
			}

			/* integer constructor */
			constexpr explicit frac(const value_type num) noexcept
			: num{num}, den{1U} {
			}

			/* sentinel constructor */
			constexpr explicit frac(decltype(nullptr)) noexcept
			: num{0U}, den{0U} {
			}

			//static constexpr auto sqrt(void) noexcept -> value_type {
			//	// compute compile time square root of mx::usz max
			//	constexpr value_type x = mx::usz(-1);
			//	return 0;
			//}


			/* reduce
			   reduce the fraction to its simplest form */
			auto reduce(void) noexcept -> self& {

				const auto g = mx::gcd(num, den);
				num /= g;
				den /= g;

				return *this;

				// racine carre de mx::usz max
				auto sq = ::pow(2U, 32U);

				// OPTIMISATION POSSIBLE
				while (num > sq || den > sq) {
					num /= 2U;
					den /= 2U;
					const auto gg = mx::gcd(num, den);
					num /= gg;
					den /= gg;
				}

				//if (den == 0U) {
				//	num = 1U;
				//	den = 1U;
				//}

				return *this;
			}

			/* invert
			   invert the fraction */
			constexpr auto invert(void) noexcept -> self& {

				const auto tmp = num;
				num = den;
				den = tmp;

				return *this;
			}


			/* reset
			   reset the fraction to 0/1 */
			constexpr auto reset(void) noexcept -> void {
				num = 1U;
				den = 1U;
			}

			/* fix
			   fix the fraction by ensuring num / den is not zero */
			constexpr auto fix(void) noexcept -> self& {
				num = num == 0U ? 1U : num;
				den = den == 0U ? 1U : den;
				return *this;
			}


			// -- public accessors --------------------------------------------

			/* value
			   return the floating point value of the fraction */
			template <typename T = double>
			constexpr auto value(void) const noexcept -> T {

				// division by zero is not possible here
				return static_cast<T>(num)
					 / static_cast<T>(den);
			}


			// -- public operators --------------------------------------------

			/* add operator */
			constexpr auto operator+(const self& other) const noexcept -> self {
				return self{safe_mul(num, other.den) + safe_mul(other.num, den),
							safe_mul(den, other.den)};
			}

			/* subtract operator */
			constexpr auto operator-(const self& other) const noexcept -> self {
				return self{safe_mul(num, other.den) - safe_mul(other.num, den),
							safe_mul(den, other.den)};
			}


			/* multiply operator */
			 auto operator*(const self& other) const /*noexcept*/-> self {
				 return self{safe_mul(num, other.num),
							 safe_mul(den, other.den)};
			}


			/* divide operator */
			constexpr auto operator/(const self& other) const -> frac {

				return self{safe_mul(num, other.den),
							safe_mul(den, other.num)};
			}


			/* divide operator with integer */
			constexpr auto operator/(const value_type other) const -> frac {
				return self{num, safe_mul(den, other)};
			}

			/* multiply operator with integer */
			constexpr auto operator*(const value_type other) const noexcept -> frac {
				return self{safe_mul(num, other), den};
			}



			// -- self modifying operators ------------------------------------

			/* self add operator */
			constexpr auto operator+=(const self& other) noexcept -> void {
				num = safe_mul(num, other.den) + safe_mul(other.num, den);
				den = safe_mul(den, other.den);
				reduce();
			}

			/* self subtract operator */
			constexpr auto operator-=(const self& other) noexcept -> void {
				num = safe_mul(num, other.den) - safe_mul(other.num, den);
				den = safe_mul(den, other.den);
				reduce();
			}

			/* self multiply operator */
			constexpr auto operator*=(const self& other) noexcept -> void {
				num = safe_mul(num, other.num);
				den = safe_mul(den, other.den);
				reduce();
			}

			/* self divide operator */
			constexpr auto operator/=(const self& other) -> void {
				num = safe_mul(num, other.den);
				den = safe_mul(den, other.num);
				reduce();
			}


			/* equality operators */
			constexpr auto operator==(const self& other) const noexcept -> bool {
				return num == other.num && den == other.den;
			}

			/* inequality operators */
			constexpr auto operator!=(const self& other) const noexcept -> bool {
				return num != other.num || den != other.den;
			}

			/* less than operator */
			constexpr auto operator<(const self& other) const noexcept -> bool {
				return safe_mul(num, other.den) < safe_mul(other.num, den);
			}

			/* greater than operator */
			constexpr auto operator>(const self& other) const noexcept -> bool {
				return safe_mul(num, other.den) > safe_mul(other.num, den);
			}

			/* less than or equal operator */
			constexpr auto operator<=(const self& other) const noexcept -> bool {
				return safe_mul(num, other.den) <= safe_mul(other.num, den);
			}

			/* greater than or equal operator */
			constexpr auto operator>=(const self& other) const noexcept -> bool {
				return safe_mul(num, other.den) >= safe_mul(other.num, den);
			}

		}; // struct frac


		/* frac mod
		   compute the modulus of two fractions */
		//inline auto frac_mod(const mx::frac& a,
		//					 const mx::frac& b) /*noexcept*/ -> mx::frac {
		//
		//	mx::usz ar;
		//	mx::usz br;
		//
		//	if (__builtin_mul_overflow(a.num, b.den, &ar)) {
		//		std::cout << "Overflow: " << a.num << " * " << b.den << "\n";
		//		std::cout << "Result would be: " << a.num * b.den << "\n";
		//		throw std::overflow_error("FRAC_MOD Fraction modulus overflow");
		//	}
		//	if (__builtin_mul_overflow(b.num, a.den, &br)) {
		//		std::cout << "Overflow: " << b.num << " * " << a.den << "\n";
		//		std::cout << "Result would be: " << b.num * a.den << "\n";
		//		throw std::overflow_error("FRAC_MOD Fraction modulus overflow");
		//	}
		//
		//	auto lhs = a.num * b.den;
		//	auto rhs = b.num * a.den;
		//
		//	if (lhs < rhs)
		//		return a;
		//
		//	double q = static_cast<double>(lhs)
		//			 / static_cast<double>(rhs);
		//
		//	mx::usz k = static_cast<mx::usz>(::floor(q));
		//
		//	mx::frac kb{k * b.num, b.den};
		//
		//	// res = a - kb
		//	mx::frac res{(a.num * kb.den) - (kb.num * a.den),
		//					a.den * kb.den};
		//	res.reduce();
		//	return res;
		//}




		inline auto frac_mod(const mx::frac& a, const mx::frac& b) noexcept -> mx::frac {

			if (safe_mul(a.num, b.den) < safe_mul(b.num, a.den))
				return a;

			mx::usz lhs_num = a.num;
			mx::usz lhs_den = a.den;
			mx::usz rhs_num = b.num;
			mx::usz rhs_den = b.den;


			mx::usz g1 = mx::gcd(lhs_num, lhs_den);
			lhs_num /= g1;
			lhs_den /= g1;

			mx::usz g2 = mx::gcd(rhs_num, rhs_den);
			rhs_num /= g2;
			rhs_den /= g2;

			// floor(a/b) = floor((a.num*b.den)/(b.num*a.den))
			mx::usz num_scaled = safe_mul(lhs_num, rhs_den);
			mx::usz den_scaled = safe_mul(rhs_num, lhs_den);

			if (den_scaled == 0)
				den_scaled = 1;

			mx::usz q = num_scaled / den_scaled;

			// q*b
			mx::usz kb_num = safe_mul(q, b.num);
			mx::usz kb_den = b.den;

			// result = a - q*b
			mx::usz res_num = safe_mul(a.num, kb_den) - safe_mul(kb_num, a.den);
			mx::usz res_den = safe_mul(a.den, kb_den);

			mx::frac res{res_num, res_den};
			res.reduce();
			return res;
		}


		inline auto lcm_frac(const mx::frac& f1,
							 const mx::frac& f2) noexcept -> mx::frac {


			// L1 = n1/d1
			// L2 = n2/d2
			const auto n1 = f1.num;
			const auto d1 = f1.den;
			const auto n2 = f2.num;
			const auto d2 = f2.den;

			// On veut la plus petite durée T telle que :
			//   T est un multiple de L1 et L2
			// En pratique, pour des durées de type "steps * ratio",
			// tu peux utiliser: L = lcm(n1*d2, n2*d1) / (d1*d2)
			//
			// Ici on retourne directement la fraction L.

			const auto A = n1 * d2;
			const auto B = n2 * d1;

			const auto N = mx::lcm(A, B);   // num
			const auto D = d1 * d2;         // den

			return mx::frac{N, D};
		}


		inline auto make_reduced_frac(const mx::usz n,
									  const mx::usz d) noexcept -> mx::frac {
			mx::frac f{n, d};
			f.reduce();
			return f;
		}

} // namespace mx

// << operator for fraction
inline auto operator<<(std::ostream& os, const mx::frac& f) -> std::ostream& {
	os << f.num << "/" << f.den;
	return os;
}

#endif // math_hpp

			///* scalar multiply operator */
			//constexpr auto operator*(const value_type k) const noexcept -> mx::frac {
			//
			//	if (k == 0U) // HERE NOT THROWING
			//		return mx::frac{1U /*0U*/, 1U};
			//
			//	return self{num * k, den};
			//}
			//
			//
			//// division par scalaire entier k (k > 0)
			//constexpr auto operator/(value_type k) const -> frac {
			//	if (k == 0U)
			//		throw std::runtime_error{"fraction: division by zero scalar"};
			//	return self{num, den * k};
			//}
