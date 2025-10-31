

	template <sd::action_type A, ml::literal... Ws>
	struct entry2 final {
	};



	namespace impl {
		template <typename...>
		struct is_entry;
		template <sd::action_type A, ml::literal... Ws>
		struct is_entry<entry2<A, Ws...>> final {
			static constexpr bool value = true;
			non_instantiable_class(is_entry);
		};
	}

	template <typename T>
	concept is_entry = sx::impl::is_entry<T>::value;


	template <sx::is_entry... Es>
	class flat_trie final {


		private:

			using self = sx::flat_trie<Es...>;



			template <ml::literal... Ws>
			struct word_list final {

			};

			template <typename...>
			struct concat;

			template <sd::action_type A, ml::literal... Ws>
			struct concat<sx::entry2<A, Ws...>> {
				using type = word_list<Ws...>;
			};

			template <sd::action_type A1, ml::literal... W1,
					  sd::action_type A2, ml::literal... W2,
					  typename... Rest>
			struct concat<sx::entry2<A1, W1...>,
								sx::entry2<A2, W2...>,
								Rest...> {
				using type = typename concat<
									sx::entry2<A1, W1..., W2...>,
									Rest...
								>::type;
			};



			using words_type = typename concat<Es...>::type;


			template <ml::literal W>
			static auto print_word(void) -> void {
				std::cout << W.data << "\n";
			}

		public:
			static auto print(void) -> void {
			}


			static constexpr auto strlen(const char* s) noexcept -> sx::uint {
				sx::uint n = 0U;
				while (*s != '\0') {
					++s; ++n;
				} return n;
			}

			static constexpr auto lcp(const char* s1, const char* s2) noexcept -> sx::uint {
				sx::uint n = 0U;
				while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2) {
					++s1; ++s2; ++n;
				} return n;
			}

			template <typename T, sx::uint N, typename C>
			static constexpr auto insertion_sort(T (&arr)[N], const C& cmp) noexcept -> void {
				for (sx::uint i = 1U; i < N; ++i) {
					const char* key = arr[i];
					sx::uint j = i;
					while (j > 0U && cmp(key, arr[j - 1U])) {
						arr[j] = arr[j - 1]; --j;
					}
					arr[j] = key;
				}
			}


			template <typename...>
			struct dispatch;




			template <sd::action_type A, ml::literal... Ws>
			struct dispatch<sx::entry2<A, Ws...>> {

				static constexpr const char* words[] { Ws.data... };
				static constexpr sd::action_type action = A;
				static constexpr sx::uint word_count = sizeof...(Ws);


				static constexpr auto result(void) -> sx::uint {

					if constexpr (sizeof...(Ws) == 0U)
						return 0U;
					else {

						const char* ws[] { Ws.data... };

						insertion_sort(ws,
						[](const char* a, const char* b) constexpr static noexcept -> bool {
							return std::string_view(a) < std::string_view(b);
						});

						sx::uint count = self::strlen(ws[0U]);
						for (sx::uint i = 1U; i < sizeof...(Ws); ++i) {
							sx::uint l = lcp(ws[i - 1U], ws[i]);
							count += (self::strlen(ws[i]) - l);
						}

						return count;
					}
				}
			};

			static constexpr auto count_nodes(void) -> sx::uint {
				return ((self::dispatch<decltype(Es)>::result()) + ... );
			}

			struct edge final {
				::uint8_t ch;
				sx::uint next;
			};


			// -- private static members --------------------------------------

			static constexpr sx::uint ALPHABET_SIZE = 26U;
			static constexpr sx::uint NODE_COUNT    = self::count_nodes();
			static constexpr sx::uint ARRAY_SIZE    = self::NODE_COUNT
													* self::ALPHABET_SIZE;
			static constexpr sx::uint EMPTY_SLOT    = ARRAY_SIZE;

			// other implementation (compact trie with edges list per node)
			static constexpr sx::uint EDGE_COUNT = self::NODE_COUNT; // approximation


			// -- private members ---------------------------------------------

			sx::uint          _nodes[self::ARRAY_SIZE];
			sd::action_type _actions[self::NODE_COUNT + 1U];

			// other implementation (compact trie with edges list per node)
			//self::edge _edges[self::EDGE_COUNT];
			//sx::uint _offsets[self::NODE_COUNT + 1U];



			template <sx::entry2 E>
			constexpr auto fill(void) noexcept -> void {

				using T = decltype(E);

				const char* const* words = dispatch<T>::words;
				const auto count = dispatch<T>::word_count;

				//const char* words[] { Ws.data... };

				sx::uint available = 0U;

				for (sx::uint w = 0U; w < count; ++w) {
					const char* word = words[w];
					sx::uint node    = 0U;

					for (sx::uint i = 0U; word[i] != '\0'; ++i) {
						const char c = word[i];
						const sx::uint ci = static_cast<sx::uint>(c - 'a') % self::ALPHABET_SIZE;
						const sx::uint ni = node * self::ALPHABET_SIZE;
						const sx::uint si = ni + ci;

						if (_nodes[si] != EMPTY_SLOT) {
							node = _nodes[si];
							continue; }

						_nodes[si] = ++available;
						node       = _nodes[si];
					}

					//if (node < self::NODE_COUNT)
					_actions[node] = dispatch<T>::action;
					//_is_terminal[node] = true;
				}
			}


		public:

			// -- public lifecycle --------------------------------------------

			constexpr flat_trie(void) noexcept
			: _nodes{}, _actions{}, _edges{}, _offsets{} {

				for (sx::uint& n : _nodes)
					n = EMPTY_SLOT;

				((self::fill<Es>()), ... );
			}


			// -- public accessors --------------------------------------------

			constexpr auto contains(const char* word) const noexcept -> bool {
				sx::uint node = 0U;

				for (sx::uint i = 0U; word[i] != '\0'; ++i) {
					const char c = word[i];
					const sx::uint char_idx = static_cast<sx::uint>(c - 'a') % self::ALPHABET_SIZE;
					const sx::uint node_idx = node * self::ALPHABET_SIZE;
					const sx::uint slot_idx = node_idx + char_idx;

					if (_nodes[slot_idx] == EMPTY_SLOT)
						return false;

					node = _nodes[slot_idx];
				}

				return _actions[node] != nullptr;
			}


			auto debug(void) const -> void {

				std::cout << "flat_trie debug:\n";
				std::cout << "  ALPHABET_SIZE: " << self::ALPHABET_SIZE << "\n";
				std::cout << "  NODE_COUNT:    " << self::NODE_COUNT    << "\n";
				std::cout << "  ARRAY_SIZE:    " << self::ARRAY_SIZE    << "\n";
				std::cout << "  EMPTY_SLOT:    " << self::EMPTY_SLOT    << "\n";
				std::cout << "  NODES:\n";
				for (sx::uint n = 0U; n < self::NODE_COUNT; ++n) {
					std::cout << "    Node " << n << ": ";
					for (sx::uint c = 0U; c < self::ALPHABET_SIZE; ++c) {
						sx::uint idx = n * self::ALPHABET_SIZE + c;
						if (_nodes[idx] != EMPTY_SLOT)
							std::cout << static_cast<char>('a' + c)
									  << "->" << _nodes[idx] << " ";
					}
					if (_actions[n] != nullptr)
						std::cout << "(terminal)";
					std::cout << "\n";
				}

				std::cout << "sizeof(flat_trie): " << sizeof(self) << " bytes\n";
			}

	}; // class flat_trie
