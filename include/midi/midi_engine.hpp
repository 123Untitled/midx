#ifndef midi_midi_engine_hpp
#define midi_midi_engine_hpp

#include "core/types.hpp"
//#include "coremidi/events.hpp"
//#include "coremidi/coremidi.hpp"
#include "midi/midi_event.hpp"

//#include "diagnostics/runtime_error.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- M I D I  E N G I N E ------------------------------------------------

	class midi_engine final {


		private:

			// -- private constants -------------------------------------------

			/* maximum notes (16 channels x 128 notes = 2048) */
			constexpr static mx::usz max_notes = 16U * 128U;

			struct state final {
				mx::u16 ticks;
				mx::u16 epoch;
			}; // struct state


			// -- private members ---------------------------------------------

			state   _states[max_notes];
			mx::u16 _active[max_notes];
			mx::u16 _count;
			mx::u16 _epoch;


			// -- private static methods --------------------------------------

			inline static auto idx_to_ch(const mx::u16 index) noexcept -> mx::u8 {
				return static_cast<mx::u8>(index >> 7U);
			}

			inline static auto idx_to_no(const mx::u16 index) noexcept -> mx::u8 {
				return static_cast<mx::u8>(index & 0x7FU);
			}

			/* to index */
			static auto to_index(const mx::u8 ch, const mx::u8 no) noexcept -> mx::u16 {
				return (static_cast<mx::usz>(ch) << 7U) | no;
			}


			/* reset */
			auto _reset(void) noexcept -> void {

				// reset states
				for (mx::u16 i = 0U; i < max_notes; ++i)
					_states[i].ticks = _states[i].epoch = 0U;

				_count = 0U;
				_epoch = 1U;
			}

		public:


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			midi_engine(void)
			: _states{},
			  _active{},
			  _count{0U},
			  _epoch{1U} {
			}

			/* off all */
			template <typename T>
			auto off_all(T& evs) -> void {

				// note off all notes
				for (; _count > 0U; --_count) {
					const auto i = _active[_count - 1U];
					evs.note_off(idx_to_ch(i), idx_to_no(i));
				}

				_reset();
			}


			/* next epoch */
			auto next_epoch(void) noexcept -> void {

				// increment epoch, reset if overflow
				if (_epoch == 0xFFFFU) {
					for (mx::u16 i = 0U; i < max_notes; ++i)
						_states[i].epoch = 0U;
					_epoch = 1U;
					return;
				}
				++_epoch;
			}


			/* note on */
			template <typename T>
			auto note_on(T& evs, const mx::midi_event& ev) -> void {

				// get index
				const mx::u16 i = to_index(ev.channel, ev.note);

				// get state
				auto& st = _states[i];

				// ensure gate is not zero
				if (ev.gate == 0U)
					return;

				// check if already note on
				if (st.epoch == _epoch) {
					st.ticks = ev.gate;
					return;
				}

				// check if not already active
				if (st.ticks == 0U) {
					_active[_count++] = i;
				}
				else {
					//std::cout << "\x1b[33mWarning:\x1b[0m "
					//		  << "Retriggering note before note off. "
					//		  << "Ch " << static_cast<mx::usz>(ch)
					//		  << " No " << static_cast<mx::usz>(no)
					//		  << " Ticks " << ticks << std::endl;
					// note off before note on
					evs.note_off(ev.channel, ev.note);
				}

				// note on event can be triggered here
				evs.note_on(ev.channel, ev.note, ev.velocity);
				//std::cout << "\x1b[32mNote On:\x1b[0m Ch " << static_cast<mx::usz>(ch)
				//		  << " No " << static_cast<mx::usz>(no)
				//		  << " Ve " << static_cast<mx::usz>(ve)
				//		  << " Ticks " << ticks << std::endl;

				st.epoch = _epoch;
				st.ticks = ev.gate;
			}

			/* off pass */
			template <typename T>
			auto off_pass(T& evs) -> void {

				mx::u16 i = 0U;
				while (i < _count) {

					const mx::u16 a = _active[i];

					// decrement ticks, continue if not zero
					if (--_states[a].ticks != 0U) {
						++i;
						continue;
					}

					// compute channel and note
					const mx::u8 ch = idx_to_ch(a);
					const mx::u8 no = idx_to_no(a);

					// note off event can be triggered here
					evs.note_off(ch, no);
					//std::cout << "\x1b[31mNote Off:\x1b[0m Ch " << static_cast<mx::usz>(ch)
					//		  << " No " << static_cast<mx::usz>(no) << std::endl;

					// swap pop
					_active[i] = _active[--_count];
				}

			}

	}; // class midi_engine

} // namespace mx

#endif // midi_midi_engine_hpp
