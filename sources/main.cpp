#include <iostream>
#include "application.hpp"

//#include "language/char_map.hpp"

#include "language/syntax/block_list.hpp"
#include "macros.hpp"
#include "language/syntax/syntax_layout.hpp"
#include "language/syntax/block_iterator.hpp"

#include "language/tokens.hpp"
#include "debug_chars.hpp"

namespace sx {
}


auto is_lower(int c) -> bool {
	return (c >= 'a') && (c <= 'z');
}

auto main(int ac, char** av) -> int {


	//auto& bl = sx::layout;

	//auto bv = bl.find_block("play");

	//if (bv.block_not_found()) {
	//	std::cout << "Block 'note' not found\n";
	//	return 1;
	//}

	//auto pv = bv.find_param("title");
	//
	//if (pv.param_not_found()) {
	//	std::cout << "Param 'title' not found\n";
	//	return 1;
	//}


	//for (const auto& block : bl) {
	//
	//
	//	std::cout << "block:";
	//	for (const auto& alias : block.aliases())
	//		std::cout << " '" << alias << "'";
	//	std::cout << "\n";
	//
	//	for (const auto& param : block.params()) {
	//		std::cout << "   param:";
	//		for (const auto& palias : param.aliases())
	//			std::cout << " '" << palias << "'";
	//		std::cout << "\n";
	//	}
	//}
	//return 0;







	try {
		std::cout << "\x1b[31m--- program started ---\x1b[0m\n";

		ml::unix_descriptor fd{ml::open("./hello.msh", O_RDONLY)};
		ml::mapped_file mf{fd};
		ml::project p;
		p.update(mf);

		return 0;


		if (ac != 2) {
			std::cerr << "Usage: " << av[0] << " <file-path>\n";
			return 1;
		}
		ml::application::run(av[1U]);
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}

	return 0;
}



	//using uint = unsigned int;
	//
	//constexpr auto common_prefix_length(const char* a, const char* b) noexcept -> size_type {
	//	// N = min size to iterate on N
	//	size_type n = 0U;
	//	while (a[n] && b[n] && (a[n] == b[n]))
	//		++n;
	//	return n;
	//}
	//
	//constexpr auto strlen(const char* s) noexcept -> size_type {
	//	size_type n = 0U;
	//	while (s[n])
	//		++n;
	//	return n;
	//}
	//
	//constexpr auto compute_trie_sizes(const char* const* words, const size_type nwords) noexcept -> std::pair<size_type, size_type> {
	//	if (nwords == 0U)
	//		return {0U, 1U};
	//
	//	size_type edges   = ::strlen(words[0U]);
	//	for (size_type i = 1U; i < nwords; ++i) {
	//		const size_type cpl = common_prefix_length(words[i - 1U], words[i]);
	//		edges += (sx::strlen(words[i]) - cpl);
	//	}
	//
	//	size_type offsets = edges + 1U;
	//	return {edges, offsets};
	//}
	//
	////template <size_type NEDGES, size_type NOFFSETS>
	//class flat_trie final {
	//
	//	private:
	//
	//		struct edge final {
	//			char        c;
	//			size_type   dest;
	//		};
	//
	//
	//		//const edge        _edges[NEDGES];
	//		//const size_type _offsets[NOFFSETS];
	//		std::vector<edge> _edges;
	//		std::vector<size_type> _offsets;
	//
	//	public:
	//
	//		flat_trie(const char* const* words, const size_type nwords) noexcept
	//		: _edges{}, _offsets{} {
	//
	//			auto sizes = sx::compute_trie_sizes(words, nwords);
	//			_edges.resize(sizes.first);
	//			_offsets.resize(sizes.second);
	//
	//
	//			size_type next_offset = 1U; // 0 is root
	//
	//			#define MAX_WORD_LENGTH 64
	//			size_type stack[MAX_WORD_LENGTH];
	//			stack[0U] = 0U;
	//			size_type sp = 1U;
	//			size_type edge_index = 0U;
	//
	//			const char* prev = "";
	//
	//			for (size_type i = 0U; i < nwords; ++i) {
	//
	//				const auto cpl = common_prefix_length(prev, words[i]);
	//
	//				while (sp > (cpl + 1U)) {
	//
	//					auto& e = _edges[stack[--sp]];
	//					--sp;
	//					e.dest = next_offset;
	//				}
	//			}
	//		}
	//
	//}; // class flat_trie








	//template <ml::literal... Ns>
	//struct aliases final {
	//	non_instantiable_class(aliases);
	//	static constexpr ml::usz count = sizeof...(Ns);
	//};
	//
	//template <sd::action_type, typename>
	//struct entry;
	//
	//template <sd::action_type A, ml::literal... Ns>
	//struct entry<A, kw::aliases<Ns...>> final {
	//	using aliases = kw::aliases<Ns...>;
	//	non_instantiable_class(entry);
	//	static constexpr ml::usz alias_count = sizeof...(Ns);
	//};
	//
	//template <sd::action_type A, ml::literal... Ns>
	//using make_entry = kw::entry<A, kw::aliases<Ns...>>;
	//
	///* forward declaration */
	//template <typename...>
	//struct list;
	//
	//
	///* entry specialization */
	//template <sd::action_type... As, typename... Ns>
	//struct list<kw::entry<As, Ns>...> final {
	//	non_instantiable_class(list);
	//	static constexpr ml::usz entry_count = sizeof...(Ns);
	//	static constexpr ml::usz alias_count = (kw::entry<As, Ns>::alias_count + ... );
	//};
	//
	///* list specialization */
	//template <typename... Tp>
	//struct list<kw::list<Tp...>> final {
	//	non_instantiable_class(list);
	//	using type = kw::list<Tp...>;
	//};
	//
	///* list concatenation specialization */
	//template <typename... Tp, typename... Ts, typename... Rs>
	//struct list<kw::list<Tp...>, kw::list<Ts...>, Rs...> final {
	//	non_instantiable_class(list);
	//	using type = typename kw::list<kw::list<Tp..., Ts...>, Rs...>::type;
	//};
	//
	//
	//template <typename...>
	//struct block final {
	//	static_assert(false, "block must be specialized");
	//};
	//
	//template <sd::action_type A, ml::literal... Ns, typename... P>
	//struct block<kw::entry<A, kw::aliases<Ns...>>, kw::list<P...>> final {
	//	using specifier = kw::entry<A, kw::aliases<Ns...>>;
	//	using params    = kw::list<P...>;
	//	non_instantiable_class(block);
	//
	//	static constexpr ml::usz entry_count = params::entry_count + 1U;
	//	static constexpr ml::usz alias_count = specifier::alias_count
	//										 +    params::alias_count;
	//
	//	static auto debug(void) -> void {
	//		std::cout << "entry count: " << entry_count << "\n";
	//		std::cout << "alias count: " << alias_count  << "\n";
	//	}
	//};
	//
	//template <typename...>
	//class flatten;
	//
	//template <typename... S, typename... P>
	//class flatten<kw::block<S, P>...> final {
	//
	//	private:
	//		static constexpr ml::usz block_count = sizeof...(S);
	//
	//		struct block final {
	//			ml::u16 ns;
	//			ml::u16 ne;
	//			ml::u16 ps;
	//			ml::u16 pe;
	//			ml::u16 ai;
	//		};
	//
	//		struct param final {
	//			ml::u16 ns;
	//			ml::u16 ne;
	//			ml::u16 ai;
	//		};
	//
	//
	//		struct block_aliases final {
	//
	//
	//			template <typename T>
	//			static constexpr auto dispatch(void) -> void {
	//				for (ml::usz i = 0U; i < T::count; ++i) {
	//					std::cout << " - " << T::names[i] << "\n";
	//				}
	//			}
	//
	//			static constexpr auto debug(void) -> void {
	//				((dispatch<typename S::aliases>()), ...);
	//			}
	//
	//			static constexpr const char* aliases[(S::alias_count + ... )] {
	//				//(S::specifier::name, ... )
	//			};
	//		};
	//
	//
	//	public:
	//		static auto debug(void) -> void {
	//			block_aliases::debug();
	//			//std::cout << "block alias count: " << (S::alias_count + ... ) << "\n";
	//			//constexpr block_aliases ba{};
	//			//for (ml::usz i = 0U; i < (S::alias_count + ... ); ++i) {
	//			//	std::cout << " - " << ba.aliases[i] << "\n";
	//			//}
	//			//((kw::block<S, P>::debug()), ...);
	//		}
	//
	//}; // class flatten
	//
	//
	//using test = kw::flatten<
	//
	//			kw::block<
	//				kw::make_entry<&sd::call<>, "track", "tr">,
	//
	//				kw::list<
	//					kw::make_entry<&sd::call<>, "track">,
	//					kw::make_entry<&sd::call<>, "trig", "trigger">,
	//					kw::make_entry<&sd::call<>, "trig", "trigger", "tg">
	//				>
	//			>,
	//			kw::block<
	//				kw::make_entry<&sd::call<>, "track", "tr">,
	//
	//				kw::list<
	//					kw::make_entry<&sd::call<>, "track">,
	//					kw::make_entry<&sd::call<>, "trig", "trigger">,
	//					kw::make_entry<&sd::call<>, "trig", "trigger", "tg">
	//				>
	//			>
	//			>;

















	//template <sd::action_type A, ml::literal N>
	//struct entry final {
	//	non_instantiable_class(entry);
	//	static constexpr sd::action_type action = A;
	//	static constexpr const char*       name = N.data;
	//};
	//
	//
	//namespace impl {
	//
	//	/* forward declaration */
	//	template <typename...>
	//	struct list;
	//
	//	/* entry specialization */
	//	template <sd::action_type... As, ml::literal... Ls>
	//	struct list<kw::entry<As, Ls>...> final {
	//		non_instantiable_class(list);
	//		static constexpr ml::usz size = sizeof...(Ls);
	//		using type = kw::impl::list<kw::entry<As, Ls>...>;
	//	};
	//
	//	/* list specialization */
	//	template <typename... Tp>
	//	struct list<kw::impl::list<Tp...>> final {
	//		non_instantiable_class(list);
	//		using type = kw::impl::list<Tp...>;
	//	};
	//
	//	/* list concatenation specialization */
	//	template <typename... Tp, typename... Ts, typename... Rest>
	//	struct list<kw::impl::list<Tp...>, kw::impl::list<Ts...>, Rest...> final {
	//		non_instantiable_class(list);
	//		using type = typename kw::impl::list<kw::impl::list<Tp..., Ts...>, Rest...>::type;
	//	};
	//
	//
	//
	//	template <sd::action_type A, ml::literal... Ns>
	//	struct add final {
	//		non_instantiable_class(add);
	//		using type = kw::impl::list<kw::entry<A, Ns>...>;
	//	};
	//
	//} // namespace impl
	//
	//template <typename... Tp>
	//using list = typename kw::impl::list<Tp...>::type;
	//
	//
	//template <sd::action_type A, ml::literal... Ns>
	//using add_param = typename kw::impl::add<A, Ns...>::type;
	//
	//template <sd::action_type A, ml::literal... Ns>
	//using add_specifier = typename kw::impl::add<A, Ns...>::type;
	//
	//template <typename, typename>
	//struct block;
	//
	//template <typename... S, typename... P>
	//struct block<kw::impl::list<S...>, kw::impl::list<P...>> {
	//	using specifier = kw::list<S...>;
	//	using params    = kw::list<P...>;
	//	//non_instantiable_class(block);
	//
	//	static constexpr ml::usz block_count = specifier::size;
	//	static constexpr ml::usz param_count = params::size;
	//	static constexpr ml::usz entry_count = block_count + param_count;
	//
	//};
	//
	//
	//
	//template <typename...>
	//class flatten;
	//
	//template <typename... S, typename... P>
	//class flatten<kw::block<S, P>...> final {
	//
	//
	//	private:
	//
	//
	//		struct block final {
	//			ml::u16 ns;
	//			ml::u16 ne;
	//			ml::u16 ps;
	//			ml::u16 pe;
	//			ml::u16 ai;
	//		};
	//
	//		struct param final {
	//			ml::u16 ns;
	//			ml::u16 ne;
	//			ml::u16 ai;
	//		};
	//
	//
	//		static constexpr ml::usz entry_count = (kw::block<S, P>::entry_count + ...);
	//		static constexpr ml::usz block_count = (kw::block<S, P>::block_count + ...);
	//		static constexpr ml::usz param_count = (kw::block<S, P>::param_count + ...);
	//
	//		static constexpr const char* _names[entry_count] = {
	//		};
	//
	//		static constexpr sd::action_type _actions[entry_count] = {
	//		};
	//
	//		static constexpr block _blocks[block_count] = {
	//		};
	//
	//		static constexpr param _params[param_count] = {
	//		};
	//
	//
	//	public:
	//
	//		static auto debug(void) -> void {
	//			std::cout << "flatten debug:\n";
	//			std::cout << " total entries: " << entry_count << "\n";
	//			std::cout << " total blocks:  " << block_count << "\n";
	//			std::cout << " total params:  " << param_count << "\n";
	//		}
	//
	//
	//};
	//
	//using block_track = kw::block<
	//							  kw::list<
	//										kw::entry<&sd::call<>, "track", "fioj">,
	//										kw::entry<&sd::call<>, "track">
	//										//kw::add_specifier<&sd::call<>, "track", "fioj">,
	//										//kw::add_specifier<&sd::call<>, "track">
	//								>,
	//
	//							  kw::list<
	//									kw::entry<&sd::call<>, "trig", "trigger", "tr">
	//									//kw::add_param<&sd::call<>, "trig", "trigger", "tr">,
	//									//kw::add_param<&sd::call<>, "note", "no">,
	//									//kw::add_param<&sd::call<>, "gate", "ga">
	//							>
	//							>;
