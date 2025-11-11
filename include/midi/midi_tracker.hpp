#ifndef midi_midi_tracker_hpp
#define midi_midi_tracker_hpp

#include "types.hpp"
#include "coremidi/eventlist.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	class midi_tracker final {

		private:

			mx::u32  _table[16U * 128U];
			mx::u16 _active[16U * 128U];
			mx::u16 _count;


		public:

			midi_tracker(void) noexcept
			: _table{}, _active{}, _count{0U} {
			}

			auto reset(void) noexcept -> void {
				_count = 0U;
			}

			auto note_on(cm::eventlist& evs,
						 const mx::u8  ch,
						 const mx::u8  note,
						 mx::u32 ticks
						 ) noexcept -> void {

				const mx::u16 i = (ch * 128U) + note;

				if (ticks == 0U) {
					++ticks;
				}

				if (_table[i] != 0U) {
					_table[i] = ticks;
					return;
				}

				_table[i] = ticks;
				_active[_count++] = i;


			}

			auto off_pass(cm::eventlist& evs) -> void {

				mx::u16 i = 0U;
				while (i < _count) {

					const mx::u16 a = _active[i];

					// secure check (should not happen)

					if (_table[a] > 0U && --_table[a] != 0U) {
						++i; continue;
					}
					const mx::u8 ch   = static_cast<mx::u8>(a / 128U);
					const mx::u8 note = static_cast<mx::u8>(a % 128U);

					// note off event can be triggered here
					evs.note_off(ch, note);

					// swap pop
					_active[i] = _active[_count - 1U];

					--_count;
				}

			}


	};

} // namespace mx

#endif // midi_midi_tracker_hpp
