#ifndef escapes_bracketed_paste_hpp
#define escapes_bracketed_paste_hpp


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- B R A C K E T E D  P A S T E ----------------------------------------

	class bracketed_paste final {


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			bracketed_paste(void);

			/* copy constructor */
			bracketed_paste(const bracketed_paste&) noexcept = default;

			/* move constructor */
			bracketed_paste(bracketed_paste&&) noexcept = default;

			/* destructor */
			~bracketed_paste(void) noexcept;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const bracketed_paste&) noexcept -> bracketed_paste& = default;

			/* move assignment operator */
			auto operator=(bracketed_paste&&) noexcept -> bracketed_paste& = default;

	}; // class bracketed_paste

} // namespace mx

#endif // escapes_bracketed_paste_hpp
