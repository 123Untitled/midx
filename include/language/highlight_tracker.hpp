#ifndef language_highlight_tracker_hpp
#define language_highlight_tracker_hpp

#include "core/types.hpp"
#include "language/tokens/tokens.hpp"
#include "math.hpp"
#include <unordered_map>
#include <string>
#include <set>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- H I G H L I G H T  T R A C K E R ------------------------------------

	class highlight_tracker final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::highlight_tracker;


			class highlight final {

				private:
					using self = highlight;
				public:
					mx::frac expire;
					mx::usz token_index;

				highlight(void) noexcept = default;

				highlight(const mx::frac& exp, const mx::usz index) noexcept
				: expire{exp}, token_index{index} {
				}

				auto operator<(const self& other) const noexcept -> bool {
					//if (expire != other.expire)
					//	return expire < other.expire;
					//return token_index < other.token_index;
					// optimized
					return (expire < other.expire)
						|| (expire == other.expire && token_index < other.token_index);
				}
			};


			// -- private members ---------------------------------------------

			using set_type = std::set<highlight>;
			using map_type = std::unordered_map<mx::usz, const highlight*>;


			/* sorted indices */
			set_type _set;

			/* token indices */
			map_type _map;

			/* tokens */
			const tk::tokens* _tokens;


			class light final {
				public:
					mx::usz token_index;
					const char* group;
			};

			/* added */
			std::vector<light> _added;

			/* removed */
			std::vector<mx::usz> _removed;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			highlight_tracker(void) noexcept
			: _set{},
			  _map{},
			  _tokens{nullptr} {
			}

			/* destructor */
			~highlight_tracker(void) noexcept = default;


			// -- public methods ----------------------------------------------

			/* init
			   initialize the tracker with tokens reference */
			auto init(const tk::tokens& tokens) noexcept -> void {
				_tokens = &tokens;
			}

			/* mark active
			   mark a highlight as active with expiration time */
			auto mark_active(const mx::usz, const char*, const mx::frac&) -> void;

			auto update(const mx::frac&) -> void;

			/* has changes
			   check if there are any changes recorded */
			auto has_changes(void) const noexcept -> bool;

			/* generate json
			   generate the JSON output for added and removed highlights */
			auto generate_json(void) -> std::string;


			/* clear output
			   clear the accumulated JSON output buffer */
			auto clear(void) noexcept -> void {
				_removed.clear();
				  _added.clear();
			}

			/* reset
			   reset the tracker */
			auto reset(void) noexcept -> void {
				_removed.clear();
				  _added.clear();
					_map.clear();
					_set.clear();
			}


			// -- public accessors --------------------------------------------

			/* is initialized */
			auto is_initialized(void) const noexcept -> bool {
				return _tokens != nullptr;
			}


	}; // class highlight_tracker

} // namespace mx

#endif // language_highlight_tracker_hpp
