#ifndef data_track_hpp
#define data_track_hpp

#include "data/sequence.hpp"
#include "coremidi/eventlist.hpp"
#include "midi/midi_tracker.hpp"
#include <sstream>

#include <math.h>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- T R A C K -----------------------------------------------------------

	class track final {


		// -- friends ---------------------------------------------------------

		/* parser as friend */
		friend class parser;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::track;


			// -- private members ---------------------------------------------

			/* indices to sequences */
			ml::usz _indices[ml::seq_type::seq_count];

			/* has sequence */
			bool _has_sequence[ml::seq_type::seq_count];

			/* values */
			ml::i8 _values[ml::seq_type::seq_count];

			/* states */
			const ml::seq_slot* _states[ml::seq_type::seq_count];


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			track(void) noexcept
			/* uninitialized */ {
				self::reset();
			}


			// -- public modifiers --------------------------------------------

			/* reset */
			auto reset(void) noexcept -> void {
				for (ml::usz i = 0U; i < ml::seq_type::seq_count; ++i) {
					_has_sequence[i] = false;
					_values[i] = ml::seq_defs[i];
					_states[i] = nullptr;
				}
			}


			// -- public methods ----------------------------------------------

			auto set_index(const ml::seq_type stype, const ml::usz sindex) noexcept -> void {
				_indices[stype]      = sindex;
				_has_sequence[stype] = true;
			}


			// -- public accessors --------------------------------------------

			/* index */
			template <ml::seq_type T>
			auto index(void) const noexcept -> ml::usz {
				static_assert(T < ml::seq_type::seq_count, "invalid sequence type");
				return _indices[T];
			}


			/* play */
			auto play(std::stringstream& ss, bool& first, mx::midi_tracker& tr, cm::eventlist& evs, std::vector<ml::sequence>& seqs, const ml::u64 timeline) noexcept -> void {

				for (mx::usz i = 0U; i < ml::seq_type::seq_count; ++i) {

					if (_has_sequence[i] == false)
						continue;

					const auto& seq = seqs[_indices[i]];

					if (seq.empty() == true)
						continue;

					_states[i] = &seq.next(timeline);
					_values[i] = _states[i]->value;

					const auto* t     = _states[i]->token;
					const char* group = "Underlined";

						if (!first)
							ss << ",";
						first = false;

						ss << "{\"l\":" << t->line
							<< ",\"s\":" << t->col_head
							<< ",\"e\":" << t->col_tail
							<< ",\"g\":\"" << group << "\"}";
				}

				if (_values[ml::seq_type::TR] == 0)
					return;

				mx::i8 note = _values[ml::seq_type::NT];


				mx::usz oc = note + (_values[ml::seq_type::OC] * 12U);

				// clamp note to valid range
				if (oc < 0U)
					note = 0U;
				else if (oc > 127U)
					note = 127;
				else
					note = static_cast<mx::i8>(oc);

				note += _values[ml::seq_type::SE];


				const mx::i8 velo = _values[ml::seq_type::VL];
				const mx::i8 chan = _values[ml::seq_type::CH];
				evs.note_on(static_cast<mx::u8>(chan),
							static_cast<mx::u8>(note),
							static_cast<mx::u8>(velo));


				// current gate
				auto gate = _values[ml::seq_type::GA];
				const auto& trigs = seqs[_indices[ml::seq_type::TR]];
				const auto& v = trigs.data();

				mx::usz i = 1U;
				// search next trigger
				for (; i < v.size(); ++i) {
					mx::usz idx = (i + timeline) % v.size();

					if (v[idx].value != 0)
						break;
				}

				const float gate_pct = (float)gate / 100.f;
				const mx::u32 when = static_cast<mx::u32>(std::round(gate_pct * static_cast<float>(i)));

				tr.note_on(evs, static_cast<mx::u8>(chan),
							static_cast<mx::u8>(note),
							when);
			}


			auto debug(const std::vector<ml::sequence>& seqs) const -> void {

				for (ml::usz i = 0U; i < ml::seq_type::seq_count; ++i) {

					if (_has_sequence[i] == false)
						continue;

					std::cout << "    SEQ[" << i << "] INDEX: " << _indices[i] << " DATA: ";

					const auto& data = seqs[_indices[i]].data();

					for (mx::usz j = 0U; j < data.size(); ++j) {
						std::cout << static_cast<ml::i32>(data[j].value);
						if (j + 1U < data.size())
							std::cout << ", ";
					}


					std::cout << "\n";
				}
			}


	}; // class track

} // namespace ml

#endif // midilang_data_track_hpp
