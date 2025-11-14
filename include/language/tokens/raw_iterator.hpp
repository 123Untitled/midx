#ifndef language_tokens_raw_iterator_hpp
#define language_tokens_raw_iterator_hpp

#include "core/types.hpp"


// -- T K  N A M E S P A C E --------------------------------------------------

namespace tk {


	// -- forward declarations ------------------------------------------------

	/* tokens */
	class tokens;

	namespace raw {
		struct token;
	} // namespace raw


	// -- R A W  I T E R A T O R  ---------------------------------------------

	class raw_iterator final {


		// -- friends ---------------------------------------------------------

		/* tokens as friend */
		friend class tk::tokens;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = tk::raw_iterator;


			// -- private members ---------------------------------------------

			/* tokens pointer */
			tk::tokens* _tokens;

			/* index */
			mx::usz _index;


			// -- private lifecycle -------------------------------------------

			/* base constructor */
			raw_iterator(tk::tokens&) noexcept;

			/* constructor */
			raw_iterator(tk::tokens&, const mx::usz) noexcept;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			raw_iterator(void) noexcept;


			// -- public operators --------------------------------------------

			/* pre-increment operator */
			auto operator++(void) noexcept -> self&;

			/* post-increment operator */
			auto operator++(int) noexcept -> self;


			/* const dereference operator */
			auto operator*(void) const noexcept -> tk::raw::token&;


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

	}; // class raw_iterator


	// -- C O N S T  R A W  I T E R A T O R  ----------------------------------

	class const_raw_iterator final {


		// -- friends ---------------------------------------------------------

		/* tokens as friend */
		friend class tk::tokens;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = tk::const_raw_iterator;


			// -- private members ---------------------------------------------

			/* tokens pointer */
			const tk::tokens* _tokens;

			/* index */
			mx::usz _index;


			// -- private lifecycle -------------------------------------------

			/* base constructor */
			const_raw_iterator(const tk::tokens&) noexcept;

			/* constructor */
			const_raw_iterator(const tk::tokens&, const mx::usz) noexcept;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			const_raw_iterator(void) noexcept;


			// -- public operators --------------------------------------------

			/* pre-increment operator */
			auto operator++(void) noexcept -> self&;

			/* post-increment operator */
			auto operator++(int) noexcept -> self;


			/* const dereference operator */
			auto operator*(void) const noexcept -> const tk::raw::token&;


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

			/* const token */
			auto token(void) const noexcept -> const tk::raw::token&;

	}; // class const_raw_iterator

} // namespace tk

#endif // language_tokens_raw_iterator_hpp
