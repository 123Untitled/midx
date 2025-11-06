#ifndef language_ast_value_iterator_hpp
#define language_ast_value_iterator_hpp

#include "language/ast/value_view.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- forward declarations ------------------------------------------------

	class tree;
	class value;


	// -- V A L U E  I T E R A T O R ------------------------------------------

	class value_iterator final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = as::value_iterator;


			// -- private members ---------------------------------------------

			/* tree reference */
			const as::tree* _tree;

			/* value index */
			ml::usz _vi;


		public:

			// -- public lifecycle --------------------------------------------

			/* constructor */
			value_iterator(const as::tree&, const ml::usz = 0U) noexcept;


			// -- public operators --------------------------------------------

			/* dereference operator */
			auto operator*(void) const noexcept -> as::value_view;

			/* pre-increment operator */
			auto operator++(void) noexcept -> self&;

			/* equality operators */
			auto operator==(const self&) const noexcept -> bool;

			/* inequality operators */
			auto operator!=(const self&) const noexcept -> bool;

	}; // class value_iterator

} // namespace as

#endif // language_ast_value_iterator_hpp
