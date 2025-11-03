#ifndef language_syntax_param_view_hpp
#define language_syntax_param_view_hpp

#include "language/syntax/block_list.hpp"


// -- S X  N A M E S P A C E --------------------------------------------------

namespace sx {


	// -- P A R A M  V I E W --------------------------------------------------

	template <sx::is_block_list T>
	class param_view final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sx::param_view<T>;


			// -- private members ---------------------------------------------

			/* block list reference */
			const T*       _bl;

			/* param index */
			sx::size_type  _pi;


		public:

			// -- public lifecycle --------------------------------------------

			/* constructor */
			constexpr param_view(const T& bl,
								 const sx::size_type pi) noexcept
			: _bl{&bl}, _pi{pi} {
			}


			// -- public accessors --------------------------------------------

			/* action */
			constexpr auto action(void) const noexcept -> T::action_type {
				return _bl->param_actions[_pi];
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
					&_bl->param_aliases[_bl->param_offsets[_pi]],
					&_bl->param_aliases[_bl->param_offsets[_pi + 1U]]
				};
			}

			/* param not found */
			auto param_not_found(void) const noexcept -> bool {
				return (_pi == T::PARAM_NOT_FOUND);
			}


			// -- public operators --------------------------------------------

			/* equality operators */
			constexpr auto operator==(const size_type pi) const noexcept -> bool {
				return _pi == pi;
			}

	}; // class param_view

} // namespace sx

#endif // language_syntax_param_view_hpp
