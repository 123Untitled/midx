#ifndef language_syntax_block_list_hpp
#define language_syntax_block_list_hpp

#include "literal.hpp"
#include "language/syntax/block.hpp"
#include "language/lexer/lexeme.hpp"
#include "macros.hpp"


// -- S X  N A M E S P A C E --------------------------------------------------

namespace sx {


	constexpr auto equal(const lx::lexeme& l, const char* a) noexcept -> bool {
		ml::usz len = 0U;
		while (a[len] != '\0')
			++len;

		if (len != l.size)
			return false;
		const char* p =  reinterpret_cast<const char*>(l.data);

		for (ml::usz i = 0U; i < len; ++i) {
			if (p[i] != a[i])
				return false;
		}
		return true;
	}

	// -- forward declarations ------------------------------------------------


	template <typename T,
			  size_type, size_type,
			  size_type, size_type>
	class block_list;


	namespace impl {

		/* is block_list false case */
		template <typename>
		struct is_block_list final {
			static constexpr bool value = false;
			non_instantiable_class(is_block_list);
		};

		/* is block_list true case */
		template <typename T, size_type NB, size_type NBA,
				size_type NP, size_type NPA>
		struct is_block_list<sx::block_list<T, NB, NBA, NP, NPA>> final {
			static constexpr bool value = true;
			non_instantiable_class(is_block_list);
		};
	}

	/* is_block_list */
	template <typename T>
	concept is_block_list = sx::impl::is_block_list<T>::value;


	template <sx::is_block_list>
	class block_iterator;

	template <sx::is_block_list>
	class param_iterator;

	template <sx::is_block_list>
	class block_view;

	template <sx::is_block_list>
	class param_view;


	// -- B L O C K  L I S T --------------------------------------------------

	template <typename T,
			  size_type NBLOCKS,
			  size_type NBLOCK_ALIASES,
			  size_type NPARAMS,
			  size_type NPARAM_ALIASES>
	class block_list final {


		// -- friends ---------------------------------------------------------

		template <sx::is_block_list>
		friend class sx::block_iterator;

		template <sx::is_block_list>
		friend class sx::param_iterator;

		template <sx::is_block_list>
		friend class sx::block_view;

		template <sx::is_block_list>
		friend class sx::param_view;



		public:

			// -- public constants --------------------------------------------

			/* block not found */
			static constexpr size_type BLOCK_NOT_FOUND = NBLOCKS;

			/* param not found */
			static constexpr size_type PARAM_NOT_FOUND = NPARAMS;


			using action_type = T;

		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sx::block_list<T,
										NBLOCKS,
										NBLOCK_ALIASES,
										NPARAMS,
										NPARAM_ALIASES>;


			// -- private members ---------------------------------------------

			/* aliases */
			const char* block_aliases[NBLOCK_ALIASES];
			const char* param_aliases[NPARAM_ALIASES];

			/* actions */
			T  block_actions[NBLOCKS];
			T  param_actions[NPARAMS];

			/* offsets */
			size_type   block_offsets[NBLOCKS + 1U];
			size_type   param_offsets[NPARAMS + 1U];
			size_type   level_offsets[NBLOCKS + 1U];


			// -- private methods ---------------------------------------------

			/* flatten */
			template <size_type NBA, size_type NP, size_type NPA>
			constexpr auto flatten(const sx::block<T, NBA, NP, NPA>& b,
								   size_type& b_alias_index,
								   size_type& b_level_index,
								   size_type& p_alias_index,
								   size_type& p_param_index) noexcept -> void {

				const sx::entry<T, NBA>&        spec = b.specifier;
				const sx::param_list<T, NP, NPA>& ps = b.params;

				// -- block level ---------------------------------------------

				block_offsets[b_level_index] = b_alias_index;

				for (size_type i = 0U; i < NBA; ++i) {
					block_aliases[b_alias_index] = spec.aliases[i];
					++b_alias_index;
				}

				block_actions[b_level_index] = spec.action;


				// end of block aliases
				block_offsets[b_level_index + 1U] = b_alias_index;


				// -- hierarchy level -----------------------------------------

				level_offsets[b_level_index] = p_param_index;
				++b_level_index;


				// -- param level ---------------------------------------------

				for (size_type i = 0U; i < NP; ++i)
					param_actions[p_param_index + i] = ps._actions[i];

				for (size_type i = 0U; i < NPA; ++i)
					param_aliases[p_alias_index + i] = ps._aliases[i];

				for (size_type i = 0U; i < NP + 1U; ++i)
					param_offsets[p_param_index + i] = ps._offsets[i] + p_alias_index;

				p_param_index += NP;
				p_alias_index += NPA;
			}


		public:

			// -- public lifecycle --------------------------------------------

			/* blocks constructor */
			template <size_type... NBA, size_type... NP, size_type... NPA>
			constexpr block_list(const sx::block<T, NBA, NP, NPA>&... blocks) noexcept
			: block_aliases{}, param_aliases{},
			  block_actions{}, param_actions{},
			  block_offsets{}, param_offsets{},
			  level_offsets{} {

				// -- flatten -------------------------------------------------

				size_type b_alias_index = 0U;
				size_type p_alias_index = 0U;
				size_type b_level_index = 0U;
				size_type p_param_index = 0U;

				((flatten<NBA, NP, NPA>(blocks,
									   b_alias_index,
									   b_level_index,
									   p_alias_index,
									   p_param_index)), ...);

				block_offsets[NBLOCKS] = b_alias_index;
				level_offsets[NBLOCKS] = p_param_index;
				param_offsets[NPARAMS] = p_alias_index;
			}



			// -- public iterators --------------------------------------------

			/* begin */
			constexpr auto begin(void) const noexcept -> sx::block_iterator<self> {
				return sx::block_iterator<self>{*this};
			}

			/* end */
			constexpr auto end(void) const noexcept -> sx::block_iterator<self> {
				return sx::block_iterator<self>{*this, NBLOCKS};
			}

			/* blocks */
			constexpr auto blocks(void) const noexcept -> const self& {
				return *this;
			}


			/* find block */
			constexpr auto find_block(const lx::lexeme& spec) const noexcept -> sx::block_view<self> {

				for (size_type bi = 0U; bi < NBLOCKS; ++bi) {

					const size_type bs = block_offsets[bi];
					const size_type be = block_offsets[bi + 1U];

					for (size_type i = bs; i < be; ++i) {
						if (sx::equal(spec, block_aliases[i]))
							return sx::block_view<self>{*this, bi};
					}
				}
				return sx::block_view<self>{*this, BLOCK_NOT_FOUND};
			}

			/* find block */
			template <ml::literal L>
			constexpr auto find_block(void) const noexcept -> sx::block_view<self> {

				for (size_type bi = 0U; bi < NBLOCKS; ++bi) {

					const size_type bs = block_offsets[bi];
					const size_type be = block_offsets[bi + 1U];

					for (size_type i = bs; i < be; ++i) {

						constexpr std::string_view v{L.data};

						if (v == block_aliases[i])
							return sx::block_view<self>{*this, bi};
					}
				}
				return sx::block_view<self>{*this, BLOCK_NOT_FOUND};
			}


	}; // class block_list


	// -- deduction guides ----------------------------------------------------

	template <typename T, size_type... NBA, size_type... NP, size_type... NPA>
	block_list(const sx::block<T, NBA, NP, NPA>&...) -> block_list<T,
																sizeof...(NBA),
																(NBA + ... ),
																(NP  + ... ),
																(NPA + ... )>;

} // namespace sx

#endif // language_syntax_block_list_hpp

			//static constexpr auto layout = sx::block_list {
			//
			//	// block trigger
			//	sx::block{
			//		sx::entry{&sx::builder::empty, "trig", "tr", "trigger"},
			//		sx::param_list{
			//			sx::entry{&sx::builder::empty, "sequence", "seq"},
			//			sx::entry{&sx::builder::empty, "repeat", "rpt"},
			//			sx::entry{&sx::builder::empty, "linked", "lnk"}
			//		}
			//	},
			//
			//	// block note
			//	sx::block{
			//		sx::entry{&sx::builder::empty, "note", "nt", "no"},
			//		sx::param_list{
			//			sx::entry{&sx::builder::empty, "sequence", "seq"},
			//			sx::entry{&sx::builder::empty, "repeat", "rpt"},
			//			sx::entry{&sx::builder::empty, "linked", "lnk"}
			//		}
			//	},
			//
			//
			//	// block gate
			//	sx::block{
			//		sx::entry{&sx::builder::empty, "gate", "ga"},
			//		sx::param_list{
			//			sx::entry{&sx::builder::empty, "sequence", "seq"},
			//			sx::entry{&sx::builder::empty, "repeat", "rpt"},
			//			sx::entry{&sx::builder::empty, "linked", "lnk"}
			//		}
			//	},
			//
			//	// block track
			//	sx::block{
			//		sx::entry{&sx::builder::empty, "track"},
			//		sx::param_list{
			//			sx::entry{&sx::builder::empty, "trig", "tr", "trigger"},
			//			sx::entry{&sx::builder::empty, "note", "nt", "no"},
			//			sx::entry{&sx::builder::empty, "gate", "ga"},
			//		}
			//	},
			//
			//	// block pattern
			//	sx::block{
			//		sx::entry{&sx::builder::empty, "pattern", "ptn"},
			//		sx::param_list{
			//			sx::entry{&sx::builder::empty, "track", "seq"}
			//		}
			//	}
			//};




			//
			//
			//auto debug(void) const -> void {
			//
			//	for (size_type bi = 0U; bi < NBLOCKS; ++bi) {
			//
			//		const size_type bs = block_offsets[bi];
			//		const size_type be = block_offsets[bi + 1U];
			//
			//		std::cout << "-- Block " << bi << " ";
			//
			//		for (size_type i = bs; i < be; ++i)
			//			std::cout << "'" << block_aliases[i] << "' ";
			//		std::cout << "\n";
			//
			//		const size_type ps = level_offsets[bi];
			//		const size_type pe = level_offsets[bi + 1U];
			//
			//
			//		for (size_type pi = ps; pi < pe; ++pi) {
			//
			//			const size_type pas = param_offsets[pi];
			//			const size_type pae = param_offsets[pi + 1U];
			//
			//			std::cout << "----- Param " << pi << " ";
			//
			//			for (size_type i = pas; i < pae; ++i)
			//				std::cout << "'" << param_aliases[i] << "' ";
			//			std::cout << "\n";
			//		}
			//	}
			//
			//
			//	std::cout << "NBLOCKS:        " << (unsigned)NBLOCKS        << "\n";
			//	std::cout << "NBLOCK_ALIASES: " << (unsigned)NBLOCK_ALIASES << "\n";
			//	std::cout << "NPARAMS:        " << (unsigned)NPARAMS        << "\n";
			//	std::cout << "NPARAM_ALIASES: " << (unsigned)NPARAM_ALIASES << "\n";
			//
			//	std::cout << "sizeof(block_list): " << sizeof(self) << " bytes\n";
			//
			//	std::cout << "size of param actions: " << sizeof(param_actions) << " bytes\n";
			//	std::cout << "size of param aliases: " << sizeof(param_aliases) << " bytes\n";
			//	std::cout << "size of block actions: " << sizeof(block_actions) << " bytes\n";
			//	std::cout << "size of block aliases: " << sizeof(block_aliases) << " bytes\n";
			//	std::cout << "size of block offsets: " << sizeof(block_offsets) << " bytes\n";
			//	std::cout << "size of level offsets: " << sizeof(level_offsets) << " bytes\n";
			//	std::cout << "size of param offsets: " << sizeof(param_offsets) << " bytes\n";
			//}
