#ifndef data_model_hpp
#define data_model_hpp

#include <vector>
#include "data/sequence.hpp"
#include "data/track.hpp"

#include "midi/midi_engine.hpp"

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

			/* sequences */
			std::vector<mx::sequence> _seqs;

			/* tracks */
			std::vector<mx::track> _tracks;


			/* patterns */
			// ...


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			model(void) noexcept
			: _seqs{}, _tracks{} {
			}

			/* copy constructor */
			model(const model&) = default;

			/* move constructor */
			model(model&&) noexcept = default;

			/* destructor */
			~model(void) noexcept = default;


			auto new_sequence(void) -> mx::usz {
				const mx::usz idx = _seqs.size();
				static_cast<void>(_seqs.emplace_back());
				return idx;
			}

			auto new_track(void) -> mx::usz {
				const mx::usz idx = _tracks.size();
				static_cast<void>(_tracks.emplace_back());
				return idx;
			}

			auto get_sequence(const mx::usz idx) noexcept -> mx::sequence& {
				return _seqs[idx];
			}

			auto get_track(const mx::usz idx) noexcept -> mx::track& {
				return _tracks[idx];
			}

			auto clear(void) noexcept -> void {
				  _tracks.clear();
				    _seqs.clear();
			}

			auto play(std::stringstream& ss, mx::midi_engine& engine, const mx::u64 timeline) -> void {

				for (auto& t : _tracks) {
					t.play(ss, engine, _seqs, timeline);
				}
			}

			auto debug(void) const -> void {
				std::cout << "MODEL DEBUG:\n";

				std::cout << "  TRACKS: " << _tracks.size() << "\n";
				for (mx::usz i = 0U; i < _tracks.size(); ++i) {
					std::cout << "  TRACK[" << i << "] DEBUG:\n";
					_tracks[i].debug(_seqs);
				}
			}

	};


} // namespace mx

#endif // data_model_hpp
