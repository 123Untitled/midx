#ifndef midi_midi_engine_hpp
#define midi_midi_engine_hpp

#include "core/types.hpp"
//#include "coremidi/events.hpp"
//#include "coremidi/coremidi.hpp"
#include "midi/midi_event.hpp"

#include "core/utility/type_operations.hpp"
#include "core/memory/align_up.hpp"
#include "core/memory/malloc.hpp"
//#include "core/memory/memset.hpp"
#include "core/utility/exchange.hpp"
#include "core/traits/transformations.hpp"

//#include "diagnostics/runtime_error.hpp"

//#include <CoreAudio/HostTime.h>   // ou AudioToolbox.h si tu préfères
#include <mach/mach_time.h>
#include <iostream>
//#include <vector>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- M I D I  E N G I N E ------------------------------------------------

	class midi_engine final {


		private:

			// -- private constants -------------------------------------------

			/* maximum notes (16 channels x 128 notes = 2048) */
			constexpr static mx::usz max_notes = 16U * 128U;

			struct state final {
				mx::u16 ticks;
				mx::u16 epoch;
			}; // struct state


			// -- private members ---------------------------------------------

			state   _states[max_notes];
			mx::u16 _active[max_notes];
			mx::u16 _count;
			mx::u16 _epoch;


			// -- private static methods --------------------------------------

			inline static auto idx_to_ch(const mx::u16 index) noexcept -> mx::u8 {
				return static_cast<mx::u8>(index >> 7U);
			}

			inline static auto idx_to_no(const mx::u16 index) noexcept -> mx::u8 {
				return static_cast<mx::u8>(index & 0x7FU);
			}

			/* to index */
			static auto to_index(const mx::u8 ch, const mx::u8 no) noexcept -> mx::u16 {
				return static_cast<mx::u16>((ch << 7U) | no);
			}


			/* reset */
			auto _reset(void) noexcept -> void {

				// reset states
				for (mx::u16 i = 0U; i < max_notes; ++i)
					_states[i].ticks = _states[i].epoch = 0U;

				_count = 0U;
				_epoch = 1U;
			}

		public:


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			midi_engine(void)
			: _states{},
			  _active{},
			  _count{0U},
			  _epoch{1U} {
			}

			/* off all */
			template <typename T>
			auto off_all(T& evs) -> void {

				//auto ts = AudioGetCurrentHostTime();
				auto ts = ::mach_absolute_time();

				// note off all notes
				for (; _count > 0U; --_count) {
					const auto i = _active[_count - 1U];
					evs.note_off(idx_to_ch(i), idx_to_no(i), ts);
				}

				_reset();
			}


			/* next epoch */
			auto next_epoch(void) noexcept -> void {

				// increment epoch, reset if overflow
				if (_epoch == 0xFFFFU) {
					for (mx::u16 i = 0U; i < max_notes; ++i)
						_states[i].epoch = 0U;
					_epoch = 1U;
					return;
				}
				++_epoch;
			}


			/* note on */
			template <typename T>
			auto note_on(T& evs, const mx::midi_event& ev, const mx::u64 ts) -> void {

				// get index
				const mx::u16 i = to_index(ev.channel, ev.note);

				// get state
				auto& st = _states[i];

				// ensure gate is not zero
				if (ev.gate == 0U)
					return;

				// check if already note on
				if (st.epoch == _epoch) {
					st.ticks = (mx::u16)ev.gate; // CAST WILL BE FIXED !
					return;
				}

				// check if not already active
				if (st.ticks == 0U) {
					_active[_count++] = i;
				}
				else {
					std::cout << "\x1b[33mWarning:\x1b[0m "
							  << "Retriggering note before note off. "
							  << "Ch " << static_cast<mx::usz>(ev.channel)
							  << " No " << static_cast<mx::usz>(ev.note)
							  << " Ticks " << ev.gate << "\r\n";
					// note off before note on
					evs.note_off(ev.channel, ev.note, ts);
				}

				// note on event can be triggered here
				evs.note_on(ev.channel, ev.note, ev.velocity, ts);
				std::cout << "\x1b[32mNote On:\x1b[0m Ch " << static_cast<mx::usz>(ev.channel)
						  << " No " << static_cast<mx::usz>(ev.note)
						  << " Ve " << static_cast<mx::usz>(ev.velocity)
						  << " Ticks " << ev.gate << "\r\n";

				st.epoch = _epoch;
				st.ticks = (mx::u16)ev.gate; // CAST WILL BE FIXED !
			}

			/* off pass */
			template <typename T>
			auto off_pass(T& evs, const mx::u64 ts) -> void {

				mx::u16 i = 0U;
				while (i < _count) {

					const mx::u16 a = _active[i];

					// decrement ticks, continue if not zero
					if (--_states[a].ticks != 0U) {
						++i;
						continue;
					}

					// compute channel and note
					const mx::u8 ch = idx_to_ch(a);
					const mx::u8 no = idx_to_no(a);

					// note off event can be triggered here
					evs.note_off(ch, no, ts);
					std::cout << "\x1b[31mNote Off:\x1b[0m Ch " << static_cast<mx::usz>(ch)
							  << " No " << static_cast<mx::usz>(no) << "\r\n";

					// swap pop
					_active[i] = _active[--_count];
				}

			}

			template <typename F, typename... Ts>
			auto for_active(F&& f, Ts&&... args) -> void {

				mx::u16 i = 0U;
				while (i < _count) {

					const mx::u16 a = _active[i];

					// decrement ticks, continue if not zero
					if (--_states[a].ticks != 0U) {
						++i;
						continue;
					}

					// compute channel and note
					const mx::u8 ch = idx_to_ch(a);
					const mx::u8 no = idx_to_no(a);

					// call function
					f(ch, no, mx::forward<Ts>(args)...);

					// swap pop
					_active[i] = _active[--_count];
				}
			}

	}; // class midi_engine


	/* index of */
	template <typename T, typename... Tp>
	consteval auto index_of(void) noexcept -> mx::usz {

		static_assert(mx::is_one_of<T, Tp...>, "T is not in parameter pack.");

		mx::usz i = 0U;
		bool found = false;
		((mx::is_same<T, Tp> ? found = true : found ? true : ++i), ...);
		return i;
	}

	/* count of */
	template <typename T, typename... Tp>
	constexpr mx::usz count_of = (0U + ... + (mx::is_same<T, Tp> ? 1U : 0U));

	/* all unique */
	template <typename... Tp>
	concept all_unique = ((mx::count_of<Tp, Tp...> == 1U) && ...);



	// -- S T O R A G E -------------------------------------------------------

	template <typename... Tp>
	class storage final {


		// -- assertions ------------------------------------------------------

		/* requires non-empty parameter pack */
		static_assert(sizeof...(Tp) > 0U, "storage requires at least one type");


		private:
			
			// -- private types -----------------------------------------------

			/* self type */
			using self = storage<Tp...>;


			// -- private members ---------------------------------------------

			mx::u8* _data;
			mx::usz _capacity;
			mx::usz _sizes[sizeof...(Tp)];
			mx::usz _offsets[sizeof...(Tp)];


		public:

			storage(void) noexcept
			: _data{nullptr}, _capacity{0U}, _sizes{}, _offsets{} {
			}

			storage(const self&) = delete;
			storage(self&& other) noexcept
			:     _data{mx::exchange(other._data, nullptr)},
			  _capacity{mx::exchange(other._capacity, 0U)} {

				// copy sizes
				for (mx::usz i = 0U; i < sizeof...(Tp); ++i)
					_sizes[i] = other._sizes[i];

				// copy offsets
				for (mx::usz i = 0U; i < sizeof...(Tp); ++i)
					_offsets[i] = other._offsets[i];
			}

			storage(const mx::usz (&sizes)[sizeof...(Tp)]) {
				mx::usz offset = 0U;
				mx::usz index  = 0U;

				// store sizes
				for (mx::usz i = 0U; i < sizeof...(Tp); ++i)
					_sizes[i] = sizes[i];

				// compute offsets
				((_offsets[index] = offset = mx::align_up<Tp>(offset),
				 (offset += sizes[index] * sizeof(Tp)), ++index), ...);

				_capacity = offset;

				_data = mx::allocate<mx::u8>(_capacity);

				//mx::bzero(_data, _capacity);
			}

			~storage(void) noexcept {
				if (_data != nullptr)
					mx::deallocate(_data);
			}


			auto resize(const mx::usz (&sizes)[sizeof...(Tp)]) -> void {

				mx::usz offset = 0U;
				mx::usz index  = 0U;

				// store sizes
				for (mx::usz i = 0U; i < sizeof...(Tp); ++i)
					_sizes[i] = sizes[i];

				// compute offsets
				((_offsets[index] = offset = mx::align_up<Tp>(offset),
				 (offset += sizes[index] * sizeof(Tp)), ++index), ...);

				if (offset <= _capacity)
					return;

				_capacity = offset;

				_data = mx::reallocate(_data, _capacity);
			}



			template <typename T>
			auto ptr_of(void) noexcept -> T* {
				static_assert(mx::all_unique<Tp...>, "ambiguous type access in storage");
				return reinterpret_cast<T*>(_data + _offsets[mx::index_of<T, Tp...>()]);
			}

			template <typename T>
			auto ptr_of(void) const noexcept -> const T* {
				static_assert(mx::all_unique<Tp...>, "ambiguous type access in storage");
				return reinterpret_cast<const T*>(_data + _offsets[mx::index_of<T, Tp...>()]);
			}

			template <mx::usz I>
			auto ptr_of(void) noexcept -> typename mx::type_at<I, Tp...>* {
				using T = typename mx::type_at<I, Tp...>;
				return reinterpret_cast<T*>(_data + _offsets[I]);
			}

			template <mx::usz I>
			auto ptr_of(void) const noexcept -> const typename mx::type_at<I, Tp...>* {
				using T = typename mx::type_at<I, Tp...>;
				return reinterpret_cast<const T*>(_data + _offsets[I]);
			}

			template <typename T>
			auto count_of(void) const noexcept -> mx::usz {
				static_assert(mx::all_unique<Tp...>, "ambiguous type access in storage");
				return _sizes[mx::index_of<T, Tp...>()];
			}

			template <mx::usz I>
			auto count_of(void) const noexcept -> mx::usz {
				static_assert(I < sizeof...(Tp), "storage size index out of bounds");
				return _sizes[I];
			}


			template <typename T>
			auto _debug_type(void) const noexcept -> void {
				constexpr mx::usz o = mx::index_of<T, Tp...>();
				std::cout << "Type: " << o << " Range: ";
				// determine start and end
				mx::usz start = _offsets[o];
				mx::usz end   = (o + 1U < sizeof...(Tp) ? _offsets[o + 1U] : _capacity);
				mx::usz size = _sizes[o];
				// compute padding in sizeof T
				mx::usz padding = (end - start) / sizeof(T) - size;

				std::cout << "[" << start << ", " << size << ", " << padding << "]\r\n    ";

				T* ptr = reinterpret_cast<T*>(_data + start);
				for (mx::usz i = 0U; i < size; ++i) {
					std::cout << ptr[i] << " ";
				}
				std::cout << std::endl;

			}


			auto debug(void) const noexcept -> void {
				(this->_debug_type<Tp>(), ...);
			}
	};


	class midi2_engine final {


		private:



			class metrics final {

				private:

					/* ranges */
					mx::u8 _min_gr, _max_gr;
					mx::u8 _min_ch, _max_ch;
					mx::u8 _min_no, _max_no;

					/* strides */
					mx::u16 _stride_gr, _stride_ch;

					/* count */
					mx::u16 _count;


				public:

					// -- public lifecycle ------------------------------------

					/* default constructor */
					metrics(void) noexcept = default;


					auto no_range(const mx::u8 min, const mx::u8 max) noexcept -> void {
						_min_no = min;
						_max_no = max;
					}

					auto ch_range(const mx::u8 min, const mx::u8 max) noexcept -> void {
						_min_ch = min;
						_max_ch = max;
					}

					auto gr_range(const mx::u8 min, const mx::u8 max) noexcept -> void {
						_min_gr = min;
						_max_gr = max;
					}

					auto compute(void) noexcept -> void {

						// counts
						const mx::u16 num_no = _max_no - _min_no + 1U;
						const mx::u16 num_ch = _max_ch - _min_ch + 1U;
						const mx::u16 num_gr = _max_gr - _min_gr + 1U;

						// strides
						_stride_gr = num_ch * num_no;
						_stride_ch = num_no;

						_count = _stride_gr * num_gr;
					}

					auto to_index(const mx::u8 gr, const mx::u8 ch, const mx::u8 no) const noexcept -> mx::u16 {
						return static_cast<mx::u16>(
							((gr - _min_gr) * _stride_gr) +
							((ch - _min_ch) * _stride_ch) +
							 (no - _min_no)
						);
					}

					auto to_group(const mx::u16 index) const noexcept -> mx::u8 {
						return static_cast<mx::u8>(index / _stride_gr + _min_gr);
					}

					auto to_channel(const mx::u16 index) const noexcept -> mx::u8 {
						return static_cast<mx::u8>(((index % _stride_gr) / _stride_ch) + _min_ch);
					}

					auto to_note(const mx::u16 index) const noexcept -> mx::u8 {
						return static_cast<mx::u8>((index % _stride_ch) + _min_no);
					}

					auto count(void) const noexcept -> mx::u16 {
						return _count;
					}

			};

			enum action : mx::u8 {
				none   = 0U,
				on     = 0b01U,
				off    = 0b10U,
				retrig = on | off
			};

			struct state final {
				mx::u16 ticks;
				action  action;
			}; // struct state
			

			metrics            _metrics;
			mx::storage<state, mx::u16> _storage;

			mx::u16 _count;



		public:

			midi2_engine(void) {

				const auto total = _metrics.count();

				// compute layout
				const mx::usz states_bytes  = total * sizeof(state);
				const mx::usz active_bytes  = total * sizeof(mx::u16);
				const mx::usz active_offset = mx::align_up<mx::u16>(states_bytes);
				const mx::usz total_bytes   = active_offset + active_bytes;

				// allocate
				_storage.resize({total, total});


				// initialize states
				//for (mx::u16 i = 0U; i < total; ++i) {
				//	_states[i].ticks = 0U;
				//	_states[i].epoch = 0U;
				//}

				_count = 0U;

			}


			auto note_on(const mx::u8 gr,
					     const mx::u8 ch,
						 const mx::u8 no,
						 const mx::u8 ga
						 ) noexcept -> void {

				// get index
				const auto i = _metrics.to_index(gr, ch, no);

				// get state
				auto& st = _storage.ptr_of<state>()[i];

				// ensure gate is not zero
				if (ga == 0U)
					return;

				// check if already note on
				if (st.action & action::on) {
					// extend gate
					st.ticks = ga;
					return;
				}

				// check if not already active
				if (st.ticks == 0U) {
					_storage.ptr_of<mx::u16>()[_count++] = i;
					st.action = action::on;
				}
				else {
					// retrig
					st.action = action::retrig;
				}

				st.ticks = ga;
			}


			template <typename F, typename... Ts>
			auto process(F&& emit, Ts&&... args) -> void {
				mx::u16 i = 0U;

				// setup pointers
				auto* states = _storage.ptr_of<state>();
				auto* active = _storage.ptr_of<mx::u16>();

				while (i < _count) {

					const mx::u16 a = active[i];
					auto& st = states[a];

					const auto gr = _metrics.to_group(a);
					const auto ch = _metrics.to_channel(a);
					const auto no = _metrics.to_note(a);

					if (st.action == action::retrig) {
						//emit(action::note_off, g, ch, no, st);
						//emit(action::note_on, g, ch, no, st);
						st.action = action::none;
						++i;
						continue;
					}

					if (st.action == action::on) {
						//emit(action::note_on, g, ch, no, st);
						st.action = action::none;
						++i;
						continue;
					}

					if (--st.ticks == 0U) {
					//	emit(action::note_off, g, ch, no, st);
						//active[i] = _active[--_active_count];
						continue;
					}

					++i;
				}
			}

	};

} // namespace mx

#endif // midi_midi_engine_hpp
