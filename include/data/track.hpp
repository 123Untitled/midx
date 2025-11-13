#ifndef data_track_hpp
#define data_track_hpp

#include "data/sequence.hpp"
#include "midi/midi_engine.hpp"
#include "data/node.hpp"

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
			mx::usz _indices[mx::seq_type::seq_count];

			/* has sequence */
			bool _has_sequence[mx::seq_type::seq_count];

			/* values */
			mx::i8 _values[mx::seq_type::seq_count];

			/* states */
			const mx::seq_slot* _states[mx::seq_type::seq_count];


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
				for (mx::usz i = 0U; i < mx::seq_type::seq_count; ++i) {
					_has_sequence[i] = false;
					_values[i] = mx::seq_defs[i];
					_states[i] = nullptr;
				}
			}


			// -- public methods ----------------------------------------------

			auto set_index(const mx::seq_type stype, const mx::usz sindex) noexcept -> void {
				_indices[stype]      = sindex;
				_has_sequence[stype] = true;
			}


			// -- public accessors --------------------------------------------

			/* index */
			template <mx::seq_type T>
			auto index(void) const noexcept -> mx::usz {
				static_assert(T < mx::seq_type::seq_count, "invalid sequence type");
				return _indices[T];
			}


			/* play */
			//auto play(std::stringstream& ss, mx::midi_engine& engine, std::vector<mx::sequence>& seqs, const mx::u64 timeline) noexcept -> void {
			auto play(std::stringstream& ss, mx::midi_engine& engine,
					  std::vector<mx::atomic_type>& seqs, const mx::u64 timeline) noexcept -> void {

				for (mx::usz i = 0U; i < mx::seq_type::seq_count; ++i) {

					if (_has_sequence[i] == false)
						continue;

					const auto& seq = seqs[_indices[i]];

					const mx::i8 n = seq.evaluate(timeline, mx::seq_defs[i]);
					std::cout << (mx::u32)n << " ";

					/*
					if (seq.empty() == true)
						continue;

					_states[i] = &seq.next(timeline);
					_values[i] = _states[i]->value;

					const auto* t     = _states[i]->token;
					const char* group = "IncSearch";

					ss << "{\"l\":" << t->line
					   << ",\"s\":" << t->col_head
					   << ",\"e\":" << t->col_tail
					   << ",\"g\":\"" << group << "\"},";
					   */
				}

				/*
				if (_values[mx::seq_type::TR] == 0)
					return;


				const mx::i8 velo = _values[mx::seq_type::VL];
				const mx::i8 chan = _values[mx::seq_type::CH];
				mx::i8 note = _values[mx::seq_type::NT];


				mx::usz oc = note + (_values[mx::seq_type::OC] * 12U);

				// clamp note to valid range
				if (oc < 0U)
					note = 0U;
				else if (oc > 127U)
					note = 127;
				else
					note = static_cast<mx::i8>(oc);

				note += _values[mx::seq_type::SE];


				// current gate
				auto gate = _values[mx::seq_type::GA];
				const auto& trigs = seqs[_indices[mx::seq_type::TR]];
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

				// send note on event
				engine.note_on(chan, note, velo, when);
				*/
			}


			auto debug(const std::vector<mx::sequence>& seqs) const -> void {

				for (mx::usz i = 0U; i < mx::seq_type::seq_count; ++i) {

					if (_has_sequence[i] == false)
						continue;

					std::cout << "    SEQ[" << i << "] INDEX: " << _indices[i] << " DATA: ";

					const auto& data = seqs[_indices[i]].data();

					for (mx::usz j = 0U; j < data.size(); ++j) {
						std::cout << static_cast<mx::i32>(data[j].value);
						if (j + 1U < data.size())
							std::cout << ", ";
					}


					std::cout << "\n";
				}
			}


	}; // class track

} // namespace mx

#endif // midilang_data_track_hpp
