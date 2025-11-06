#ifndef language_ast_value_view_hpp
#define language_ast_value_view_hpp

#include "types.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- forward declarations ------------------------------------------------

	class tree;
	class value;


	// -- V A L U E  V I E W --------------------------------------------------

	class value_view final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = as::value_view;


			// -- private members ---------------------------------------------

			/* tree reference */
			const as::tree* _tree;

			/* value index */
			ml::usz _vi;


		public:

			// -- public lifecycle --------------------------------------------

			/* constructor */
			value_view(const as::tree&, const ml::usz) noexcept;


			// -- public accessors --------------------------------------------

			/* value */
			auto value(void) const noexcept -> const as::value&;

	}; // class value_view

} // namespace as

#endif // language_ast_value_view_hpp
