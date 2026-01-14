#ifndef atomic_hpp
#define atomic_hpp

#include "core/type_traits/type_categories/is_integral.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	enum class mo : int {
		relaxed = __ATOMIC_RELAXED,
		consume = __ATOMIC_CONSUME,
		acquire = __ATOMIC_ACQUIRE,
		release = __ATOMIC_RELEASE,
		acq_rel = __ATOMIC_ACQ_REL,
		seq_cst = __ATOMIC_SEQ_CST,
	};


	// -- A T O M I C ---------------------------------------------------------

	template <typename T>
	class atomic final {

		static_assert(mx::is_integral<T>);
		//static_assert(is_trivially_copyable<_Tp>::value, "std::atomic<T> requires that 'T' be a trivially copyable type");

		private:
			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::atomic<T>;


			// -- private members ---------------------------------------------

			//mutable
				volatile T _value;


			template <mx::mo O, mx::mo... Os>
			static constexpr bool valid_order = ((O == Os) || ...);

			template <mx::mo O>
			static constexpr bool load_order = valid_order<O, mo::relaxed, mo::consume, mo::acquire, mo::seq_cst>;

			template <mx::mo O>
			static constexpr bool store_order = valid_order<O, mo::relaxed, mo::release, mo::seq_cst>;

			template <mx::mo O>
			static constexpr bool exchange_order = valid_order<O, mo::relaxed, mo::acquire, mo::release, mo::acq_rel, mo::seq_cst>;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			constexpr atomic(void) noexcept = default;

			/* value constructor */
			constexpr explicit atomic(const T value) noexcept
			: _value{value} {
			}

			/* deleted copy constructor */
			atomic(const self&) = delete;

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;
			auto operator=(const self&) volatile -> self& = delete;


			// -- public methods ----------------------------------------------

			/* load */
			template <mx::mo order = mo::seq_cst>
			auto load(void) const noexcept -> T {
				static_assert(load_order<order>);
				return ::__atomic_load_n(&_value, static_cast<int>(order));
			}

			/* load (volatile) */
			template <mx::mo order = mo::seq_cst>
			auto load(void) const volatile noexcept -> T {
				static_assert(load_order<order>);
				return ::__atomic_load_n(&_value, static_cast<int>(order));
			}


			/* store */
			template <mx::mo order = mo::seq_cst>
			auto store(const T value) noexcept -> void {
				static_assert(store_order<order>);
				::__atomic_store_n(&_value, value, static_cast<int>(order));
			}

			/* store (volatile) */
			template <mx::mo order = mo::seq_cst>
			auto store(const T value) volatile noexcept -> void {
				static_assert(store_order<order>);
				::__atomic_store_n(&_value, value, static_cast<int>(order));
			}

			/* exchange */
			template <mx::mo order = mo::seq_cst>
			auto exchange(const T value) noexcept -> T {
				static_assert(exchange_order<order>);
				return ::__atomic_exchange_n(&_value, value, static_cast<int>(order));
			}

			/* exchange (volatile) */
			template <mx::mo order = mo::seq_cst>
			auto exchange(const T value) volatile noexcept -> T {
				static_assert(exchange_order<order>);
				return ::__atomic_exchange_n(&_value, value, static_cast<int>(order));
			}

	}; // class atomic

} // namespace mx

#endif // atomic_hpp
