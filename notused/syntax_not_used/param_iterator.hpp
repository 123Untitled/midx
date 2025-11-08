#ifndef language_syntax_param_iterator_hpp
#define language_syntax_param_iterator_hpp

#include "language/syntax/block_list.hpp"
#include "language/syntax/param_view.hpp"


// -- S X  N A M E S P A C E --------------------------------------------------

namespace sx {


	// -- P A R A M  I T E R A T O R ------------------------------------------

	template <sx::is_block_list T>
	class param_iterator final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sx::param_iterator<T>;


			// -- private members ---------------------------------------------

			/* block list reference */
			const T* _bl;

			/* param index */
			sx::size_type _pi;


		public:

			// -- public lifecycle --------------------------------------------

			/* constructor */
			constexpr param_iterator(const T& bl,
									 const size_type pi = 0U) noexcept
			: _bl{&bl}, _pi{pi} {
			}


			// -- public operators --------------------------------------------

			/* dereference operator */
			constexpr auto operator*(void) const noexcept -> sx::param_view<T> {
				return sx::param_view<T>{*_bl, _pi};
			}

			/* increment operator */
			constexpr auto operator++(void) noexcept -> self& {
				++_pi;
				return *this;
			}

			/* equality operators */
			constexpr bool operator==(const self& other) const noexcept {
				return _pi == other._pi;
			}

			/* inequality operators */
			constexpr bool operator!=(const self& other) const noexcept {
				return _pi != other._pi;
			}

	}; // class param_iterator

} // namespace sx

#endif // language_syntax_param_iterator_hpp
