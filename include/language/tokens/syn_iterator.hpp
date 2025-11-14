#ifndef language_tokens_syn_iterator_hpp
#define language_tokens_syn_iterator_hpp

#include "core/types.hpp"
#include "language/tokens/syn_view.hpp"


// -- T K  N A M E S P A C E --------------------------------------------------

namespace tk {


	// -- forward declarations ------------------------------------------------

	/* tokens */
	class tokens;


	// -- S Y N  I T E R A T O R  ---------------------------------------------

	class syn_iterator final {


		// -- friends ---------------------------------------------------------

		/* tokens as friend */
		friend class tk::tokens;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = tk::syn_iterator;


			// -- private members ---------------------------------------------

			/* tokens pointer */
			tk::tokens* _tokens;

			/* index */
			mx::usz _index;


			// -- private lifecycle -------------------------------------------

			/* base constructor */
			syn_iterator(tk::tokens&) noexcept;

			/* constructor */
			syn_iterator(tk::tokens&, const mx::usz) noexcept;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			syn_iterator(void) noexcept;


			// -- public operators --------------------------------------------

			/* pre-increment operator */
			auto operator++(void) noexcept -> self&;

			/* post-increment operator */
			auto operator++(int) noexcept -> self;


			/* const dereference operator */
			auto operator*(void) const noexcept -> tk::syn_view;


			/* equality operator */
			auto operator==(const self&) const noexcept -> bool;

			/* inequality operator */
			auto operator!=(const self&) const noexcept -> bool;


			/* const [] operator */
			auto operator[](const mx::isz) const noexcept -> tk::syn_view;


			// -- public iterators --------------------------------------------

			/* const begin */
			auto begin(void) const noexcept -> self;

			/* const end */
			auto end(void) const noexcept -> self;

	}; // class syn_iterator


	// -- C O N S T  S Y N  I T E R A T O R  ----------------------------------

	class const_syn_iterator final {


		// -- friends ---------------------------------------------------------

		/* tokens as friend */
		friend class tk::tokens;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = tk::const_syn_iterator;


			// -- private members ---------------------------------------------

			/* tokens pointer */
			const tk::tokens* _tokens;

			/* index */
			mx::usz _index;


			// -- private lifecycle -------------------------------------------

			/* base constructor */
			const_syn_iterator(const tk::tokens&) noexcept;

			/* constructor */
			const_syn_iterator(const tk::tokens&, const mx::usz) noexcept;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			const_syn_iterator(void) noexcept;


			// -- public operators --------------------------------------------

			/* pre-increment operator */
			auto operator++(void) noexcept -> self&;

			/* post-increment operator */
			auto operator++(int) noexcept -> self;


			/* const dereference operator */
			auto operator*(void) const noexcept -> tk::const_syn_view;


			/* equality operator */
			auto operator==(const self&) const noexcept -> bool;

			/* inequality operator */
			auto operator!=(const self&) const noexcept -> bool;


			/* const [] operator */
			auto operator[](const mx::isz) const noexcept -> tk::const_syn_view;


			// -- public iterators --------------------------------------------

			/* const begin */
			auto begin(void) const noexcept -> self;

			/* const end */
			auto end(void) const noexcept -> self;

	}; // class const_syn_iterator

} // namespace tk

#endif // language_tokens_syn_iterator_hpp
