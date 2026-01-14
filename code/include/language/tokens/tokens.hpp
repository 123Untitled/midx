#ifndef language_tokens_list_hpp
#define language_tokens_list_hpp

#include "language/tokens/token_view.hpp"
#include "language/tokens/iterator.hpp"

#include <vector>

namespace lx {
	struct lexeme;
}

// -- T K  N A M E S P A C E --------------------------------------------------

namespace tk {


	// -- T O K E N S ---------------------------------------------------------

	class tokens final {


		// -- friends ---------------------------------------------------------

		/* iterator as friend */
		friend class iterator;

		/* const iterator as friend */
		friend class const_iterator;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = tk::tokens;


			// -- private members ---------------------------------------------

			/* chunks */
			std::vector<tk::chunk> _chunks;

			/* tokens */
			std::vector<tk::token> _tokens;

			/* filtered */
			std::vector<mx::usz> _filtered;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			tokens(void) noexcept = default;


			// -- public modifiers --------------------------------------------

			/* push token */
			template <typename... Tp>
			auto push_token(const tk::id id, const Tp&... chunks) -> void {

				static_assert(
						(sizeof...(Tp) == 0U) ||
						(std::is_same_v<Tp, tk::chunk> && ...)
						);

				const mx::usz start = _chunks.size();

				if constexpr (sizeof...(Tp) != 0U) {
					mx::usz i = start;
					_chunks.resize(i + sizeof...(Tp));
					((_chunks[i] = chunks, ++i), ...);
				}

				_tokens.emplace_back(id, start, sizeof...(Tp));
			}

			/* push filtered token */
			template <typename... Tp>
			auto push_filtered_token(const tk::id id, const Tp&... chunks) -> void {
				const auto index = _tokens.size();
				self::push_token(id, chunks...);
				_filtered.push_back(index);
			}

			/* push chunk */
			//auto push_chunk(const lx::lexeme&, const tk::range&) -> mx::usz;

			/* clear */
			auto clear(void) noexcept -> void {
				_filtered.clear();
				  _tokens.clear();
				  _chunks.clear();
			}


			// -- public accessors --------------------------------------------

			/* size */
			auto size(void) const noexcept -> mx::usz {
				return _tokens.size();
			}

			/* filtered size */
			auto filtered_size(void) const noexcept -> mx::usz {
				return _filtered.size();
			}

			/* filtered */
			auto filtered(void) noexcept -> tk::iterator {
				return tk::iterator{this};
			}

			/* const filtered */
			auto filtered(void) const noexcept -> tk::const_iterator {
				return tk::const_iterator{this};
			}


			/* view */
			auto filtered_view(const mx::usz index) const noexcept -> tk::const_token_view {
				return tk::const_token_view{_chunks.data(),
										   _tokens.data() + _filtered[index]};
			}


			// -- public methods ----------------------------------------------

			/* for each */
			template <typename F, typename... Tp>
			auto for_each(F&& fn, Tp&&... args) const -> void {

				tk::const_token_view tv{_chunks.data()};

				for (const auto& t : _tokens) {
					tv._token = &t;
					fn(tv, std::forward<Tp>(args)...);
				}
			}

			/* for each filtered */
			template <typename F, typename... Tp>
			inline auto for_each_filtered(F&& fn, Tp&&... args) const -> void {

				tk::const_token_view tv{_chunks.data()};

				for (const auto& i : _filtered) {
					tv._token = _tokens.data() + i;
					fn(tv, std::forward<Tp>(args)...);
				}
			}

			/* debug */
			auto debug(void) const -> void;

	}; // class tokens





} // namespace tk

#endif // language_tokens_list_hpp
