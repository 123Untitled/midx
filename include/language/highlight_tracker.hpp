#ifndef language_highlight_tracker_hpp
#define language_highlight_tracker_hpp

#include "core/types.hpp"
#include "language/tokens/tokens.hpp"
#include "math.hpp"
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


			// -- private members ---------------------------------------------

			using map_type = std::unordered_map<mx::usz, const char*>;

			using mark_fn = auto (self::*)(const mx::usz, const char*) -> void;

			map_type _now;
			map_type _old;

			mark_fn _mark;

			/* tokens */
			const tk::tokens* _tokens;


			auto _mark_active_check(const mx::usz, const char*) -> void;
			auto _mark_active_force(const mx::usz, const char*) -> void;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			highlight_tracker(void) noexcept
			: _now{}, _old{}, _mark{&self::_mark_active_check},
			  _tokens{nullptr} {
			}

			/* destructor */
			~highlight_tracker(void) noexcept = default;


			// -- public methods ----------------------------------------------

			/* init
			   initialize the tracker with tokens reference */
			auto init(const tk::tokens& tokens) noexcept -> void {
				_tokens = &tokens;
				_now.clear();
				_old.clear();
				_mark = &self::_mark_active_check;
			}

			/* mark active
			   mark a highlight as active with expiration time */
			auto mark_active(const mx::usz, const char*) -> void;

			/* has changes
			   check if there are any changes recorded */
			auto has_changes(void) const noexcept -> bool;

			/* generate json
			   generate the JSON output for added and removed highlights */
			auto generate_json(void) -> std::string;


			/* swap now
			   swap current and old maps */
			auto swap_now(void) noexcept -> void {
				_old.clear();
				_old.swap(_now);
				_mark = &self::_mark_active_check;
			}

			/* clear
			   clear all highlights */
			auto clear(void) noexcept -> void {
				_now.clear();
			}


	}; // class highlight_tracker

} // namespace mx

#endif // language_highlight_tracker_hpp
