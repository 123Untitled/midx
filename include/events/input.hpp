#ifndef events_input_hpp
#define events_input_hpp

#include "escapes/bracketed_paste.hpp"
#include "escapes/mouse.hpp"
#include "core/types.hpp"
#include "terminal/tty.hpp"
#include "monitoring/watcher.hpp"

#include <vector>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	enum class token_type {

		ctrl_a,
		ctrl_b,
		ctrl_c,
		ctrl_d,
		ctrl_e,
		alt_f,

		arrow_up,
		arrow_down,
		arrow_right,
		arrow_left,

		escape,
		tab,
		shift_tab,
		enter,

		control, // arrows, control, alt, shift
		printable,

		characters,
		clipboard,

	}; // enum class token


	//struct token final {
	//
	//	token_type type;
	//	mx::variant<char, std::string_view> value;
	//}; // struct token


	class monitor;


	// -- I N P U T -----------------------------------------------------------

	class input final : public mx::watcher {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::input;


			// -- private constants -------------------------------------------

			/* buffer size */
			static constexpr auto _buffer_size = 1024U;


			// -- private members ---------------------------------------------

			/* tty */
			mx::tty _tty;

			/* clipboard */
			mx::bracketed_paste _bp;

			/* mouse */
			mx::mouse _mouse;

			/* buffer */
			std::vector<mx::u8> _buffer;


		public:

			// -- public lifecycle --------------------------------------------

			/* monitor constructor */
			input(const mx::monitor&);


			// -- public accessors --------------------------------------------

			/* buffer */
			auto buffer(void) const noexcept -> const std::vector<mx::u8>& {
				return _buffer;
			}


			// -- public overrides --------------------------------------------

			/* on event */
			auto on_event(mx::application&, const struct ::kevent&) -> void override final;

			/* ident */
			auto ident(void) const noexcept -> int override final;


		private:

			enum class char_type {
				c_char,
				c_arrow_up,
				c_arrow_down,
				c_arrow_right,
				c_arrow_left,
				c_escape,
				c_tab,
				c_shift_tab,
				c_delete,
				c_ctrl_c,
				c_ctrl_d,
				c_ctrl_l,
				c_ctrl_u,
			};

	}; // class input

} // namespace mx

#endif // events_input_hpp
