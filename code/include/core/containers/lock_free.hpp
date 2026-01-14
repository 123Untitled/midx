#ifndef lock_free_hpp
#define lock_free_hpp

#include "core/types.hpp"
#include <atomic>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- L O C K  F R E E ----------------------------------------------------

	template <typename T, mx::usz N>
	class lock_free final {
		private:

			T _buffer[N];

			std::atomic<mx::usz> _wr;
			std::atomic<mx::usz> _rd;

		public:

			lock_free(void)
			: /* uninitialized */ _wr{0U}, _rd{0U} {
			}

			auto push(const T& item) noexcept -> bool {

				const mx::usz wr   = _wr.load(std::memory_order_relaxed);
				const mx::usz next = (wr + 1U) % N;

				if (next == _rd.load(std::memory_order_acquire))
					return false; // full

				_buffer[wr] = item;
				_wr.store(next, std::memory_order_release);

				return true;
			}

			auto pop(T& item) noexcept -> bool {

				const mx::usz rd = _rd.load(std::memory_order_relaxed);

				if (rd == _wr.load(std::memory_order_acquire))
					return false; // empty

				item = _buffer[rd];
				_rd.store((rd + 1U) % N, std::memory_order_release);

				return true;
			}

	}; // class lock_free

} // namespace mx

#endif // lock_free_hpp
