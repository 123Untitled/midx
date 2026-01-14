


#include <sys/syscall.h>
#include <mach/mach.h>
#include "system/concurrency/atomic.hpp"


namespace mx {



	// mach semaphores
	class semaphore final {

		private:

			::semaphore_t _sem;

		public:

			semaphore(void) noexcept
			: _sem{SEMAPHORE_NULL} {
			}

			explicit semaphore(const int value) {
				_sem = SEMAPHORE_NULL;
				const auto ret = ::semaphore_create(::mach_task_self(),
												   &_sem,
												   SYNC_POLICY_FIFO,
												   value);

				if (ret != KERN_SUCCESS)
					throw mx::system_error{"semaphore_create"};
			}

			~semaphore(void) noexcept {
				if (_sem != SEMAPHORE_NULL)
					static_cast<void>(::semaphore_destroy(::mach_task_self(), _sem));
			}


			// -- public lifecycle --------------------------------------------

			/* wait */
			auto wait(void) -> void {
				const auto ret = ::semaphore_wait(_sem);
				if (ret != KERN_SUCCESS)
					throw mx::system_error{"semaphore_wait"};
			}

			/* signal */
			auto signal(void) -> void {
				const auto ret = ::semaphore_signal(_sem);
				if (ret != KERN_SUCCESS)
					throw mx::system_error{"semaphore_signal"};
			}

	}; // class semaphore

	struct ctx {
	};
	auto parse(ctx& c) -> bool {
		return true;
	}

	class dsl final {

		private:

			mx::analyzer _analyzers[2U];
			mx::semaphore _can_parse;
			mx::atomic<mx::usz> _active;
			mx::atomic<bool>    _pending;


			// -- private methods ---------------------------------------------

			/* active */
			auto active(void) const noexcept -> mx::usz {
				return _active.load<mx::mo::acquire>();
			}

			/* inactive */
			auto inactive(void) const noexcept -> mx::usz {
				return 1U - active();
			}

			/* pending */
			auto pending(void) const noexcept -> bool {
				return _pending.load<mx::mo::acquire>();
			}

			/* request switch */
			auto request_switch(void) -> void {
				_pending.store<mx::mo::release>(true);
			}


		public:


			auto parse(mx::string&& data) -> void {

				// wait authorization to parse
				_can_parse.wait();
				//_pending.wait(false, std::memory_order_acquire);

				const auto inactive = this->inactive();

				auto& analyzer = _analyzers[inactive];
				analyzer.analyze(mx::move(data));

				// check for errors
				if (analyzer.has_errors() == true) {
					_can_parse.signal();
					return;
				}

				// signal pending switch
				this->request_switch();
			}

	}; // class dsl
	
	inline auto exemple(void) -> void {
		mx::semaphore can_parse{1};
		std::atomic<mx::usz> active = 0U;
		std::atomic<bool> pending = false;
		mx::ctx ctxs[2U];


		// in main thread...
		// wait authorization to parse
		can_parse.wait();
		//pending.wait(false, std::memory_order_acquire);

		const auto inactive = 1U - active.load(std::memory_order_acquire);

		// parse data here
		const auto err = parse(ctxs[inactive]);

		if (err == false) {
			can_parse.signal();
			return;
		}

		// signal pending switch
		pending.store(true, std::memory_order_release);



		// in clock thread...
		while (true) {
			const auto index = active.load(std::memory_order_acquire);
			// evaluate parsed data
			// evaluate(ctx[index]);

			if (pending.exchange(false, std::memory_order_acquire) == true) {
				// switch active index
				active.store(1U - index, std::memory_order_release);
				// allow parsing
				can_parse.signal();
			}
		}

	}


} // namespace mx


/* *****************************************

   in clock thread:
   write pipe only if all data processed in main thread

***************************************** */
namespace mx {
	
	template <pa::id P, tk::id T>
	class converter final {

		 public:
		private:

	}; // class converter


}

#include <sys/poll.h>

namespace mx {
	class poll final {

		private:
			std::vector<struct ::pollfd> _fds;

		public:

			auto add(const int fd, const short events) -> void {
				_fds.emplace_back(fd, events, 0);
			}
			auto del(const int fd) -> void {

				for (mx::usz i = 0U; i < _fds.size(); ++i) {
					if (_fds[i].fd != fd)
						continue;

					// pop swap
					if (i + 1U < _fds.size())
						_fds[i] = _fds.back();
					_fds.pop_back();
					break;
				}
			}
			auto wait(const int timeout_ms = -1) -> void {

				const auto ret = ::poll(_fds.data(),
										static_cast<const ::nfds_t>(_fds.size()),
										timeout_ms);
				if (ret < 0)
					throw mx::system_error{"poll"};

				mx::usz count = static_cast<mx::usz>(ret);


				for (mx::usz i = 0U; i < _fds.size(); ++i) {

					if (_fds[i].revents == 0)
						continue;

					_fds[i].revents = 0;

					if (--count == 0U)
						break;
				}
			}

	}; // class poll



} // namespace mx
