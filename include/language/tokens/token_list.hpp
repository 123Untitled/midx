#ifndef language_tokens_list_hpp
#define language_tokens_list_hpp

#include "byte_range.hpp"
#include "language/tokens.hpp"
#include <vector>


// -- T K  N A M E S P A C E --------------------------------------------------

namespace tk {


	// -- forward declarations ------------------------------------------------

	/* iterator */
	class iterator;


	// -- L I S T -------------------------------------------------------------

	class token_list final {


		// -- friends ---------------------------------------------------------

		/* iterator as friend */
		friend class tk::iterator;


		private:


			// -- private members ---------------------------------------------

			/* tokens */
			std::vector<tk::token> _tokens;

			/* parse indices */
			std::vector<mx::usz> _parse_indices;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			token_list(void) noexcept = default;


			// -- public methods ----------------------------------------------

			/* begin */
			auto begin(void) noexcept -> tk::iterator;

			/* end */
			auto end(void) noexcept -> tk::iterator;

			/* for each */
			template <typename F, typename... Tp>
			auto for_each(F&& f, Tp&&... args) const -> void {
				for (mx::usz i = 0U; i < _tokens.size(); ++i)
					f(_tokens[i], std::forward<Tp>(args)...);
			}

			/* for each parse */
			template <typename F, typename... Tp>
			auto for_each_parse(F&& f, Tp&&... args) const -> void {
				for (mx::usz i = 0U; i < _parse_indices.size(); ++i)
					f(_tokens[_parse_indices[i]], std::forward<Tp>(args)...);
			}


			// -- public modifiers --------------------------------------------

			/* push token */
			auto push_token(const tk::token&) -> void;

			/* push parse token */
			auto push_parse_token(const tk::token&) -> void;

			/* clear */
			auto clear(void) noexcept -> void {
				_tokens.clear();
				_parse_indices.clear();
			}


			// -- public accessors --------------------------------------------

			/* size */
			auto size(void) const noexcept -> mx::usz {
				return _tokens.size();
			}

			/* parsable size */
			auto parsable_size(void) const noexcept -> mx::usz {
				return _parse_indices.size();
			}

			/* tokens */
			auto tokens(void) noexcept -> const std::vector<tk::token>& {
				return _tokens;
			}

			/* debug */
			auto debug(const mx::byte_range&) -> void;


			// -- public operators --------------------------------------------

			/* index operator */
			auto operator[](const mx::usz index) noexcept -> tk::token& {
				return _tokens[index];
			}

	}; // class token_list


	// -- I T E R A T O R -----------------------------------------------------

	class iterator final {


		private:

			// -- private members ---------------------------------------------

			/* list */
			tk::token_list* _list;

			/* index */
			mx::usz _index;


		public:

			// -- public lifecycle --------------------------------------------

			/* constructor */
			iterator(tk::token_list& list, const mx::usz index = 0U) noexcept
			: _list{&list}, _index{index} {
			}


			// -- public operators --------------------------------------------

			/* pre-increment operator */
			auto operator++(void) noexcept -> tk::iterator& {
				++_index;
				return *this;
			}

			/* post-increment operator */
			auto operator++(int) noexcept -> tk::iterator {
				tk::iterator temp{*this};
				++_index;
				return temp;
			}

			/* dereference operator */
			auto operator*(void) const noexcept -> tk::token& {
				return _list->_tokens[_list->_parse_indices[_index]];
			}

			/* equality operator */
			auto operator==(const tk::iterator& other) const noexcept -> bool {
				return _index == other._index;
			}

			/* inequality operator */
			auto operator!=(const tk::iterator& other) const noexcept -> bool {
				return _index != other._index;
			}

	}; // class iterator

} // namespace tk

#endif // language_tokens_list_hpp
