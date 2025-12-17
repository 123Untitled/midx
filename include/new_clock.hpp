#ifndef new_clock_hpp
#define new_clock_hpp

#include "core/types.hpp"
#include "system/unix_pipe.hpp"
#include "math.hpp"
#include "midi/constant.hpp"

#include <atomic>
#include <stdexcept>

#include <AudioUnit/AudioUnit.h>
#include <CoreAudio/CoreAudio.h>
#include <AudioToolbox/AudioToolbox.h>

#include "time/timebase.hpp"

namespace au {

	using render_action_flags = ::AudioUnitRenderActionFlags;

} // namespace au

namespace mx {

	inline auto bpm_to_frac(const double bpm) noexcept -> mx::frac {
		constexpr mx::u64 scale = 1'000'000'000ULL;
		return mx::frac{
			static_cast<mx::u64>((bpm * scale) + 0.5),
			scale
		};
	}





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

	};
	
} // namespace mx


namespace ca {

	using os_status = ::OSStatus;
	using audio_buffer_list = ::AudioBufferList;


	class audio_unit final {

		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ca::audio_unit;


			// -- private members ---------------------------------------------

			/* audio unit */
			::AudioUnit _au;

			/* initialized */
			bool _initialized;

			/* started */
			bool _started;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			audio_unit(void)
			: _au{nullptr},
			  _initialized{false},
			  _started{false} {

				const ::AudioComponentDescription desc {
					.componentType         = kAudioUnitType_Output,
					.componentSubType      = kAudioUnitSubType_DefaultOutput,
					.componentManufacturer = kAudioUnitManufacturer_Apple,
					.componentFlags        = 0U,
					.componentFlagsMask    = 0U
				};

				const ::AudioComponent comp = ::AudioComponentFindNext(nullptr, &desc);

				if (comp == nullptr)
					throw std::runtime_error{"AudioComponentFindNext failed"};


				const ca::os_status err = ::AudioComponentInstanceNew(comp, &_au);

				if (err != noErr)
					throw std::runtime_error{"AudioComponentInstanceNew failed"};
			}


			template <typename T>
			auto set_property(const ::AudioUnitPropertyID   id,
							  const ::AudioUnitScope     scope,
							  const ::AudioUnitElement element,
							  const T&                    data) -> void {

				const auto err = ::AudioUnitSetProperty(_au, id, scope, element,
														&data, sizeof(T));

				if (err != noErr)
					throw std::runtime_error{"AudioUnitSetProperty failed"};
			}

			template <typename T>
			auto set_render_callback(
					const ::AURenderCallback cb, T& refcon) -> void {

				const ::AURenderCallbackStruct rcb {
					.inputProc        = cb,
					.inputProcRefCon = static_cast<void*>(&refcon)
				};

				const auto err = ::AudioUnitSetProperty(_au,
													   kAudioUnitProperty_SetRenderCallback,
													   kAudioUnitScope_Input,
													   0U,
													   &rcb,
													   sizeof(::AURenderCallbackStruct));

				if (err != noErr)
					throw std::runtime_error{"AudioUnitSetProperty(SetRenderCallback) failed"};
			}


			auto initialize(void) -> void {

				if (_initialized)
					return;

				const auto err = ::AudioUnitInitialize(_au);

				if (err != noErr)
					throw std::runtime_error{"AudioUnitInitialize failed"};

				_initialized = true;
			}

			auto start(void) -> void {

				if (!_initialized)
					this->initialize();

				const auto err = ::AudioOutputUnitStart(_au);

				if (err != noErr)
					throw std::runtime_error{"AudioOutputUnitStart failed"};

				_started = true;
			}

			auto stop(void) -> void {

				if (!_started)
					return;

				const auto err = ::AudioOutputUnitStop(_au);

				if (err != noErr)
					throw std::runtime_error{"AudioOutputUnitStop failed"};

				_started = false;
			}


			~audio_unit(void) noexcept {

				if (_au == nullptr)
					return;

				// stop
				if (_started == true)
					static_cast<void>(::AudioOutputUnitStop(_au));

				// uninitialize
				if (_initialized == true)
					static_cast<void>(::AudioUnitUninitialize(_au));

				// dispose
				static_cast<void>(::AudioComponentInstanceDispose(_au));
			}




	}; // class audio_unit


	class audio final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ca::audio;


			// -- private members ---------------------------------------------

			/* running */
			std::atomic<bool> _running;

			/* bpm */
			double _bpm;

			/* sample rate */
			double _sample_rate;

			/* host time per sample */
			double _host_time_per_sample;

			/* sample per tick */
			mx::frac _samples_per_tick;

			/* sample counter */
			mx::frac _sample_counter;

			/* audio unit */
			ca::audio_unit _au;

			/* lock free queue */
			mx::lock_free<mx::u64, 1024U> _queue;

			/* pipe */
			mx::unix_pipe _pipe;


			// -- private static methods --------------------------------------

			static auto _audio_callback(void* in_ref_con,
										au::render_action_flags* flags,
										const AudioTimeStamp* timestamp,
										UInt32 bus_number,
										UInt32 num_frames,
										ca::audio_buffer_list* data) noexcept -> ca::os_status {

				// fill buffer with silence
				for (UInt32 i = 0U; i < data->mNumberBuffers; ++i) {

					float* out = static_cast<float*>(data->mBuffers[i].mData);
					const UInt32 frames = data->mBuffers[i].mDataByteSize / sizeof(float);

					for (UInt32 f = 0; f < frames; ++f)
						out[f] = 0.0f;
				}

				// indicate output is silence
				*flags |= kAudioUnitRenderAction_OutputIsSilence;


				if (in_ref_con == nullptr)
					return noErr;

				// get self pointer
				self* audio = static_cast<self*>(in_ref_con);

				audio->_extract_ticks(num_frames, *timestamp);

				return noErr;
			}




			auto _extract_ticks(const ::UInt32 num_frames,
								const ::AudioTimeStamp& timestamp) -> ca::os_status {

				if (!_running.load(std::memory_order_acquire))
					return noErr;


				// host start time
				mx::u64 start_time = timestamp.mHostTime;

				mx::frac samples{num_frames, 1U};

				_sample_counter += samples;

				if (_sample_counter < _samples_per_tick)
					return noErr;

				do {
					// number of samples from buffer start to tick
					const mx::frac offset = _samples_per_tick - (_sample_counter - samples);

					// factor
					const double factor = static_cast<double>(offset.num) / static_cast<double>(offset.den);

					// exact timestamp of the tick
					const UInt64 tick_time = start_time + static_cast<UInt64>(factor * _host_time_per_sample);

					_queue.push(tick_time);

					// subtract counter
					_sample_counter -= _samples_per_tick;

				} while (_sample_counter >= _samples_per_tick);

				// write to pipe to notify tick availability
				static_cast<void>(::write(_pipe.wr(), "T", 1U));

				return noErr;
			}


			//static auto _sample_per_tick

		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			audio(void)
			: _running{false},
			  _bpm{130.0},
			  _sample_rate{48000.0},
			  _host_time_per_sample{mx::timebase::host_time_per_sample(_sample_rate)},
			  _samples_per_tick{static_cast<mx::usz>(_sample_rate * 60.0), static_cast<mx::usz>(_bpm
					  * MIDI_PPQN
					  )},
			  _sample_counter{0U, 1U},
			  _au{},
			  _queue{},
			  _pipe{mx::pipe()} {

				std::cout << "Host time per sample: " << _host_time_per_sample << "\r\n";
				std::cout << "Sample per tick: " << _samples_per_tick << "\r\n";
				std::cout << "Sample rate: " << _sample_rate << "\r\n";
				std::cout << "BPM: " << _bpm << "\r\n";


				_au.set_render_callback(&self::_audio_callback, *this);

				_pipe.non_blocking_wr();


				const ::AudioStreamBasicDescription asbd {
					.mSampleRate       = _sample_rate,
					.mFormatID         = kAudioFormatLinearPCM,
					.mFormatFlags      = kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked,

					.mBytesPerPacket   = sizeof(float),
					.mFramesPerPacket  = 1U,

					.mBytesPerFrame    = sizeof(float),
					.mChannelsPerFrame = 1U,

					.mBitsPerChannel   = mx::bits<float>,
					.mReserved         = 0U
				};

				_au.set_property(kAudioUnitProperty_StreamFormat,
								 kAudioUnitScope_Input, 0U, asbd);
			}


			auto start(void) -> void {

				_au.start();

				_running.store(true, std::memory_order_release);
			}

			auto stop(void) -> void {


				_au.stop();

				_running.store(false, std::memory_order_release);
			}

			/* destructor */
			~audio(void) noexcept = default;



			auto ident(void) const noexcept -> int {
				return _pipe.rd();
			}

			auto is_running(void) const noexcept -> bool {
				return _running.load(std::memory_order_acquire);
			}

			auto queue(void) noexcept -> mx::lock_free<mx::u64, 1024U>& {
				return _queue;
			}

			auto consume(void) const noexcept -> void {
				char c;
				::read(_pipe.rd(), &c, 1U);
			}

	}; // class audio

} // namespace ca

#endif // new_clock_hpp
