#ifndef ml_language_keyword_library_hpp
#define ml_language_keyword_library_hpp

#include "types.hpp"
#include <string_view>

#include <iostream>

#include "language/state_driver.hpp"


// -- K W  N A M E S P A C E --------------------------------------------------

namespace kw {


	// -- P A R A M  E N T R Y ------------------------------------------------

	class param_entry final {


		public:

			/* name */
			std::string_view name;

			/* action */
			sd::action_type action;

			/* deleted default constructor */
			//param_entry(void) noexcept = delete;
			consteval param_entry(void) noexcept
			: name{}, action{nullptr} {
			}

			constexpr param_entry(const char* n, const sd::action_type a) noexcept
			: name{n}, action{a} {
			}
	};




	// -- B L O C K  E N T R Y ------------------------------------------------

	template <unsigned N>
	class block_entry final {

		public:


			std::string_view name;

			sd::action_type action;

			kw::param_entry labels[N];

			consteval block_entry(void) noexcept
			: name{}, action{nullptr}, labels{} {
			}

			template <typename... Tp>
			consteval block_entry(const char* n,
								  const sd::action_type& a,
								  const kw::param_entry& first,
								  const Tp&... rest) noexcept
			: name{n}, action{a}, labels{first, rest...} {
			}
	};


	// -- deduction guides ----------------------------------------------------

	template <typename... Tp>
	block_entry(const char*,
				const sd::action_type&,
				const kw::param_entry&,
				const Tp&...) -> block_entry<1U + sizeof...(Tp)>;



	// -- K E Y W O R D  L I B R A R Y ----------------------------------------

	template <unsigned... Ns>
	class keyword_library final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = kw::keyword_library<Ns...>;


			// -- private constants -------------------------------------------

			/* block not found */
			static constexpr ml::u32 BLOCK_NOT_FOUND = sizeof...(Ns);

			/* param not found */
			static constexpr ml::u32 PARAM_NOT_FOUND = (Ns + ... );



			// -- block range -------------------------------------------------

			class block_range final {

				public:
					std::string_view name;
					sd::action_type action;
					ml::usz start;
					ml::usz end;
			};


			// -- private members ---------------------------------------------

			/* blocks */
			self::block_range _blocks[sizeof...(Ns)];

			/* params */
			kw::param_entry _params[(Ns + ... )];


			// -- private methods ---------------------------------------------


			template <unsigned N>
			consteval auto _fill(const kw::block_entry<N>& block, ml::u32& offset, ml::u32& bi) noexcept -> void {

				_blocks[bi].name   = block.name;
				_blocks[bi].action = block.action;
				_blocks[bi].start  = offset;
				_blocks[bi].end    = offset + N;

				for (ml::u32 pi = 0U; pi < N; ++pi)
					_params[offset + pi] = block.labels[pi];

				offset += N;
				++bi;
			}

		public:

			// -- public lifecycle --------------------------------------------

			// constructor
			consteval keyword_library(const kw::block_entry<Ns>&... blocks) noexcept
			/* uninitialized */ {

				ml::u32 offset = 0U;
				ml::u32 bi     = 0U;

				// fold on each block
				((self::_fill<Ns>(blocks, offset, bi)), ...);
			}


			/* block index */
			constexpr auto block_index(const std::string_view& name) const noexcept -> ml::u32 {
				for (ml::u32 i = 0U; i < sizeof...(Ns); ++i) {
					if (_blocks[i].name == name)
						return i;
				} return BLOCK_NOT_FOUND;
			}

			/* param index */
			constexpr auto param_index(const ml::u32 bi, const std::string_view& name) const noexcept -> ml::u32 {

				const auto& block = _blocks[bi];

				for (ml::usz i = block.start; i < block.end; ++i) {
					if (_params[i].name == name)
						return i;
				} return PARAM_NOT_FOUND;
			}

			/* block not found */
			constexpr auto block_not_found(void) const noexcept -> ml::u32 {
				return BLOCK_NOT_FOUND;
			}

			/* param not found */
			constexpr auto param_not_found(void) const noexcept -> ml::u32 {
				return PARAM_NOT_FOUND;
			}

			/* block action */
			constexpr auto block_action(const ml::u32 bi) const noexcept -> sd::action_type {
				return _blocks[bi].action;
			}

			/* param action */
			constexpr auto param_action(const ml::u32 pi) const noexcept -> sd::action_type {
				return _params[pi].action;
			}

			/* block action */
			constexpr auto block_action(const ml::u32 bi, ml::state_driver& driver) const -> void {
				(driver.*_blocks[bi].action)();
			}

			/* param action */
			constexpr auto param_action(const ml::u32 pi, ml::state_driver& driver) const -> void {
				(driver.*_params[pi].action)();
			}
			


			auto debug(void) const -> void {
				for (ml::usz i = 0U; i < sizeof...(Ns); ++i) {
					std::cout << " - " << _blocks[i].name << "\n";
					for (ml::usz j = _blocks[i].start; j < _blocks[i].end; ++j) {
						std::cout << "   - " << _params[j].name << "\n";
					}
				}
			}


	}; // class keyword_library


///* keywords */
//static constexpr kw::keyword_library _keywords {
//
//
//	kw::block_entry{"note",   nullptr,
//		kw::param_entry{"seq", nullptr},
//		kw::param_entry{"restart", nullptr}
//	},
//
//	kw::block_entry{"octave",   nullptr,
//		kw::param_entry{"seq", nullptr},
//		kw::param_entry{"restart", nullptr}
//	},
//
//	kw::block_entry{"track",   nullptr,
//		kw::param_entry{"note", nullptr}
//	},
//
//	kw::block_entry{"track",     nullptr,
//		kw::param_entry{"note",  nullptr},
//		kw::param_entry{"gate",  nullptr}
//	},
//	kw::block_entry{"pattern",   nullptr,
//		kw::param_entry{"track", nullptr}
//	}
//};


} // namespace kw

#endif // ml_language_keyword_library_hpp
