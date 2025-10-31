#ifndef language_syntax_block_iterator_hpp
#define language_syntax_block_iterator_hpp

#include "language/syntax/block_list.hpp"
#include "language/syntax/block_view.hpp"


// -- S X  N A M E S P A C E --------------------------------------------------

namespace sx {


	// -- B L O C K  I T E R A T O R ------------------------------------------

	template <sx::is_block_list T>
	class block_iterator final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sx::block_iterator<T>;


			// -- private members ---------------------------------------------

			/* block list reference */
			const T*       _bl;

			/* block index */
			sx::size_type  _bi;


		public:

			// -- public lifecycle --------------------------------------------

			/* constructor */
			constexpr block_iterator(const T& bl,
									 const sx::size_type bi = 0U) noexcept
			: _bl{&bl}, _bi{bi} {
			}


			// -- public operators --------------------------------------------

			/* dereference operator */
			constexpr auto operator*(void) const noexcept -> sx::block_view<T> {
				return sx::block_view<T>{*_bl, _bi};
			}

			/* increment operator */
			constexpr auto operator++(void) noexcept -> self& {
				++_bi;
				return *this;
			}

			/* equality operators */
			constexpr auto operator==(const self& other) const noexcept -> bool {
				return _bi == other._bi;
			}

			/* inequality operators */
			constexpr auto operator!=(const self& other) const noexcept -> bool {
				return _bi != other._bi;
			}

	}; // class block_iterator

} // namespace sx

#endif // language_syntax_block_iterator_hpp
