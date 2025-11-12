#ifndef midilang_time_signature_hpp
#define midilang_time_signature_hpp

#include "types.hpp"
#include "midi/constant.hpp"


// TODO: check for zero division !!!


// -- M L  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- S I G N A T U R E ---------------------------------------------------

	class signature final {

		public:

			// -- public types ------------------------------------------------

			/* value type */
			using value_type = mx::u32;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::signature;


			// -- private members ---------------------------------------------

			/* numerator */
			value_type _numerator;

			/* denominator */
			value_type _denominator;

			/* modulus */
			value_type _modulus;


			// -- private methods ---------------------------------------------

			/* compute modulus */
			auto _compute_modulus(void) const noexcept -> value_type {
				return (_numerator * MIDI_PPQN * 4U) / _denominator;
			}

			/* compute modulus */
			template <value_type num, value_type den>
			static consteval auto _compute_modulus(void) noexcept -> value_type {
				return (num * MIDI_PPQN * 4U) / den;
			}


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			signature(void) noexcept
			: _numerator{1U}, _denominator{16U},
			  _modulus{self::_compute_modulus<1U, 16U>()} {
			}

			/* constructor */
			signature(const value_type& num, const value_type& den) noexcept
			: _numerator{num}, _denominator{den},
			  _modulus{self::_compute_modulus()} {
			}

			/* copy constructor */
			signature(const self&) noexcept = default;

			/* move constructor */
			signature(self&&) noexcept = default;

			/* destructor */
			~signature(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public accessors --------------------------------------------

			/* numerator */
			auto numerator(void) const noexcept -> const value_type& {
				return _numerator;
			}

			/* denominator */
			auto denominator(void) const noexcept -> const value_type& {
				return _denominator;
			}

			/* modulus */
			auto modulus(void) const noexcept -> const value_type& {
				return _modulus;
			}

			/* is time */
			auto is_time(const value_type& count) const noexcept -> bool {
				return (count % _modulus) == 0U;
			}

			/* count */
			auto count(const value_type& clock) const noexcept -> value_type {
				return clock / _modulus;
			}

			/* to ticks */
			auto to_ticks(const value_type& count) const noexcept -> value_type {
				return count * _modulus;
			}


			// -- public modifiers --------------------------------------------

			/* numerator */
			auto numerator(const value_type& num) noexcept -> void {
				_numerator = num;
				_modulus = self::_compute_modulus();
			}

			/* denominator */
			auto denominator(const value_type& den) noexcept -> void {
				_denominator = den;
				_modulus = self::_compute_modulus();
			}

			/* assign */
			auto assign(const value_type& num, const value_type& den) noexcept -> void {
				_numerator = num;
				_denominator = den;
				_modulus = self::_compute_modulus();
			}


			// -- public comparison operators ---------------------------------

			/* equal */
			auto operator==(const self& other) const noexcept -> bool {
				return _modulus == other._modulus;
			}

			/* not equal */
			auto operator!=(const self& other) const noexcept -> bool {
				return _modulus != other._modulus;
			}

			/* less than */
			auto operator<(const self& other) const noexcept -> bool {
				return _modulus < other._modulus;
			}

			/* less than or equal */
			auto operator<=(const self& other) const noexcept -> bool {
				return _modulus <= other._modulus;
			}

			/* greater than */
			auto operator>(const self& other) const noexcept -> bool {
				return _modulus > other._modulus;
			}

			/* greater than or equal */
			auto operator>=(const self& other) const noexcept -> bool {
				return _modulus >= other._modulus;
			}

	}; // class signature

} // namespace mx

#endif // midilang_time_signature_hpp
