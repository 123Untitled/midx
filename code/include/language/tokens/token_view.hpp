#ifndef language_tokens_token_view_hpp
#define language_tokens_token_view_hpp

#include "core/types.hpp"
#include "language/tokens/def.hpp"



// -- T K  N A M E S P A C E --------------------------------------------------

namespace tk {


	// -- forward declarations ------------------------------------------------

	class const_token_view;


	// -- T O K E N  V I E W --------------------------------------------------

	class token_view final {


		// -- friends ---------------------------------------------------------

		/* tokens as friend */
		friend class tokens;

		/* iterator as friend */
		friend class iterator;


		private:

			// -- private members ---------------------------------------------

			/* chunks pointer */
			tk::chunk* _chunks;

			/* token pointer */
			tk::token* _token;



			// -- private lifecycle -------------------------------------------

			/* chunks constructor */
			token_view(tk::chunk*, tk::token* = nullptr) noexcept;


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			//token_view(void) = delete;
			token_view(void)
			: _chunks{nullptr}, _token{nullptr} {
			}


			/* make invalid */
			auto make_invalid(void) const noexcept -> void {
				_token->id = tk::invalid;
			}


			// -- public accessors --------------------------------------------

			/* token id */
			auto id(void) const noexcept -> tk::id;

			/* first chunk */
			auto first_chunk(void) const noexcept -> tk::chunk&;

			/* last chunk */
			auto last_chunk(void) const noexcept -> tk::chunk&;

			/* number of chunks */
			auto num_chunks(void) const noexcept -> mx::usz;


			// -- conversion operators ----------------------------------------

			/* to const token view */
			operator const_token_view(void) const noexcept;


			// -- public methods ----------------------------------------------

			/* for each chunk */
			template <typename F, typename... Tp>
			inline auto for_each_chunk(F&& fn, Tp&&... args) const -> void {

				mx::usz i = _token->start;
				const mx::usz end = i + _token->count;
				for (; i < end; ++i)
					fn(_chunks[i], std::forward<Tp>(args)...);
			}

	}; // class token_view


	// -- C O N S T  T O K E N  V I E W ---------------------------------------

	class const_token_view final {


		// -- friends ---------------------------------------------------------

		/* tokens as friend */
		friend class tokens;

		/* iterator as friend */
		friend class const_iterator;

		/* token view as friend */
		friend class token_view;


		private:

			// -- private members ---------------------------------------------

			/* chunks pointer */
			const tk::chunk* _chunks;

			/* token pointer */
			const tk::token* _token;



			// -- private lifecycle -------------------------------------------

			/* chunks constructor */
			const_token_view(const tk::chunk*, const tk::token* = nullptr) noexcept;


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			//const_token_view(void) = delete;
			const_token_view(void)
			: _chunks{nullptr}, _token{nullptr} {
			}


			// -- public accessors --------------------------------------------

			/* token id */
			auto id(void) const noexcept -> tk::id;

			/* first chunk */
			auto first_chunk(void) const noexcept -> const tk::chunk&;

			/* last chunk */
			auto last_chunk(void) const noexcept -> const tk::chunk&;

			/* number of chunks */
			auto num_chunks(void) const noexcept -> mx::usz;


			// -- public methods ----------------------------------------------

			/* for each chunk */
			template <typename F, typename... Tp>
			inline auto for_each_chunk(F&& fn, Tp&&... args) const -> void {

				mx::usz i = _token->start;
				const mx::usz end = i + _token->count;
				for (; i < end; ++i) {
					fn(_chunks[i], std::forward<Tp>(args)...);
				}
			}

			auto is_null(void) const noexcept -> bool {
				return _token == nullptr || _chunks == nullptr;
			}

	}; // class const_token_view

} // namespace tk


/* ostream operator */
auto operator<<(std::ostream&, const tk::const_token_view&) -> std::ostream&;

#endif // language_tokens_token_view_hpp
