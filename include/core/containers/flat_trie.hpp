#ifndef core_containers_flat_trie_hpp
#define core_containers_flat_trie_hpp

#include "core/types.hpp"
#include "literal.hpp"
#include "language/lexer/lexeme.hpp"
#include "core/type_traits/type_transformations/conditional.hpp"

//#include "core/string/string_view.hpp"
#include "core/string/strcmp.hpp"
#include <iostream>

#include "core/utility/limits.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {



	/* entry */
	template <typename T, T V, mx::literal... Ws>
	struct entry final {
		non_instantiable_class(entry);
		static constexpr const char* words[sizeof...(Ws)] { Ws.data... };
		static constexpr mx::usz     count = sizeof...(Ws);
		static constexpr T           value = V;
	};



	namespace impl {
		template <typename...>
		struct is_entry;
		template <typename T, T V, mx::literal... Ws>
		struct is_entry<entry<T, V, Ws...>> final {
			static constexpr bool value = true;
			non_instantiable_class(is_entry);
		};
	} // namespace impl

	template <typename T>
	concept is_entry = mx::impl::is_entry<T>::value;



	// -- F L A T  T R I E ----------------------------------------------------

	template <typename T, T D, mx::is_entry... Es>
	class flat_trie final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::flat_trie<T, D, Es...>;


			// -- private static methods --------------------------------------

			/* count total nodes */
			static constexpr auto count_nodes(void) -> mx::usz {

				return (([]<mx::is_entry E>() static constexpr noexcept -> mx::usz {

					// longest common prefix
					auto lcp = [](const char* s1, const char* s2)
						static constexpr noexcept -> mx::usz {
						mx::usz n = 0U;
						while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2) {
							++s1; ++s2; ++n;
						} return n;
					};

					// string length
					auto strlen = [](const char* s)
						static constexpr noexcept -> mx::usz {
						mx::usz n = 0U;
						while (*s != '\0') {
							++s; ++n;
						} return n;
					};

					// copy words
					const char* ws[E::count] {};
					for (mx::usz i = 0U; i < E::count; ++i)
						ws[i] = E::words[i];

					// insertion sort
					[](const char* (&arr)[E::count])
						static constexpr noexcept -> void {
						for (mx::usz i = 1U; i < E::count; ++i) {
							const char* key = arr[i];
							mx::usz j = i;
							//while (j > 0U && mx::string_view(key)
							//			   < mx::string_view(arr[j - 1U])) {
							//while (j > 0U && mx::str_lexicographical(key, arr[j - 1U]) > 0) {

							while (j > 0U) {
								if (mx::str_lexicographical(key, arr[j - 1U]) < 0)
									break;
								arr[j] = arr[j - 1]; --j;
							}
							arr[j] = key;
						}
					}(ws);

					// calculate total length with lcp
					mx::usz len = strlen(ws[0U]);
					for (mx::usz i = 1U; i < E::count; ++i) {
						mx::usz l = lcp(ws[i - 1U], ws[i]);
						len += (strlen(ws[i]) - l);
					}

					return len;
				  }).template operator()<Es>() + ... );
			}


			// -- private static members --------------------------------------

			static constexpr mx::usz ALPHABET_SIZE = 26U;
			static constexpr mx::usz NODE_COUNT    = self::count_nodes();
			static constexpr mx::usz ARRAY_SIZE    = self::NODE_COUNT
												   * self::ALPHABET_SIZE;

			using node_type =
				mx::conditional<self::NODE_COUNT <= mx::limits<mx::u8 >::max(), mx::u8,
				mx::conditional<self::NODE_COUNT <= mx::limits<mx::u16>::max(), mx::u16,
				mx::conditional<self::NODE_COUNT <= mx::limits<mx::u32>::max(), mx::u32,
				mx::u64>>>;

			static constexpr node_type EMPTY_SLOT = 0U;




			// -- private members ---------------------------------------------

			/* nodes */
			node_type _nodes[self::ARRAY_SIZE];

			/* values */
			T _values[self::NODE_COUNT + 1U];



			// -- private methods ---------------------------------------------

			/* fill */
			template <mx::is_entry E>
			constexpr auto fill(node_type& available) noexcept -> void {

				constexpr auto& words = E::words;
				constexpr auto  count = E::count;
				constexpr auto  value = E::value;


				for (mx::usz w = 0U; w < count; ++w) {

					node_type node = 0U;
					for (mx::usz i = 0U; words[w][i] != '\0'; ++i) {

						const mx::usz si = (node * self::ALPHABET_SIZE)
										 + (static_cast<mx::usz>(words[w][i] - 'a')
												 % self::ALPHABET_SIZE);

						if (_nodes[si] != EMPTY_SLOT) {
							node = _nodes[si];
							continue;
						}

						_nodes[si] = ++available;
						node       = _nodes[si];
					}

					_values[node] = value;
				}
			}


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			constexpr flat_trie(void) noexcept
			: _nodes{}, _values{} {

				for (node_type& n : _nodes)
					n = EMPTY_SLOT;

				for (T& v : _values)
					v = D;

				node_type available = 0U;
				((self::fill<Es>(available)), ... );
			}


			// -- public accessors --------------------------------------------

			/* find */
			constexpr auto find(const char* word) const noexcept -> T {

				node_type node = 0U;

				for (mx::usz i = 0U; word[i] != '\0'; ++i) {
					const mx::usz si = (node * self::ALPHABET_SIZE)
									 + (static_cast<mx::usz>(word[i] - 'a')
											 % self::ALPHABET_SIZE);

					if (_nodes[si] == EMPTY_SLOT)
						return D;

					node = _nodes[si];
				}

				return _values[node];
			}

			/* find */
			constexpr auto find(const lx::lexeme& l) const noexcept -> T {

				node_type node = 0U;

				for (mx::usz i = 0U; i < l.size; ++i) {
					const mx::usz si = (node * self::ALPHABET_SIZE)
									 + (static_cast<mx::usz>(l.data[i] - 'a')
											 % self::ALPHABET_SIZE);

					if (_nodes[si] == EMPTY_SLOT)
						return D;

					node = _nodes[si];
				}

				return _values[node];
			}
			

			/* contains */
			constexpr auto contains(const char* word) const noexcept -> bool {
				return self::find(word) != D;
			}


			auto debug(void) const -> void {

				std::cout << "flat_trie debug:\n";
				std::cout << "  ALPHABET_SIZE: " << self::ALPHABET_SIZE << "\n";
				std::cout << "  NODE_COUNT:    " << self::NODE_COUNT    << "\n";
				std::cout << "  ARRAY_SIZE:    " << self::ARRAY_SIZE    << "\n";
				std::cout << "  EMPTY_SLOT:    " << (int)self::EMPTY_SLOT    << "\n";
				std::cout << "  NODES:\n";
				for (mx::usz n = 0U; n < self::NODE_COUNT; ++n) {
					std::cout << "    Node " << n << ": ";
					for (mx::usz c = 0U; c < self::ALPHABET_SIZE; ++c) {
						mx::usz idx = n * self::ALPHABET_SIZE + c;
						if (_nodes[idx] != EMPTY_SLOT)
							std::cout << static_cast<char>('a' + c)
									  << "->" << (int)_nodes[idx] << " ";
					}

					if (_values[n] != D)
						std::cout << "(terminal)";
					std::cout << "\n";
				}

				std::cout << "sizeof(flat_trie): " << sizeof(self) << " bytes\n";
			}

	}; // class flat_trie

} // namespace mx

#endif // core_containers_flat_trie_hpp
