#ifndef language_tokens_map_iterator_hpp
#define language_tokens_map_iterator_hpp

#include "core/types.hpp"


// -- T K  N A M E S P A C E --------------------------------------------------

namespace tk {


	// -- forward declarations ------------------------------------------------

	/* tokens */
	class tokens;

	namespace raw {
		struct token;
	} // namespace raw


	// -- M A P  I T E R A T O R  ---------------------------------------------

	class map_iterator final {


		// -- friends ---------------------------------------------------------

		/* tokens as friend */
		friend class tk::tokens;

		/* syn_view as friend */
		friend class syn_view;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = tk::map_iterator;


			// -- private members ---------------------------------------------

			/* tokens pointer */
			tk::tokens* _tokens;

			/* index */
			mx::usz _index;


			// -- private lifecycle -------------------------------------------

			/* base constructor */
			map_iterator(tk::tokens&) noexcept;

			/* constructor */
			map_iterator(tk::tokens&, const mx::usz) noexcept;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			map_iterator(void) noexcept;


			// -- public operators --------------------------------------------

			/* pre-increment operator */
			auto operator++(void) noexcept -> self&;

			/* post-increment operator */
			auto operator++(int) noexcept -> self;


			/* dereference operator */
			auto operator*(void) const noexcept -> tk::raw::token&;

			/* arrow operator */
			auto operator->(void) const noexcept -> tk::raw::token*;


			/* equality operator */
			auto operator==(const self&) const noexcept -> bool;

			/* inequality operator */
			auto operator!=(const self&) const noexcept -> bool;


			/* const [] operator */
			auto operator[](const mx::isz) const noexcept -> tk::raw::token&;


			// -- public iterators --------------------------------------------

			/* const begin */
			auto begin(void) const noexcept -> self;

			/* const end */
			auto end(void) const noexcept -> self;


			// -- public accessors --------------------------------------------

			/* const token */
			auto token(void) const noexcept -> tk::raw::token&;

	}; // class map_iterator


	// -- C O N S T  M A P  I T E R A T O R  ----------------------------------

	class const_map_iterator final {


		// -- friends ---------------------------------------------------------

		/* tokens as friend */
		friend class tk::tokens;

		/* const_syn_view as friend */
		friend class const_syn_view;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = tk::const_map_iterator;


			// -- private members ---------------------------------------------

			/* tokens pointer */
			const tk::tokens* _tokens;

			/* index */
			mx::usz _index;


			// -- private lifecycle -------------------------------------------

			/* base constructor */
			const_map_iterator(const tk::tokens&) noexcept;

			/* constructor */
			const_map_iterator(const tk::tokens&, const mx::usz) noexcept;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			const_map_iterator(void) noexcept;


			// -- public operators --------------------------------------------

			/* pre-increment operator */
			auto operator++(void) noexcept -> self&;

			/* post-increment operator */
			auto operator++(int) noexcept -> self;


			/* dereference operator */
			auto operator*(void) const noexcept -> const tk::raw::token&;

			/* arrow operator */
			auto operator->(void) const noexcept -> const tk::raw::token*;


			/* equality operator */
			auto operator==(const self&) const noexcept -> bool;

			/* inequality operator */
			auto operator!=(const self&) const noexcept -> bool;


			/* const [] operator */
			auto operator[](const mx::isz) const noexcept -> const tk::raw::token&;


			// -- public iterators --------------------------------------------

			/* const begin */
			auto begin(void) const noexcept -> self;

			/* const end */
			auto end(void) const noexcept -> self;


			// -- public accessors --------------------------------------------

			/* token */
			auto token(void) const noexcept -> const tk::raw::token&;

			/* index */
			auto index(void) const noexcept -> mx::usz;

	}; // class const_map_iterator

} // namespace tk

#endif // language_tokens_map_iterator_hpp
