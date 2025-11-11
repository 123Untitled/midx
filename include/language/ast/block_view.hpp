#ifndef language_ast_block_view_hpp
#define language_ast_block_view_hpp

#include "types.hpp"
#include "language/ast/param_iterator.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- forward declarations ------------------------------------------------

	class tree;
	class block;


	// -- B L O C K  V I E W --------------------------------------------------

	class block_view final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = as::block_view;


			// -- private members ---------------------------------------------

			/* tree reference */
			as::tree* _tree;

			/* block index */
			ml::usz _bi;


		public:

			// -- public lifecycle --------------------------------------------

			/* constructor */
			block_view(as::tree&, const ml::usz) noexcept;


			// -- public iterators --------------------------------------------

			/* begin */
			auto begin(void) const noexcept -> as::param_iterator;

			/* end */
			auto end(void) const noexcept -> as::param_iterator;


			// -- public accessors --------------------------------------------

			/* params */
			auto params(void) noexcept -> self&;

			/* block */
			auto block(void) noexcept -> as::block&;

			/* block index */
			auto bi(void) const noexcept -> mx::usz;

			/* num params */
			auto num_params(void) const noexcept -> mx::usz;

	}; // class block_view

} // namespace as

#endif // language_ast_block_view_hpp
