#ifndef midx_alternate_screen_hpp
#define midx_alternate_screen_hpp


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- A L T E R N A T E  S C R E E N --------------------------------------

	class alternate_screen final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::alternate_screen;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			alternate_screen(void);

			/* copy constructor */
			alternate_screen(const self&) noexcept = default;

			/* move constructor */
			alternate_screen(self&&) noexcept = default;

			/* destructor */
			~alternate_screen(void) noexcept;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;

	}; // class alternate_screen_buffer

} // namespace mx

#endif // midx_alternate_screen_buffer_hpp
