#ifndef midilang_time_bpm_hpp
#define midilang_time_bpm_hpp

#include "types.hpp"
#include "midi/constant.hpp"
#include <stdint.h>


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {



	// -- B P M ---------------------------------------------------------------

	class bpm final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::bpm;


			// -- private members ---------------------------------------------

			/* beats per minute */
			unsigned _bpm;


			// -- private constants -------------------------------------------

			/* constant */
			enum : ml::u64 {
				NANO_PER_MIN = 60'000'000'000U
			};


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			bpm(void) noexcept
			: _bpm{120U} {
			}

			/* bpm constructor */
			bpm(const unsigned& bpm) noexcept
			: _bpm{bpm} {
			}

			/* copy constructor */
			bpm(const self&) noexcept = default;

			/* move constructor */
			bpm(self&&) noexcept = default;

			/* destructor */
			~bpm(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;

			/* bpm assignment operator */
			auto operator=(const unsigned& bpm) noexcept -> self& {
				_bpm = bpm;
				return *this;
			}


			// -- public conversion operators ---------------------------------

			/* unsigned conversion operator */
			explicit operator unsigned(void) const noexcept {
				return _bpm;
			}


			// -- public methods ----------------------------------------------

			/* to nanoseconds */
			auto to_nano(void) const noexcept -> ml::u64 {
				return NANO_PER_MIN / (_bpm * MIDI_PPQN);
			}

	}; // class bpm

} // namespace ml

#endif // midilang_time_bpm_hpp
