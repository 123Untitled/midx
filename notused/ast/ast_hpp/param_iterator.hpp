#ifndef language_ast_param_iterator_hpp
#define language_ast_param_iterator_hpp

#include "types.hpp"
#include "language/ast/param_view.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- forward declarations ------------------------------------------------

	class tree;


	// -- P A R A M  I T E R A T O R ------------------------------------------

	class param_iterator final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = as::param_iterator;


			// -- private members ---------------------------------------------

			/* tree reference */
			const as::tree* _tree;

			/* param index */
			mx::usz _pi;


		public:

			// -- public lifecycle --------------------------------------------

			/* constructor */
			param_iterator(const as::tree&, const mx::usz = 0U) noexcept;


			// -- public operators --------------------------------------------

			/* dereference operator */
			auto operator*(void) const noexcept -> as::param_view;

			/* pre-increment operator */
			auto operator++(void) noexcept -> self&;

			/* equality operators */
			auto operator==(const self&) const noexcept -> bool;

			/* inequality operators */
			auto operator!=(const self&) const noexcept -> bool;

	}; // class param_iterator

} // namespace ast

#endif // language_ast_param_iterator_hpp
