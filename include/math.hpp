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


	/* lcm
	   compute the least common multiple of two numbers */
	constexpr auto lcm(mx::usz a, mx::usz b) noexcept -> mx::usz {
		if (a == 0U || b == 0U)
			return 0U;
		return (a / mx::gcd(a, b)) * b;
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
			constexpr frac(value_type n, value_type d) noexcept
			: num{n}, den{d} {
			}

			/* integer constructor */
			constexpr explicit frac(const value_type num) noexcept
			: num{num}, den{1U} {
			}


			/* reduce
			   reduce the fraction to its simplest form */
			constexpr auto reduce(void) noexcept -> self& {

				const auto g = mx::gcd(num, den);
				num /= g;
				den /= g;

				// OPTIMISATION POSSIBLE

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
			constexpr auto value(void) const noexcept -> double {

				// division by zero is not possible here
				return static_cast<double>(num)
					 / static_cast<double>(den);
			}


			// -- public operators --------------------------------------------

			/* add operator */
			constexpr auto operator+(const self& other) const noexcept -> self {

				// check overflow?
				return self{(num * other.den) + (other.num * den),
							 den * other.den};
			}


			/* multiply operator */
			 auto operator*(const self& other) const /*noexcept*/-> self {
				//std::cout os << f.num << "/" << f.den;
					self r{num * other.num,
							den * other.den};

				mx::usz a;
				mx::usz b;

				//if (__builtin_mul_overflow(num, other.num, &a))
				//	throw std::overflow_error("Fraction numerator overflow");
				//if (__builtin_mul_overflow(den, other.den, &b))
				//	throw std::overflow_error("Fraction denominator overflow");

				if (!(other.num == 1U && other.den == 1U)) {
					 //std::cout << "MULTIPLYING " << *this << " * " << other << "\n";
					 //std::cout << "RESULT: " << r << "\n";
				 }

				return r;
			}


			/* divide operator */
			constexpr auto operator/(const frac& other) const -> frac {



				self r{num * other.den, den * other.num};

				mx::usz a;
				mx::usz b;

				//if (__builtin_mul_overflow(num, other.den, &a))
				//	throw std::overflow_error("Fraction numerator overflow");
				//
				//if (__builtin_mul_overflow(den, other.num, &b))
				//	throw std::overflow_error("Fraction denominator overflow");


				if (!(other.num == 1U && other.den == 1U)) {
					 //std::cout << "MULTIPLYING " << *this << " * " << other << "\n";
					 //std::cout << "RESULT: " << r << "\n";
				}
				return r;
			}



			// -- self modifying operators ------------------------------------

			/* self add operator */
			constexpr auto operator+=(const self& other) noexcept -> void {
				num = (num * other.den) + (other.num * den);
				den = den * other.den;
			}

			/* self subtract operator */
			constexpr auto operator-=(const self& other) noexcept -> void {
				num = (num * other.den) - (other.num * den);
				den = den * other.den;
			}

			/* self multiply operator */
			constexpr auto operator*=(const self& other) noexcept -> void {
				num = num * other.num;
				den = den * other.den;
			}

			/* self divide operator */
			constexpr auto operator/=(const self& other) -> void {
				num = num * other.den;
				den = den * other.num;
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
				return (num * other.den) < (other.num * den);
			}

			/* greater than operator */
			constexpr auto operator>(const self& other) const noexcept -> bool {
				return (num * other.den) > (other.num * den);
			}

			/* less than or equal operator */
			constexpr auto operator<=(const self& other) const noexcept -> bool {
				return (num * other.den) <= (other.num * den);
			}

			/* greater than or equal operator */
			constexpr auto operator>=(const self& other) const noexcept -> bool {
				return (num * other.den) >= (other.num * den);
			}

		}; // struct frac


		/* frac mod
		   compute the modulus of two fractions */
		inline auto frac_mod(const mx::frac& a,
							 const mx::frac& b) /*noexcept*/ -> mx::frac {

			mx::usz ar;
			mx::usz br;

			if (__builtin_mul_overflow(a.num, b.den, &ar)) {
				std::cout << "Overflow: " << a.num << " * " << b.den << "\n";
				std::cout << "Result would be: " << a.num * b.den << "\n";
				throw std::overflow_error("FRAC_MOD Fraction modulus overflow");
			}
			if (__builtin_mul_overflow(b.num, a.den, &br)) {
				std::cout << "Overflow: " << b.num << " * " << a.den << "\n";
				std::cout << "Result would be: " << b.num * a.den << "\n";
				throw std::overflow_error("FRAC_MOD Fraction modulus overflow");
			}

			auto lhs = a.num * b.den;
			auto rhs = b.num * a.den;

			if (lhs < rhs)
				return a;

			double q = static_cast<double>(lhs)
					 / static_cast<double>(rhs);

			mx::usz k = static_cast<mx::usz>(::floor(q));

			mx::frac kb{k * b.num, b.den};

			// res = a - kb
			mx::frac res{(a.num * kb.den) - (kb.num * a.den),
							a.den * kb.den};
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
