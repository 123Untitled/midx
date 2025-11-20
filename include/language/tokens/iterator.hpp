#ifndef language_tokens_iterator_hpp
#define language_tokens_iterator_hpp

#include "core/types.hpp"
#include "language/tokens/token_view.hpp"


// -- T K  N A M E S P A C E --------------------------------------------------

namespace tk {


	// -- forward declarations ------------------------------------------------

	class tokens;


	// -- I T E R A T O R -----------------------------------------------------

	class iterator final {


		// -- friends ---------------------------------------------------------

		/* tokens as friend */
		friend class tokens;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = tk::iterator;


			// -- private members ---------------------------------------------

			/* tokens pointer */
			tk::tokens* _tokens;

			/* current index */
			mx::usz _index;


			// -- private lifecycle -------------------------------------------

			/* tokens constructor */
			explicit iterator(tk::tokens*) noexcept;

			/* end constructor */
			iterator(tk::tokens*, const mx::usz) noexcept;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			iterator(void) noexcept;


			// -- public operators --------------------------------------------

			/* dereference operator */
			auto operator*(void) const noexcept -> tk::token_view;


			/* pre-increment operator */
			auto operator++(void) noexcept -> self&;

			/* post-increment operator */
			auto operator++(int) noexcept -> self;

			/* pre-decrement operator */
			auto operator--(void) noexcept -> self&;

			/* post-decrement operator */
			auto operator--(int) noexcept -> self;

			auto operator-(const mx::isz offset) const noexcept -> self {
				self it = *this;
				it._index -= offset;
				return it;
			}


			/* equality operator */
			auto operator==(const self&) const noexcept -> bool;

			/* inequality operator */
			auto operator!=(const self&) const noexcept -> bool;


			// -- public iterators --------------------------------------------

			/* begin */
			auto begin(void) const noexcept -> self;

			/* end */
			auto end(void) const noexcept -> self;


			// -- public accessors --------------------------------------------

			/* token */
			auto token(void) const noexcept -> tk::token&;

			/* token at */
			auto token_at(const mx::isz) const noexcept -> tk::token&;

			/* view */
			auto view(void) const noexcept -> tk::token_view;

			/* is end */
			auto is_end(void) const noexcept -> bool;

			/* index */
			auto index(void) const noexcept -> mx::usz;

	}; // class iterator


	// -- C O N S T  I T E R A T O R ------------------------------------------

	class const_iterator final {


		// -- friends ---------------------------------------------------------

		/* tokens as friend */
		friend class tokens;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = tk::const_iterator;


			// -- private members ---------------------------------------------

			/* tokens pointer */
			const tk::tokens* _tokens;

			/* current index */
			mx::usz _index;


			// -- private lifecycle -------------------------------------------

			/* tokens constructor */
			explicit const_iterator(const tk::tokens*) noexcept;

			/* end constructor */
			const_iterator(const tk::tokens*, const mx::usz) noexcept;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			const_iterator(void) noexcept;


			// -- public operators --------------------------------------------

			/* dereference operator */
			auto operator*(void) const noexcept -> tk::const_token_view;


			/* pre-increment operator */
			auto operator++(void) noexcept -> self&;

			/* post-increment operator */
			auto operator++(int) noexcept -> self;


			/* equality operator */
			auto operator==(const self&) const noexcept -> bool;

			/* inequality operator */
			auto operator!=(const self&) const noexcept -> bool;


			// -- public iterators --------------------------------------------

			/* begin */
			auto begin(void) const noexcept -> self;

			/* end */
			auto end(void) const noexcept -> self;


			// -- public accessors --------------------------------------------

			/* token */
			auto token(void) const noexcept -> const tk::token&;

			/* token at */
			auto token_at(const mx::isz) const noexcept -> const tk::token&;

			/* view */
			auto view(void) const noexcept -> tk::const_token_view;

			/* is end */
			auto is_end(void) const noexcept -> bool;

			/* index */
			auto index(void) const noexcept -> mx::usz;

	}; // class iterator

} // namespace tk

#endif // language_tokens_iterator_hpp
