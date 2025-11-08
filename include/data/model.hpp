#ifndef data_model_hpp
#define data_model_hpp

#include <vector>
#include "data/sequence.hpp"
#include "data/track.hpp"
#include "data/pattern.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- M O D E L -----------------------------------------------------------

	class model final {


		private:
			
			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::model;

			enum defaults : ml::i8 {
				TR_DEFAULT = 0,
				NT_DEFAULT = 60,
				GA_DEFAULT = 100,
				CH_DEFAULT = 0,
				VL_DEFAULT = 127,
				OC_DEFAULT = 0,
				SE_DEFAULT = 0,
				PR_DEFAULT = 100
			};


			// -- private members ---------------------------------------------

			/* sequences */
			std::vector<ml::sequence> _seqs;


			/* tracks */
			std::vector<ml::track> _tracks;

			/* patterns */
			std::vector<ml::pattern> _patterns;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			model(void) noexcept {
			}

			/* copy constructor */
			model(const model&) = default;

			/* move constructor */
			model(model&&) noexcept = default;

			/* destructor */
			~model(void) noexcept = default;


			auto new_sequence(void) -> ml::usz {
				_seqs.emplace_back();
				return _seqs.size() - 1U;
			}

			auto clear(void) noexcept -> void {
				_seqs.clear();
				_tracks.clear();
				_patterns.clear();
			}

			auto play(const ml::u64 timeline) -> void {


				for (auto& t : _tracks) {

					t.play(_seqs, timeline);
				}
			}

	};


} // namespace mx

#endif // data_model_hpp
