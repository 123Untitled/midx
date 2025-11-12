







	//template <mx::literal... Ns>
	//struct aliases final {
	//	non_instantiable_class(aliases);
	//	static constexpr mx::usz count = sizeof...(Ns);
	//};
	//
	//template <sd::action_type, typename>
	//struct entry;
	//
	//template <sd::action_type A, mx::literal... Ns>
	//struct entry<A, kw::aliases<Ns...>> final {
	//	using aliases = kw::aliases<Ns...>;
	//	non_instantiable_class(entry);
	//	static constexpr mx::usz alias_count = sizeof...(Ns);
	//};
	//
	//template <sd::action_type A, mx::literal... Ns>
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
	//	static constexpr mx::usz entry_count = sizeof...(Ns);
	//	static constexpr mx::usz alias_count = (kw::entry<As, Ns>::alias_count + ... );
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
	//template <sd::action_type A, mx::literal... Ns, typename... P>
	//struct block<kw::entry<A, kw::aliases<Ns...>>, kw::list<P...>> final {
	//	using specifier = kw::entry<A, kw::aliases<Ns...>>;
	//	using params    = kw::list<P...>;
	//	non_instantiable_class(block);
	//
	//	static constexpr mx::usz entry_count = params::entry_count + 1U;
	//	static constexpr mx::usz alias_count = specifier::alias_count
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
	//		static constexpr mx::usz block_count = sizeof...(S);
	//
	//		struct block final {
	//			mx::u16 ns;
	//			mx::u16 ne;
	//			mx::u16 ps;
	//			mx::u16 pe;
	//			mx::u16 ai;
	//		};
	//
	//		struct param final {
	//			mx::u16 ns;
	//			mx::u16 ne;
	//			mx::u16 ai;
	//		};
	//
	//
	//		struct block_aliases final {
	//
	//
	//			template <typename T>
	//			static constexpr auto dispatch(void) -> void {
	//				for (mx::usz i = 0U; i < T::count; ++i) {
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
	//			//for (mx::usz i = 0U; i < (S::alias_count + ... ); ++i) {
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

















	//template <sd::action_type A, mx::literal N>
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
	//	template <sd::action_type... As, mx::literal... Ls>
	//	struct list<kw::entry<As, Ls>...> final {
	//		non_instantiable_class(list);
	//		static constexpr mx::usz size = sizeof...(Ls);
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
	//	template <sd::action_type A, mx::literal... Ns>
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
	//template <sd::action_type A, mx::literal... Ns>
	//using add_param = typename kw::impl::add<A, Ns...>::type;
	//
	//template <sd::action_type A, mx::literal... Ns>
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
	//	static constexpr mx::usz block_count = specifier::size;
	//	static constexpr mx::usz param_count = params::size;
	//	static constexpr mx::usz entry_count = block_count + param_count;
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
	//			mx::u16 ns;
	//			mx::u16 ne;
	//			mx::u16 ps;
	//			mx::u16 pe;
	//			mx::u16 ai;
	//		};
	//
	//		struct param final {
	//			mx::u16 ns;
	//			mx::u16 ne;
	//			mx::u16 ai;
	//		};
	//
	//
	//		static constexpr mx::usz entry_count = (kw::block<S, P>::entry_count + ...);
	//		static constexpr mx::usz block_count = (kw::block<S, P>::block_count + ...);
	//		static constexpr mx::usz param_count = (kw::block<S, P>::param_count + ...);
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
