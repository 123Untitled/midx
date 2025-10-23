#ifndef ml_language_keyword_library_hpp
#define ml_language_keyword_library_hpp

#include "types.hpp"
#include <unordered_map>
#include <string_view>

#include <iostream>


// -- K W  N A M E S P A C E --------------------------------------------------

namespace kw {


	template <typename T>
	class label_entry final {


		public:

			/* action type */
			using action_type = auto (T::*)(void) -> void;

			/* name */
			std::string_view name;

			/* action */
			action_type action;

			/* deleted default constructor */
			//label_entry(void) noexcept = delete;
			consteval label_entry(void) noexcept
			: name{}, action{nullptr} {
			}

			constexpr label_entry(const char* n, const action_type a) noexcept
			: name{n}, action{a} {
			}
	};


	// -- B L O C K  E N T R Y ------------------------------------------------

	template <typename T, unsigned N>
	class block_entry final {

		public:

			/* action type */
			using action_type = auto (T::*)(void) -> void;

			std::string_view name;

			action_type action;

			kw::label_entry<T> labels[N];

			consteval block_entry(void) noexcept
			: name{}, action{nullptr}, labels{} {
			}

			template <typename... Tp>
			consteval block_entry(const char* n,
								  const action_type& a,
								  const label_entry<T>& first,
								  const Tp&... rest) noexcept
			: name{n}, action{a}, labels{first, rest...} {
			}
	};


	// deduction guide
	template <typename T, typename... Tp>
	block_entry(const char*,
				auto (T::*)(void) -> void,
				const kw::label_entry<T>&,
				const Tp&...) -> block_entry<T, 1U + sizeof...(Tp)>;

} // namespace kw


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- K E Y W O R D  L I B R A R Y ----------------------------------------

	template <typename T, unsigned... Ns>
	class keyword_library final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::keyword_library<T, Ns...>;

			/* action type */
			using action_type = auto (T::*)(void) -> void;


			// -- block range -------------------------------------------------

			class block_range final {

				public:
					std::string_view name;
					action_type action;
					ml::usz start;
					ml::usz end;
			};


			// -- private members ---------------------------------------------

			/* blocks */
			block_range _blocks[sizeof...(Ns)];

			/* labels */
			kw::label_entry<T> _labels[(Ns + ... )];

			/* cursor */
			mutable ml::usz _cursor;



			// -- private methods ---------------------------------------------

			template <unsigned N>
			static consteval auto _has_duplicate(const kw::block_entry<T, N>& block) noexcept -> bool {
				//for (ml::usz i = 0U; i < N; ++i) {
				//	for (ml::usz j = i + 1U; j < N; ++j) {
				//		if (block.labels[i].name
				//		 == block.labels[j].name)
				//			return true; }
				//} return false;
				return false;
			}

			//template <unsigned... Np>
			consteval auto _has_duplicate(const kw::block_entry<T, Ns>&... blocks) noexcept -> bool {
				const std::string_view names[] = {blocks.name...};
				for (ml::usz i = 0U; i < sizeof...(Ns); ++i) {
					for (ml::usz j = i + 1U; j < sizeof...(Ns); ++j) {
						if (names[i] == names[j])
							return true; }
				}
				return 0;

				//return (self::_has_duplicate<Ns>(blocks) || ...);
			}

			template <unsigned N>
			consteval auto _fill(const kw::block_entry<T, N>& block, ml::usz& offset, ml::usz& bi) noexcept -> void {

				_blocks[bi].name   = block.name;
				_blocks[bi].action = block.action;
				_blocks[bi].start  = offset;
				_blocks[bi].end    = offset + N;

				for (ml::usz i = 0U; i < N; ++i)
					_labels[offset + i] = block.labels[i];

				offset += N;
				++bi;
			}

		public:

			// -- public lifecycle --------------------------------------------

			// constructor
			consteval keyword_library(const kw::block_entry<T, Ns>&... blocks) noexcept
			:/* uninitialized */ _cursor{sizeof...(Ns)} {

				// not implemented yet
				//static_assert(self::_has_duplicate(blocks...) == false,
				//			  "duplicate block names in keyword library");

				ml::usz offset = 0U;
				ml::usz bi     = 0U;

				// fold on each block
				((self::_fill<Ns>(blocks, offset, bi)), ...);
			}


			/* on block */
			auto on_block(const std::string_view& name, T& instance) const -> bool {
				for (_cursor = 0U; _cursor < sizeof...(Ns); ++_cursor) {
					if (_blocks[_cursor].name == name) {
						(instance.*_blocks[_cursor].action)();
						return true;
					}
				} return false;
			}

			/* on label */
			auto on_label(const std::string_view& name, T& instance) const -> bool {

				const auto& block = _blocks[_cursor];

				for (ml::usz i = block.start; i < block.end; ++i) {
					if (_labels[i].name == name) {
						(instance.*_labels[i].action)();
						return true;
					}
				}
				return false;
			}

			auto debug(void) const -> void {
				for (ml::usz i = 0U; i < sizeof...(Ns); ++i) {
					std::cout << " - " << _blocks[i].name << "\n";
					for (ml::usz j = _blocks[i].start; j < _blocks[i].end; ++j) {
						std::cout << "   - " << _labels[j].name << "\n";
					}
				}
			}


	}; // class keyword_library

} // namespace ml

#endif // ml_language_keyword_library_hpp
