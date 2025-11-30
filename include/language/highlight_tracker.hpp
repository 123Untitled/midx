#ifndef language_highlight_tracker_hpp
#define language_highlight_tracker_hpp

#include "core/types.hpp"
#include "language/tokens/tokens.hpp"
#include <unordered_map>
#include <string>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- H I G H L I G H T  T R A C K E R ------------------------------------

	class highlight_tracker final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::highlight_tracker;

			/* highlight entry */
			struct entry final {
				mx::usz token_idx;
				const char* group;

				entry(void) noexcept
				: token_idx{0U}, group{nullptr} {
				}

				entry(const mx::usz idx, const char* g) noexcept
				: token_idx{idx}, group{g} {
				}
			};

			/* map type */
			using map_type = std::unordered_map<mx::usz, entry>;


			// -- private members ---------------------------------------------

			/* current active highlights */
			map_type _active;

			/* pending highlights for this frame */
			map_type _pending;

			/* tokens reference */
			const tk::tokens* _tokens;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			highlight_tracker(void) noexcept
			: _active{}, _pending{}, _tokens{nullptr} {
			}

			/* destructor */
			~highlight_tracker(void) noexcept = default;


			// -- public methods ----------------------------------------------

			/* init
			   initialize the tracker with tokens reference */
			auto init(const tk::tokens& tokens) noexcept -> void {
				_tokens = &tokens;
			}

			/* begin frame
			   prepare for new evaluation frame */
			auto begin_frame(void) noexcept -> void {
				_pending.clear();
			}

			/* mark active
			   mark a highlight as active for this frame */
			auto mark_active(const mx::usz hash,
							 const mx::usz token_idx,
							 const char* group) -> void {
				_pending.emplace(hash, entry{token_idx, group});
			}

			/* end frame
			   finalize frame and generate differential JSON
			   returns true if changes occurred */
			auto end_frame(std::string& out) -> bool;


			// -- public accessors --------------------------------------------

			/* is initialized */
			auto is_initialized(void) const noexcept -> bool {
				return _tokens != nullptr;
			}


	}; // class highlight_tracker

} // namespace mx

#endif // language_highlight_tracker_hpp
