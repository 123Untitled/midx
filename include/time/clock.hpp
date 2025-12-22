#ifndef clock_hpp
#define clock_hpp

#include "core/types.hpp"
#include "system/unix_pipe.hpp"
#include "math.hpp"
#include "midi/constant.hpp"
#include "time/host_time.hpp"

#include <atomic>

#include "time/timebase.hpp"
#include "time/realtime.hpp"

#include "core/containers/lock_free.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- C L O C K -----------------------------------------------------------

	class clock final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::clock;


			// -- private members ---------------------------------------------

			/* realtime */
			mx::realtime _realtime;

			/* thread */
			::pthread_t _thread;

			/* running */
			std::atomic<bool> _running;


			/* bpm */
			mx::frac _bpm;

			/* host per tick */
			mx::frac _host_per_tick;

			/* next tick */
			mx::frac _next;


			/* lock free queue */
			mx::lock_free<mx::u64, 1024U> _queue;

			/* pipe */
			mx::unix_pipe _pipe;



			// -- private static methods --------------------------------------

			/* entrypoint */
			static auto _entrypoint(void* ptr) noexcept -> void*;



			// -- private methods ---------------------------------------------

			/* run */
			auto _run(void) noexcept -> void;


			static auto _compute_bpm_to_frac(const double bpm) noexcept -> mx::frac {
				// will be implemented as interger and fractional part later...
				constexpr mx::u64 scale = 1'000'000'000ULL;
				return mx::frac{
					static_cast<mx::u64>((bpm * scale) + 0.5),
						scale
				};
			}


			static auto _compute_host_per_tick(const mx::frac& bpm) -> mx::frac {
				const auto num = mx::timebase::num();
				const auto den = mx::timebase::den();
				return mx::frac{
					60'000'000'000ULL * den,
					(bpm.num * num * MIDI_PPQN) / bpm.den
				};
			}



		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			clock(void);

			/* destructor */
			~clock(void) noexcept;


			// -- public accessors --------------------------------------------

			/* is running */
			auto is_running(void) const noexcept -> bool;


			// -- public methods ----------------------------------------------

			/* start */
			auto start(void) -> void {

				if (is_running() == true)
					return;

				_running.store(true, std::memory_order_release);

				// create thread
				if (::pthread_create(&_thread, nullptr, _entrypoint, this) != 0) {
					_running = false;
					throw mx::system_error{"pthread_create"};
				}
			}

			/* stop */
			auto stop(void) noexcept -> void {

				if (is_running() == false)
					return;

				_running.store(false, std::memory_order_release);

				static_cast<void>(::pthread_join(_thread, nullptr));
			}


			/* toggle */
			auto toggle(void) -> void {
				is_running() == true ? stop() : start();
			}




			auto ident(void) const noexcept -> int {
				return _pipe.rd();
			}


			auto queue(void) noexcept -> mx::lock_free<mx::u64, 1024U>& {
				return _queue;
			}

			auto consume(void) const -> mx::usz {
				mx::usz count = 0U;
				const auto readed = ::read(_pipe.rd(), &count, sizeof(count));
				if (readed < 0)
					throw mx::system_error{"read"};
				return count;
			}


	}; // class clock

} // namespace mx

#endif // clock_hpp
