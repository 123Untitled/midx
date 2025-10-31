#ifndef language_syntax_block_view_hpp
#define language_syntax_block_view_hpp

#include "language/syntax/block_list.hpp"
#include "language/syntax/param_iterator.hpp"


// -- S X  N A M E S P A C E --------------------------------------------------

namespace sx {


	// -- B L O C K  V I E W --------------------------------------------------

	template <sx::is_block_list T>
	class block_view final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sx::block_view<T>;


			// -- private members ---------------------------------------------

			/* block list reference */
			const T*       _bl;

			/* block index */
			sx::size_type  _bi;


		public:

			// -- public lifecycle --------------------------------------------

			/* constructor */
			constexpr block_view(const T& bl, const sx::size_type bi) noexcept
			: _bl{&bl}, _bi{bi} {
			}


			// -- public iterators --------------------------------------------

			/* begin */
			constexpr auto begin(void) const noexcept -> sx::param_iterator<T> {
				return sx::param_iterator<T>{
					*_bl, _bl->level_offsets[_bi]
				};
			}

			/* end */
			constexpr auto end(void) const noexcept -> sx::param_iterator<T> {
				return sx::param_iterator<T>{
					*_bl, _bl->level_offsets[_bi + 1U]
				};
			}


			// -- public accessors --------------------------------------------

			/* action */
			constexpr auto action(void) const noexcept -> pr::action {
				return _bl->block_actions[_bi];
			}

			/* params */
			constexpr auto params(void) const noexcept -> const self& {
				return *this;
			}

			/* aliases */
			constexpr auto aliases(void) const noexcept -> auto {
				struct _ final {
					const char* const* b;
					const char* const* e;

					constexpr auto begin(void) const noexcept -> const char* const* {
						return b;
					}
					constexpr auto end(void) const noexcept -> const char* const* {
						return e;
					}
				};

				return _{
					&_bl->block_aliases[_bl->block_offsets[_bi]],
					&_bl->block_aliases[_bl->block_offsets[_bi + 1U]]
				};
			}

			/* block not found */
			constexpr auto block_not_found(void) const noexcept -> bool {
				return _bi == T::BLOCK_NOT_FOUND;
			}


			// -- public operators --------------------------------------------

			/* equality operators */
			constexpr auto operator==(const size_type bi) const noexcept -> bool {
				return _bi == bi;
			}


			// -- public methods ----------------------------------------------

			/* find param */
			constexpr auto find_param(const char* name) const noexcept -> sx::param_view<T> {

				const size_type ps = _bl->level_offsets[_bi];
				const size_type pe = _bl->level_offsets[_bi + 1U];

				for (size_type pi = ps; pi < pe; ++pi) {

					const size_type pas = _bl->param_offsets[pi];
					const size_type pae = _bl->param_offsets[pi + 1U];

					for (size_type i = pas; i < pae; ++i) {

						if (sx::equal(_bl->param_aliases[i], name))
							return sx::param_view<T>{*_bl, pi};
					}
				}
				return sx::param_view<T>{*_bl, T::PARAM_NOT_FOUND};
			}

	}; // class block_view

} // namespace sx

#endif // language_syntax_block_view_hpp
