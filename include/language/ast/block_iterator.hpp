#ifndef language_ast_block_iterator_hpp
#define language_ast_block_iterator_hpp

#include "types.hpp"
#include "language/ast/block_view.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- forward declarations ------------------------------------------------

	class tree;


	// -- B L O C K  I T E R A T O R ------------------------------------------

	class block_iterator final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = as::block_iterator;


			// -- private members ---------------------------------------------

			/* tree reference */
			const as::tree* _tree;

			/* block index */
			ml::usz _bi;


		public:

			// -- public lifecycle --------------------------------------------

			/* constructor */
			block_iterator(const as::tree& tree,
						   const ml::usz bi = 0U) noexcept
			: _tree{&tree}, _bi{bi} {
			}


			// -- public operators --------------------------------------------

			/* dereference operator */
			auto operator*(void) const noexcept -> as::block_view {
				return as::block_view{*_tree, _bi};
			}

			/* pre-increment operator */
			auto operator++(void) noexcept -> self& {
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


} // namespace as

#endif // language_ast_block_iterator_hpp
