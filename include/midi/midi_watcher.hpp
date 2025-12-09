#ifndef midi_watcher_hpp
#define midi_watcher_hpp

#include "core/types.hpp"
#include "midi/constant.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	class midi_watcher {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::midi_watcher;

			/* emit type */
			using emit_fn = auto (self::*)(void) -> void;


			// -- private members ---------------------------------------------

			/* sysex flag */
			bool _sysex;

			/* status */
			emit_fn _emit;

			/* data bytes */
			mx::u8 _data[2U];

			/* needed bytes */
			mx::u8 _needed;

			/* index */
			mx::u8 _index;


			/* system real-time message */
			auto _system_realtime(const mx::u8 rt) -> void {


				switch (rt) {

					// -- system real-time messages ---------------------------

					// timing clock
					case 0xF8U:
						this->on_clock();
						break;

					// start
					case 0xFAU:
						this->on_start();
						break;

					// continue
					case 0xFBU:
						this->on_continue();
						break;

					// stop
					case 0xFCU:
						this->on_stop();
						break;

					// active sensing
					case 0xFEU:
						break;

					// system reset
					case 0xFFU:
						break;

					default:
						break;
				}
			}


			/* system common message */
			auto _system_common(const mx::u8 byte) -> void {


				switch (byte) {

					// -- system exclusive message ----------------------------

					// system exclusive
					case 0xF0U:
						_sysex = true;
						break;


					// -- system common messages ------------------------------

					// midi time code quarter frame
					case 0xF1U:
						_needed = 1U;
						_emit = &self::_emit_mtc;
						break;

					// song position pointer
					case 0xF2U:
						_needed = 2U;
						_emit = &self::_emit_spp;
						break;

					// song select
					case 0xF3U:
						_needed = 1U;
						break;

					// tune request
					case 0xF6U:
						_needed = 0U;
						break;

					// end of exclusive
					case 0xF7U:
						_sysex = false;
						_needed = 0U;
						// emit sysex message here
						break;
				}
			}


			/* channel voice message */
			auto _channel_voice(const mx::u8 byte) -> void {

				switch (byte) {

					// -- channel voice messages ------------------------------

					// note off
					case 0x80U:
						_needed = 2U;
						break;

					// note on
					case 0x90U:
						_needed = 2U;
						break;

					// polyphonic pressure
					case 0xA0U:
						_needed = 2U;
						break;

					// control change
					case 0xB0U:
						_needed = 2U;
						break;

					// program change
					case 0xC0U:
						_needed = 1U;
						break;

					// channel pressure
					case 0xD0U:
						_needed = 1U;
						break;

					// pitch bend
					case 0xE0U:
						_needed = 2U;
						break;
				}
			}


			auto _feed_data(const mx::u8 byte) -> void {

				// sysex data byte
				if (_sysex) {
					// accumulate sysex data here
					return;
				}

				// check if data is needed
				if (_index >= _needed)
					return;

				// store data byte
				_data[_index] = byte;
				++_index;

				// check if message is complete
				if (_index != _needed)
					return;

				_index  = 0U;
				_needed = 0U;

				if (_emit == nullptr)
					return;

				// emit complete message here
				(this->*_emit)();

				_emit = nullptr;
			}


			auto _emit_mtc(void) -> void {
				this->on_mtc(_data[0U]);
			}

			auto _emit_spp(void) -> void {
				const mx::u16 spp = static_cast<mx::u16>(_data[0U])
								 | (static_cast<mx::u16>(_data[1U]) << 7U);

				this->on_spp(spp);
			}

			auto _emit_sysex(void) -> void {
				// emit sysex message here
			}



		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			midi_watcher(void) noexcept
			: _sysex{false},
			  _emit{nullptr},
			  _data{0U, 0U},
			  _needed{0U},
			  _index{0U} {
			}

			auto feed(const mx::u8 byte) -> void {

				// real-time message
				if (byte >= 0xF8U) {
					_system_realtime(byte);
					return;
				}

				// status byte
				if (byte & 0x80U) {

					 _index  = 0U;
					 _needed = 0U;
					 _emit   = nullptr;

					// channel voice message
					if (byte <= 0xEFU) {
						_channel_voice(byte & 0xF0U);
						return;
					}

					// system common message
					_system_common(byte);
					return;
				}

				// data byte
				_feed_data(byte);
			}

		public:

			virtual ~midi_watcher(void) noexcept = default;

			virtual auto on_clock(void) -> void = 0;
			virtual auto on_start(void) -> void = 0;
			virtual auto on_continue(void) -> void = 0;
			virtual auto on_stop(void) -> void = 0;

			virtual auto on_mtc(const mx::u8) -> void = 0;
			virtual auto on_spp(const mx::u16) -> void = 0;
	};

} // namespace mx

#endif // midi_watcher_hpp
