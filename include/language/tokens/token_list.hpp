#ifndef language_tokens_list_hpp
#define language_tokens_list_hpp

#include "language/tokens/raw_iterator.hpp"
#include "language/tokens/map_iterator.hpp"
#include "language/tokens/syn_iterator.hpp"

#include "language/tokens/token_view.hpp"

#include "language/tokens.hpp"
#include <vector>


// -- T K  N A M E S P A C E --------------------------------------------------

namespace tk {


	// -- forward declarations ------------------------------------------------

	class tokens;


	// -- T O K E N S ---------------------------------------------------------

	class tokens final {


		// -- friends ---------------------------------------------------------

		/* raw_iterator as friend */
		friend class raw_iterator;

		/* map_iterator as friend */
		friend class map_iterator;

		/* syn_iterator as friend */
		friend class syn_iterator;

		/* const_raw_iterator as friend */
		friend class const_raw_iterator;

		/* const_map_iterator as friend */
		friend class const_map_iterator;

		/* const_syn_iterator as friend */
		friend class const_syn_iterator;


		/* syn_view as friend */
		friend class syn_view;

		/* const_syn_view as friend */
		friend class const_syn_view;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = tk::tokens;


			// -- private members ---------------------------------------------

			/* ranges */
			std::vector<tk::range> _ranges;





			/* map tokens */
			std::vector<mx::usz> _map;

			/* raw tokens */
			std::vector<tk::raw::token> _raw;

			/* syn tokens */
			std::vector<tk::syn::token> _syn;


			std::vector<tk::chunk> _chunks;
			std::vector<tk::token> _tokens;
			std::vector<mx::usz> _filtered;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			tokens(void) noexcept = default;


			// -- public methods ----------------------------------------------


			/* raw tokens */
			auto raw_tokens(void) noexcept
				-> tk::raw_iterator;

			/* const raw tokens */
			auto raw_tokens(void) const noexcept
				-> tk::const_raw_iterator;

			/* map tokens */
			auto map_tokens(void) noexcept
				-> tk::map_iterator;

			/* const map tokens */
			auto map_tokens(void) const noexcept
				-> tk::const_map_iterator;

			/* syn tokens iterator */
			auto syn_tokens(void) noexcept
				-> tk::syn_iterator;

			/* const syn tokens iterator */
			auto syn_tokens(void) const noexcept
				-> tk::const_syn_iterator;



			// -- public modifiers --------------------------------------------

			/* push raw token */
			auto push_raw_token(const tk::raw::token&) -> void;

			/* push map token */
			auto push_map_token(const tk::raw::token&) -> void;

			/* push syn token */
			auto push_syn_token(const tk::syn::token&) -> void;

			/* push syn token */
			auto push_syn_token(const tk::id,
								const tk::const_map_iterator&,
								const mx::usz = 1U) -> void;

			/* clear */
			auto clear(void) noexcept -> void {
				_map.clear();
				_raw.clear();
				_syn.clear();
			}


			// -- public accessors --------------------------------------------

			/* raw size */
			auto raw_size(void) const noexcept -> mx::usz {
				return _raw.size();
			}

			/* map size */
			auto map_size(void) const noexcept -> mx::usz {
				return _map.size();
			}

			/* syn size */
			auto syn_size(void) const noexcept -> mx::usz {
				return _syn.size();
			}


			// -- public methods ----------------------------------------------

			template <typename F, typename... Tp>
			inline auto for_each_filtered(F&& fn, Tp&&... args) const -> void {

				tk::token_view tv{_chunks.data()};

				for (const auto& i : _filtered) {
					tv._token = _tokens.data() + i;
					fn(tv, std::forward<Tp>(args)...);
				}
			}

			/* debug */
			auto debug(void) const -> void;
			auto debug_syn(void) const -> void;


			// -- public operators --------------------------------------------

			/* index operator */
			auto operator[](const mx::usz index) noexcept -> const tk::raw::token& {
				return _raw[index];
			}

	}; // class tokens

	inline auto test(void) -> void {
		tk::tokens tks;
		tks.for_each_filtered(
			[](const tk::token_view& tv) static -> void {
				tv.for_each_chunk(
					[](const tk::chunk& c) static -> void {
					}
				);
			}
		);
	}

} // namespace tk

#endif // language_tokens_list_hpp
