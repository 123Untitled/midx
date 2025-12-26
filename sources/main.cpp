#include <iostream>
#include "application.hpp"

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

} // namespace mx

auto main(int ac, char** av) -> int {


	//mx::storage<mx::u16, mx::usz> s{{2U, 1U}};
	//
	//s.ptr_of<0>()[0U] = 42U;
	//s.ptr_of<0>()[1U] = 84U;
	//*s.ptr_of<mx::usz>() = 123U;
	//s.count_of<mx::u16>(); // 2
	//
	//
	//s.debug();
	//
	//return 0;

	//try {
	//mx::midi m;
	//mx::midi_event mev{
	//	.channel = 0,
	//	.note	= 60,
	//	.velocity = 127,
	//	.gate	= 100
	//};
	//sleep(1);
	////char c;
	////::read(0, &c, 1); // wait for input
	//
	//mx::host_time lt{static_cast<mx::usz>(mx::timebase::ms_to_ticks(1000U))};
	//mx::host_time ts = mx::host_time::now() + lt;
	//std::cout << ts << "\r\n";
	//
	//m.note_on(mev, ts);
	//m.flush();
	//// sleep 100 ms
	//usleep(100'000);
	//m.cancel();
	//
	//
	//sleep(3);
	//}
	//catch (const std::exception& e) {
	//	std::cerr << "\x1b[31mexception\x1b[0m -> " << e.what() << "\r\n";
	//	return 1;
	//}
	//
	//
	//return 0;



	//MIDIFlushOutput
	try {

		mx::application::run();
	}
	catch (const std::exception& e) {
		std::cerr << "\x1b[31mexception\x1b[0m -> " << e.what() << "\r\n";
		return 1;
	}

	return 0;
}
