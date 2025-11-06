#ifndef data_track_hpp
#define data_track_hpp

#include "data/sequence.hpp"
#include "coremidi/eventlist.hpp"
//#include "msh/math/round.hpp"
//#include "msh/math/ceil.hpp"
//#include "msh/midi/midi.hpp"

#include <math.h>


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	class note_off final {


		private:

			// -- private members ---------------------------------------------

			/* active */
			bool _active;

			/* channel */
			ml::u8 _channel;

			/* note */
			ml::u8 _note;

			/* when */
			ml::usz _when;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			note_off(void) noexcept
			: _active{false}, _channel{0U}, _note{0U},  _when{0U} {
			}
			
			/* note / when constructor */
			note_off(const ml::u8& channel, const ml::u8& note, const ml::usz& when) noexcept
			: _active{true}, _channel{channel}, _note{note}, _when{when} {
			}

			/* copy constructor */
			note_off(const note_off&) noexcept = default;

			/* move constructor */
			note_off(note_off&&) noexcept = default;

			/* destructor */
			~note_off(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const note_off&) noexcept -> note_off& = default;

			/* move assignment operator */
			auto operator=(note_off&&) noexcept -> note_off& = default;


			// -- public modifiers --------------------------------------------

			/* reset */
			auto reset(void) noexcept -> void {
				_active = false;
				_when = 0U;
			}

			/* force off */
			auto force_off(void) noexcept -> void {
				_when = 0U;
			}


			// -- public accessors --------------------------------------------

			/* is off */
			auto is_off(void) noexcept -> bool {

				_when -= (_when > 0U) ? 1U : 0U;

				return _when == 0U ? _active = false, true : false;
			}

			/* active */
			auto active(void) const noexcept -> bool {
				return _active;
			}

			/* note */
			auto note(void) const noexcept -> ml::u8 {
				return _note;
			}

			/* channel */
			auto channel(void) const noexcept -> ml::u8 {
				return _channel;
			}

	}; // class note_off



	// -- T R A C K -----------------------------------------------------------

	class track final {


		// -- friends ---------------------------------------------------------

		/* parser as friend */
		friend class parser;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::track;


			// -- private members ---------------------------------------------
		public:

			/* trig index */
			ml::usz tr_index;

			/* channel index */
			ml::usz ch_index;

			/* note index */
			ml::usz nt_index;

			/* gate index */
			ml::usz ga_index;

			/* velocity index */
			ml::usz vl_index;

			/* octave index */
			ml::usz oc_index;

			/* semitone index */
			ml::usz se_index;

			/* probability index */
			ml::usz pr_index;



			/* note off */
			ml::note_off _noff;



			// -- public lifecycle --------------------------------------------

			/* default constructor */
			track(void) = default;


			// -- public modifiers --------------------------------------------

			/* reset */
			auto reset(void) noexcept -> void {
				_noff.force_off();
			}


			// -- public methods ----------------------------------------------

			/* play */
			auto play(cm::eventlist& evs, const ml::usz& timeline) -> void {


				// bug reason: fs::track is deleted when vim saves
				// so _noff is not triggered

				/*
				if (_noff.active() == true) {

					if (_noff.is_off() == true) {
						evs.note_off(_noff.channel(), _noff.note());
						std::cout << "\x1b[34m\\" << std::flush;
					}
					else
						std::cout << "\x1b[35m-" << std::flush;
				}

				  _channels.next(timeline);
					 _gates.next(timeline);
					 _notes.next(timeline);
				_velocities.next(timeline);
				*/

				//if (_triggers.signature().is_time(timeline) == false
				// || _triggers.data().empty() == true)
				//	return;

				// get count from timeline beginning
				//const auto count = _triggers.signature().count(timeline);

				// compute position
				//const auto pos = count % _triggers.data().size();

				// check if trigger is set
				//if (_triggers.data()[pos] == false)
				//	return;


				//ml::usz p = (pos + 1U) % _triggers.data().size();
				//ml::usz i = 1U;

				// get next trigger
				//while (p != pos) {
				//
				//	if (_triggers.data()[p] == true)
				//		break;
				//
				//	p = (p + 1U) % _triggers.data().size();
				//	++i;
				//}

				// convert i to ppqn
				//const ml::usz ticks = _triggers.signature().to_ticks(i);


				//_gates.next(timeline);
				//_notes.next(timeline);

				//const auto when =
				//	static_cast<ml::usz>(::round(static_cast<double>(ticks))
				//	* (static_cast<double>(_gates.current()) / 100.0));

				//const auto w = ml::round<ml::usz>(when);


				//if (when == 0U)
				//	return;
				//
				//evs.note_on(_channels.current(), _notes.current(), _velocities.current());
				//
				//_noff = ml::note_off{
				//	_channels.current(),
				//	_notes.current(),
				//	when
				//};
				//
				//std::cout << "\x1b[32m/" << std::flush;
			}


	}; // class track

} // namespace ml

#endif // midilang_data_track_hpp
