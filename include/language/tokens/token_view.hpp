#ifndef language_tokens_token_view_hpp
#define language_tokens_token_view_hpp

#include "core/types.hpp"
#include "language/tokens.hpp"


// -- T K  N A M E S P A C E --------------------------------------------------

namespace tk {


	// -- T O K E N  V I E W --------------------------------------------------

	class token_view final {


		// -- friends ---------------------------------------------------------

		/* tokens as friend */
		friend class tk::tokens;


		private:

			// -- private members ---------------------------------------------

			/* token pointer */
			const tk::token* _token;

			/* chunks pointer */
			const tk::chunk* _chunks;


			// -- private lifecycle -------------------------------------------

			/* chunks constructor */
			token_view(const tk::chunk*) noexcept;


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			token_view(void) = delete;


			// -- public accessors --------------------------------------------

			/* token id */
			auto id(void) const noexcept -> tk::id;


			// -- public methods ----------------------------------------------

			/* for each chunk */
			template <typename F, typename... Tp>
			inline auto for_each_chunk(F&& fn, Tp&&... args) const -> void {

				mx::usz i = _token->start;
				do {
					const tk::chunk& c = _chunks[i];
					fn(c, std::forward<Tp>(args)...);
					i = c.next;
				} while (i != 0U);
			}

	}; // class token_view

} // namespace tk

#endif // language_tokens_token_view_hpp
