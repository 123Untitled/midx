#ifndef language_syntax_block_list_hpp
#define language_syntax_block_list_hpp

#include "language/syntax/block.hpp"

#include <iostream>


// -- S X  N A M E S P A C E --------------------------------------------------

namespace sx {


	static constexpr auto strlen(const char* s) noexcept -> ::size_t {
		if (s == nullptr)
			return 0U;
		::size_t len = 0U;
		for (; *s != '\0'; ++s)
			++len;
		return len;
	}


	template <typename...>
	class block_list;


	// -- B L O C K  L I S T --------------------------------------------------

	template <size_type... NBAs, size_type... NPs, size_type... NPAs>
	class block_list<sx::block<NBAs, NPs, NPAs>...> final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sx::block_list<sx::block<NBAs, NPs, NPAs>...>;


			template <size_type N>
			static constexpr auto next_prime(void) noexcept -> size_type {

				constexpr auto is_prime =
				[](const size_type n) static noexcept -> bool {
					if (n < 2U) return false;
					for (size_type i = 2U; i * i <= n; ++i)
						if (n % i == 0U)
							return false;
					return true;
				};

				size_type candidate = N;
				while (is_prime(candidate) == false)
					++candidate;
				return candidate;
			}

		public:

			static constexpr auto print_param_primes(void) noexcept -> void {
				((std::cout << "next_prime<" << NPAs << "> = "
							<< self::next_prime<NPAs>() << "\n"), ...);
			}


			template <size_type... NS>
			static constexpr auto total_primes(void) noexcept -> size_type {
				size_type total = 0U;
				((total += self::next_prime<NS>()), ...);
				return total;
			}


			struct weights final {
				size_type letter[26U];
				constexpr weights(void) noexcept
				: letter{} {
				}

				constexpr auto print(void) const noexcept -> void {
					std::cout << "weights: ";
					for (size_type i = 0U; i < 26U; ++i)
						std::cout << letter[i] << " ";
					std::cout << "\n";
				}
				constexpr auto operator[](const size_type i) const noexcept -> size_type {
					return letter[i];
				}
			};



		public:

			static constexpr auto hash(const char* s, const self::weights& w) noexcept -> ::uint32_t {
				const auto len = sx::strlen(s);
				::uint32_t h = len;
				::uint32_t p = 0U;

				for (::size_t i = 0U; i < len; ++i) {
					const auto c = static_cast<::uint8_t>(s[i]);
					const auto ci = ((c - 'a')) % 26U;
					h = (h * 33U) ^ w.letter[ci]; // VERY GOOD DON'T REMOVE !

					//h = (h * 33U) ^ (w[ci] ^ (i << 1));
					//p += c * p;
				}
				return h;// + p;
			}




		private:

			static constexpr size_t permute(size_t g, size_t N) noexcept {
				return (g * 2654435761U) % N; // Knuth multiplicative hash
			}

			template <size_type N>
			struct g_data final {
				size_type seq[N];
				int     score[N];

				constexpr g_data(void) noexcept
				: seq{}, score{} {
					for (size_type g = 0U; g < N; ++g) {
						seq[g]   = self::permute(g, N);
						score[g] = 0;
					}
				}

				constexpr auto sort_by_score(void) noexcept -> void {
					for (size_type i = 1U; i < N; ++i) {
						const int s = score[i];
						const size_type g   = seq[i];
						size_type j = i;
						// decroissant order
						while (j > 0U && score[j - 1U] < s) {
							score[j] = score[j - 1U];
							seq[j]   = seq[j - 1U];
							--j;
						}
						score[j] = s;
						seq[j]   = g;
					}
				}
				constexpr auto print(void) const noexcept -> void {
					std::cout << "  seq: ";
					for (size_type i = 0U; i < N; ++i)
						std::cout << seq[i] << " ";
					std::cout << "\n";
					std::cout << "score: ";
					for (size_type i = 0U; i < N; ++i)
						std::cout << score[i] << " ";
					std::cout << "\n";
				}
			};


			// -- private static methods --------------------------------------

			template <size_type P>
			static constexpr auto collision(const char* const* aliases,
											const size_type partial_count,
											const self::weights& w) noexcept -> bool {
				bool used[P] {false};

				for (size_type i = 0U; i < partial_count; ++i) {

					const auto h = self::hash(
							aliases[i], w) % P;
					//std::cout << "hash('" << aliases[i] << "') = " << h << "\n";
					// MAYBE POSITION IN SET DEPENDENT HASHING

					if (used[h])
						return true;

					used[h] = true;
				}
				return false;
			}




			template <size_type N, size_type P>
			static constexpr auto solve_weights(self::g_data<P>& gs,
												const char* const* aliases,
												const size_type i,
												self::weights& w) noexcept -> bool {

				// all slots assigned
				if (i == N)
					return true;

				// loop over shifts
				for (size_type gi = 0U; gi < P; ++gi) {

					const auto g = gs.seq[gi];

					for (const char* a = aliases[i]; *a != '\0'; ++a) {

						// todo: case insensitive
						const size_type ci = (static_cast<::uint8_t>(*a) - 'a') % 26U;
						w.letter[ci] = (w.letter[ci] + g) % P;

						if (!collision<P>(aliases, i + 1U, w)) {
							++gs.score[gi];

							if (solve_weights<N, P>(gs, aliases, i + 1U, w))
								return true;
						}
						else
							--gs.score[gi];

						// rollback change
						w.letter[ci] = (w.letter[ci] + N - g) % P;
					}

					gs.sort_by_score();
					//gs.print();
				}
				//w.print();
				return false;
			}




			template <size_type N, typename Compare>
			static constexpr auto insertion_sort(const char* (&arr)[N], const Compare& cmp) noexcept -> void {
				for (::size_t i = 1U; i < N; ++i) {
					const char* key = arr[i];
					::size_t j = i;
					while (j > 0U && cmp(key, arr[j - 1U])) {
						arr[j] = arr[j - 1];
						--j;
					}
					arr[j] = key;
				}
			}

			template <size_type N>
			static constexpr auto lexicographic_sort(const char* const* aliases,
													 const char* (&sorted)[N]) noexcept -> void {

				for (size_type i = 0U; i < N; ++i)
					sorted[i] = aliases[i];

				self::insertion_sort<N>(sorted,
					[](const char* a, const char* b) constexpr static -> bool {
						return std::string_view(a) < std::string_view(b);
					});
			}

			template <size_type N, size_type P>
			static constexpr auto perfect_seed(const char* const* aliases) noexcept -> self::weights {

				const char* sorted[N];

				self::lexicographic_sort<N>(aliases, sorted);

				self::weights w;

				self::g_data<P> gs;


				bool solved = self::solve_weights<N, P>(gs, sorted, 0U, w);
				if consteval {
					//constexpr bool solved = solve_weights(sorted, count, 0U, w);
					//static_assert(solved, "unable to find perfect hash seed");
				}
				if (!solved) {
					;
					std::cerr << "perfect hash seed \x1b[31mnot found\x1b[0m ";
				}
				else {
					;
					std::cout << "perfect hash seed \x1b[32mfound\x1b[0m ";
				}
				for (size_type i = 0U; i < N; ++i)
					std::cout << "'" << sorted[i] << "' ";
				std::cout << "\n";
				w.print();

				return w;
			}



			/* flatten */
			template <size_type NBA, size_type NP, size_type NPA>
			constexpr auto flatten(const sx::block<NBA, NP, NPA>& b,
								   size_type& b_alias_index,
								   size_type& b_level_index,
								   size_type& p_alias_index,
								   size_type& p_param_index) noexcept -> void {

				const sx::entry<NBA>&        spec = b.specifier;
				const sx::param_list<NP, NPA>& ps = b.params;

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

			// -- public constants --------------------------------------------

			static constexpr size_type NBLOCKS         = sizeof...(NBAs);
			static constexpr size_type NBLOCK_ALIASES  = (NBAs + ... );
			static constexpr size_type NPARAMS         = (NPs  + ... );
			static constexpr size_type NPARAM_ALIASES  = (NPAs + ... );
			static constexpr size_type BLOCK_NOT_FOUND = NBLOCKS;
			static constexpr size_type PARAM_NOT_FOUND = NPARAMS;

			static constexpr size_type HASH_BLOCK_SIZE = self::next_prime<NBLOCK_ALIASES>();
			static constexpr size_type HASH_PARAM_SIZE = self::total_primes<NPAs...>();


			// -- public members ----------------------------------------------

			/* blocks level */
			const char*     block_aliases[NBLOCK_ALIASES];
			sd::action_type block_actions[NBLOCKS];

			/* params level */
			const char*     param_aliases[NPARAM_ALIASES];
			sd::action_type param_actions[NPARAMS];

			/* offsets */
			size_type        block_offsets[NBLOCKS + 1U];
			size_type        level_offsets[NBLOCKS + 1U];
			size_type        param_offsets[NPARAMS + 1U];

			/* hash remaps */
			size_type        block_remaps[HASH_BLOCK_SIZE];
			size_type        hash_to_block[HASH_BLOCK_SIZE];

			size_type        hash_to_param[HASH_PARAM_SIZE];
			size_type        param_remaps[HASH_PARAM_SIZE];


			/* weights */
			self::weights   block_weights;
			self::weights   param_weights[NBLOCKS];

			/* primes */
			size_type        primes[NBLOCKS];
			size_type        prime_offsets[NBLOCKS];


			template <size_type NPA, size_type P>
			constexpr auto compute_weights(const size_type bi) noexcept -> void {

				const size_type ps = level_offsets[bi];
				const size_type pe = level_offsets[bi + 1U];
				const size_type as = param_offsets[ps];
				const size_type ae = param_offsets[pe];

				param_weights[bi] = self::perfect_seed<NPA, P>(
						param_aliases + as);


				// param remap calculation
				for (size_type pi = ps; pi < pe; ++pi) {

					const size_type pas = param_offsets[pi];
					const size_type pae = param_offsets[pi + 1U];

					for (size_type i = pas; i < pae; ++i) {

						const auto h = self::hash(param_aliases[i],
												 param_weights[bi]) % P;

						const size_type o = prime_offsets[bi] + h;

						param_remaps[o]  = i;
						hash_to_param[o] = pi;
					}
				}

				// block remap calculation
				const size_type bs = block_offsets[bi];
				const size_type be = block_offsets[bi + 1U];

				for (size_type i = bs; i < be; ++i) {
					const auto h = self::hash(block_aliases[i],
												block_weights)
											% HASH_BLOCK_SIZE;
					block_remaps[h]  = i;
					hash_to_block[h] = bi;
				}
			}



			//template <size_type... NBA, size_type... NP, size_type... NPA>
			constexpr block_list(const sx::block<NBAs, NPs, NPAs>&... blocks) noexcept
			: block_aliases{},
			  block_actions{},
			  param_aliases{},
			  param_actions{},
			  block_offsets{},
			  level_offsets{},
			  param_offsets{},
			  block_remaps{},
			  hash_to_block{},
			  hash_to_param{},
			  param_remaps{},
			  block_weights{},
			  param_weights{},
			  primes{self::next_prime<NPAs>()...},
			  prime_offsets{}
			{

				// init hash buffers
				for (size_type i = 0U; i < HASH_BLOCK_SIZE; ++i)
					block_remaps[i]  = HASH_BLOCK_SIZE;
				for (size_type i = 0U; i < HASH_PARAM_SIZE; ++i)
					param_remaps[i]  = HASH_PARAM_SIZE;

				// -- flatten -------------------------------------------------

				size_type b_alias_index = 0U;
				size_type b_level_index = 0U;
				size_type p_alias_index = 0U;
				size_type p_param_index = 0U;

				((flatten<NBAs, NPs, NPAs>(blocks,
									   b_alias_index,
									   b_level_index,
									   p_alias_index,
									   p_param_index)), ...);

				block_offsets[NBLOCKS] = b_alias_index;
				level_offsets[NBLOCKS] = p_param_index;
				param_offsets[NPARAMS] = p_alias_index;


				size_type acc = 0;
				for (size_type i = 0; i < NBLOCKS; ++i) {
					prime_offsets[i] = acc;
					acc += primes[i];
				}

				// block weights calculation
				block_weights = self::perfect_seed<NBLOCK_ALIASES, HASH_BLOCK_SIZE>(
						block_aliases
				);


				size_type bi = 0U;
				((self::compute_weights<NPAs,
				  self::next_prime<NPAs>()
				  >(bi), ++bi), ...);



			}


			constexpr auto block_index(const char* name) const noexcept -> size_type {

				const auto i = self::hash(name, block_weights) % HASH_BLOCK_SIZE;

				const auto a =  block_remaps[i];

				if (a == HASH_BLOCK_SIZE)
					return BLOCK_NOT_FOUND;

				std::string_view alias = block_aliases[a];

				if (alias == name) {
				const auto b = hash_to_block[i];
					return b;
				}

				return BLOCK_NOT_FOUND;
			}

			constexpr auto has_param(const size_type bi,
									 const char* name) const noexcept -> bool {

				if (bi >= NBLOCKS)
					return false;

				const auto h = self::hash(name, param_weights[bi]) % primes[bi];
				const auto i = prime_offsets[bi] + h;
				const auto a  = param_remaps[i];

				if (a == HASH_PARAM_SIZE)
					return false;

				const auto p  = hash_to_param[i];
				const auto action = param_actions[p];
				std::string_view view = param_aliases[a];
				return (view == name);
			}

	}; // class block_list


	// -- deduction guides ----------------------------------------------------

	template <size_type... NBA, size_type... NP, size_type... NPA>
	block_list(const sx::block<NBA, NP, NPA>&...)
	-> block_list<sx::block<NBA, NP, NPA>...>;

} // namespace sx

#endif // language_syntax_block_list_hpp





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
			//	std::cout << "size of block remaps: " << sizeof(block_remaps) << " bytes\n";
			//	std::cout << "size of hash to block: " << sizeof(hash_to_block) << " bytes\n";
			//	std::cout << "size of param remaps: " << sizeof(param_remaps) << " bytes\n";
			//	std::cout << "size of param seeds: " << sizeof(param_seeds) << " bytes\n";
			//}
