#ifndef data_model_hpp
#define data_model_hpp

#include <vector>
#include "data/sequence.hpp"
#include "data/track.hpp"

#include "coremidi/eventlist.hpp"
#include "midi/midi_tracker.hpp"

#include <sstream>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- M O D E L -----------------------------------------------------------

	class model final {


		private:
			
			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::model;



			// -- private members ---------------------------------------------

			/* tracker */
			mx::midi_tracker _tracker;


			/* sequences */
			std::vector<ml::sequence> _seqs;


			/* tracks */
			std::vector<mx::track> _tracks;

			/* patterns */
			// ...


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			model(void) noexcept
			: _tracker{},
			  _seqs{},
			  _tracks{} {
			}

			/* copy constructor */
			model(const model&) = default;

			/* move constructor */
			model(model&&) noexcept = default;

			/* destructor */
			~model(void) noexcept = default;


			auto new_sequence(void) -> ml::usz {
				const mx::usz idx = _seqs.size();
				static_cast<void>(_seqs.emplace_back());
				return idx;
			}

			auto new_track(void) -> mx::usz {
				const mx::usz idx = _tracks.size();
				static_cast<void>(_tracks.emplace_back());
				return idx;
			}

			auto get_sequence(const mx::usz idx) noexcept -> ml::sequence& {
				return _seqs[idx];
			}

			auto get_track(const mx::usz idx) noexcept -> mx::track& {
				return _tracks[idx];
			}

			auto note_off_all(cm::eventlist& evs) -> void {
				_tracker.off_pass(evs);
			}

			auto clear(void) noexcept -> void {
				  _tracks.clear();
				    _seqs.clear();
			}

			auto play(std::stringstream& ss, cm::eventlist& evs, const ml::u64 timeline) -> void {

				_tracker.off_pass(evs);
				bool first = true;

				for (auto& t : _tracks) {
					t.play(ss, first, _tracker, evs, _seqs, timeline);
				}
			}

			auto debug(void) const -> void {
				std::cout << "MODEL DEBUG:\n";

				std::cout << "  TRACKS: " << _tracks.size() << "\n";
				for (ml::usz i = 0U; i < _tracks.size(); ++i) {
					std::cout << "  TRACK[" << i << "] DEBUG:\n";
					_tracks[i].debug(_seqs);
				}
			}

	};


} // namespace mx

#endif // data_model_hpp
