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

			using tr_seq = ml::sequence<TR_DEFAULT>;
			using nt_seq = ml::sequence<NT_DEFAULT>;
			using ga_seq = ml::sequence<GA_DEFAULT>;
			using ch_seq = ml::sequence<CH_DEFAULT>;
			using vl_seq = ml::sequence<VL_DEFAULT>;
			using oc_seq = ml::sequence<OC_DEFAULT>;
			using se_seq = ml::sequence<SE_DEFAULT>;
			using pr_seq = ml::sequence<PR_DEFAULT>;


			// -- private members ---------------------------------------------

			/* trigs */
			std::vector<tr_seq> _trs;

			/* notes */
			std::vector<nt_seq> _nts;

			/* gates */
			std::vector<ga_seq> _gas;

			/* channels */
			std::vector<ch_seq> _chs;

			/* velocities */
			std::vector<vl_seq> _vls;

			/* octaves */
			std::vector<oc_seq> _ocs;

			/* semitones */
			std::vector<se_seq> _ses;

			/* probabilities */
			std::vector<pr_seq> _prs;



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


			auto play(void) -> void {


				for (const auto& t : _tracks) {

					auto& tr = _trs[t.tr_index];
					auto& nt = _nts[t.nt_index];
					auto& gt = _gas[t.ga_index];
					auto& ch = _chs[t.ch_index];
					auto& vl = _vls[t.vl_index];
					auto& oc = _ocs[t.oc_index];
					auto& se = _ses[t.se_index];
					auto& pr = _prs[t.pr_index];
				}
			}

	};


} // namespace mx

#endif // data_model_hpp
