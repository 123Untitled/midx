#ifndef language_ast_param_view_hpp
#define language_ast_param_view_hpp

#include "types.hpp"
#include "language/ast/value_iterator.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- forward declarations ------------------------------------------------

	class tree;
	class param;


	// -- P A R A M  V I E W --------------------------------------------------

	class param_view final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = as::param_view;


			// -- private members ---------------------------------------------

			/* tree reference */
			const as::tree* _tree;

			/* param index */
			mx::usz _pi;


		public:

			// -- public lifecycle --------------------------------------------

			/* constructor */
			param_view(const as::tree&, const mx::usz) noexcept;


			// -- public iterators --------------------------------------------

			/* begin */
			auto begin(void) const noexcept -> as::value_iterator;

			/* end */
			auto end(void) const noexcept -> as::value_iterator;


			// -- public accessors --------------------------------------------

			/* values */
			auto values(void) const noexcept -> const self&;

			/* param */
			auto param(void) const noexcept -> const as::param&;

			/* num values */
			auto num_values(void) const noexcept -> mx::usz;

	}; // class param_view

} // namespace as

#endif // language_ast_param_view_hpp
