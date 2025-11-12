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
			as::tree* _tree;

			/* block index */
			mx::usz _bi;


		public:

			// -- public lifecycle --------------------------------------------

			/* constructor */
			block_iterator(as::tree& tree,
						   const mx::usz bi = 0U) noexcept;


			// -- public operators --------------------------------------------

			/* dereference operator */
			auto operator*(void) const noexcept -> as::block_view;

			/* pre-increment operator */
			auto operator++(void) noexcept -> self&;

			/* equality operators */
			auto operator==(const self&) const noexcept -> bool;

			/* inequality operators */
			auto operator!=(const self&) const noexcept -> bool;

	}; // class block_iterator


} // namespace as

#endif // language_ast_block_iterator_hpp
