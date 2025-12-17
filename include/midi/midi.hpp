#ifndef midi_hpp
#define midi_hpp

#include "os.hpp"

#if defined(midx_macos)

#include "coremidi/client.hpp"
#include "coremidi/source.hpp"
#include "coremidi/destination.hpp"
#include "coremidi/output.hpp"
#include "coremidi/events.hpp"

#include "coremidi/coremidi.hpp"

#include "midi/midi_watcher.hpp"
#include "midi/midi_engine.hpp"



// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- M I D I -------------------------------------------------------------

	class midi final : public mx::midi_watcher {


		private:

			// -- private members ---------------------------------------------

			/* srcs */
			std::vector<cm::source> _srcs;

			/* destinations */
			std::vector<cm::destination> _dsts;

			/* client */
			cm::client _client;

			/* source */
			cm::source _src;

			/* destination */
			cm::destination _dst;

			/* output */
			cm::output _out;

			/* packet list */
			cm::event_list _evl;

			/* engine */
			mx::midi_engine _engine;

			/* current source */
			cm::source* _cur_src;

			/* current destination */
			cm::destination* _cur_dst;


		public:

			// -- public lifecycle --------------------------------------------
			
			/* default constructor */
			midi(void)
			: _srcs{cm::sources()},
			  _dsts{cm::destinations()},

			  _client{"midx"},

			  _src{_client, "midx-out"},
			  _dst{_client, "midx-in", *this},

			  _out{_client, "midx-output-port"},

			  _evl{},
			  _engine{},

			  _cur_src{nullptr},
			  _cur_dst{nullptr} {

				_cur_src = &_src;

				if (!_dsts.empty())
					_cur_dst = &_dsts.back();
			}


			/* start */
			auto start(void) -> void {

				// add clock start event
				//_evl.start();

				// send events
				_evl.send(*_cur_src);
				if (_cur_dst)
					_evl.send(_out, *_cur_dst);
				_evl.clear();
			}

			/* stop */
			auto stop(void) -> void {



				// note off all notes
				_engine.off_all(_evl);

				// add clock stop event
				//_evl.stop();

				// send events
				_evl.send(*_cur_src);
				if (_cur_dst)
					_evl.send(_out, *_cur_dst);
				_evl.clear();
			}

			/* flush */
			auto flush(void) -> void {

				// send events
				_evl.send(*_cur_src);
				if (_cur_dst)
					_evl.send(_out, *_cur_dst);
				_evl.clear();

				// next epoch
				_engine.next_epoch();
			}


			/* note on */
			auto note_on(const mx::midi_event& ev, const mx::u64 ts) -> void {
				_engine.note_on(_evl, ev, ts);
			}

			/* off pass */
			auto off_pass(const mx::u64 ts) -> void {
				_engine.off_pass(_evl, ts);
			}


			// -- public overrides --------------------------------------------

			/* on clock */
			auto on_clock(void) -> void override final {
				std::cout << "\x1b[32mMIDI CLOCK\x1b[0m" << "\r\n";
			}

			/* on start */
			auto on_start(void) -> void override final {
				std::cout << "\x1b[34mMIDI START\x1b[0m" << "\r\n";
			}

			/* on continue */
			auto on_continue(void) -> void override final {
				std::cout << "\x1b[33mMIDI CONTINUE\x1b[0m" << "\r\n";
			}

			/* on stop */
			auto on_stop(void) -> void override final {
				std::cout << "\x1b[31mMIDI STOP\x1b[0m" << "\r\n";
			}

			/* on mtc */
			auto on_mtc(const mx::u8 frame) -> void override final {
				std::cout << "\x1b[36mMIDI MTC\x1b[0m: " << static_cast<unsigned>(frame) << "\r\n";
			}

			/* on spp */
			auto on_spp(const mx::u16 spp) -> void override final {

				// convert in ticks
				const auto ticks = static_cast<mx::u32>(spp) * (MIDI_PPQN / 4U);

				std::cout << "\x1b[35mMIDI SPP\x1b[0m: " << ticks << " ticks\r\n";
			}


	}; // class midi

} // namespace mx

#endif // midx_macos

#endif // midi_hpp
