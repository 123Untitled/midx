#ifndef language_tokens_syn_view_hpp
#define language_tokens_syn_view_hpp

#include "core/types.hpp"
#include "language/tokens/map_iterator.hpp"


// -- T K  N A M E S P A C E --------------------------------------------------

namespace tk {


	// -- forward declarations ------------------------------------------------

	/* tokens */
	class tokens;

	/* syn token */
	namespace syn {
		struct token;
	} // namespace syn


	// -- S Y N  V I E W  -----------------------------------------------------

	class syn_view final {


		// -- friends ---------------------------------------------------------

		/* syn_iterator as friend */
		friend class syn_iterator;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = tk::syn_view;


			// -- private members ---------------------------------------------

			/* tokens pointer */
			tk::tokens* _tokens;

			/* index */
			mx::usz _index;


			// -- private lifecycle -------------------------------------------

			/* constructor */
			syn_view(tk::tokens&, const mx::usz) noexcept;


		public:

			// -- public iterators --------------------------------------------

			/* begin */
			auto begin(void) const noexcept -> tk::map_iterator;

			/* end */
			auto end(void) const noexcept -> tk::map_iterator;


			// -- public accessors --------------------------------------------

			/* token */
			auto token(void) const noexcept -> tk::syn::token&;

	}; // class syn_view


	// -- C O N S T  S Y N  V I E W  ------------------------------------------

	class const_syn_view final {


		// -- friends ---------------------------------------------------------

		/* const_syn_iterator as friend */
		friend class const_syn_iterator;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = tk::const_syn_view;


			// -- private members ---------------------------------------------

			/* tokens pointer */
			const tk::tokens* _tokens;

			/* index */
			mx::usz _index;


			// -- private lifecycle -------------------------------------------

			/* constructor */
			const_syn_view(const tk::tokens&, const mx::usz) noexcept;


		public:

			// -- public iterators --------------------------------------------

			/* begin */
			auto begin(void) const noexcept -> tk::const_map_iterator;

			/* end */
			auto end(void) const noexcept -> tk::const_map_iterator;


			// -- public accessors --------------------------------------------

			/* token */
			auto token(void) const noexcept -> const tk::syn::token&;

	}; // class const_syn_view

} // namespace tk

#endif // language_tokens_syn_view_hpp
