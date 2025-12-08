#ifndef language_ast_play_ctx_hpp
#define language_ast_play_ctx_hpp

#include "core/types.hpp"
#include "language/syntax/parameter.hpp"
#include "language/tokens/tokens.hpp"
#include "language/ast/hash_run.hpp"
#include "midi/midi_engine.hpp"
#include "math.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {

	class tree;

	struct average final {
		mx::usz count;
		mx::isz sum;

		average(void) noexcept
		: count{0U}, sum{0} {
		}

		auto add(const mx::i8 v) noexcept -> void {
			sum += v;
			++count;
		}

		auto get_or(const mx::i8 def) const noexcept -> mx::i8 {
			return count == 0U
				? def
				: static_cast<mx::i8>(
						sum / static_cast<mx::isz>(count)
				  );
		}

		auto reset(void) noexcept -> void {
			count = 0U; sum = 0;
		}
	};

	struct notes final {

		mx::u8 states[128U];
		mx::i8 active[128U];
		mx::u8 count;

		notes(void) noexcept
		: states{}, active{}, count{0U} {
		}

		auto add(const mx::i8 n) noexcept -> void {

			if (states[n] == 0U) {
				states[n] = 1U;
				active[count] = n;
				++count;
			}
		}

		auto reset(void) noexcept -> void {
			for (mx::u8 i = 0U; i < count; ++i) {
				const mx::i8 n = active[i];
				states[n] = 0U;
			}
			count = 0U;
		}
	};

	struct channels final {
		mx::u8 states[16U];
		mx::i8 active[16U];
		mx::u8 count;

		channels(void) noexcept
		: states{}, active{}, count{0U} {
		}

		auto add(const mx::i8 c) noexcept -> void {

			if (states[c] == 0U) {
				states[c] = 1U;
				active[count] = c;
				++count;
			}
		}

		auto reset(void) noexcept -> void {
			for (mx::u8 i = 0U; i < count; ++i) {
				const mx::i8 c = active[i];
				states[c] = 0U;
			}
			count = 0U;
		}
	};

	struct event final {

		/* values */
		//mx::usz values[pa::max_params];

		/* trig */
		bool _trig;

		/* notes */
		as::notes _nts;

		/* gate */
		as::average _ga;

		/* velo */
		as::average _vl;

		/* octa */
		mx::i8 _oc;

		/* semi */
		mx::i8 _se;

		/* chan */
		as::channels _chs;

		/* prob */
		as::average _pr;




		event(void) noexcept
		: _trig{false},
		  _nts{},
		  _ga{},
		  _vl{},
		  _oc{0},
		  _se{0},
		  _chs{},
		  _pr{} {
		}

		auto trig(void) noexcept -> void {
			_trig = true;
		}

		auto flush(mx::midi_engine& engine) noexcept -> void {

			static bool init = false;
			if (init == false) {
				::srand(::getpid());
				init = true;
			}

			if (_trig == false)
				return;

			mx::i8 oc = _oc;
			mx::i8 se = _se;
			mx::i8 ga = _ga.get_or(50);
			mx::i8 vl = _vl.get_or(100);
			mx::i8 pr = _pr.get_or(100);


			// convert gate as ticks
			//mx::u32 gate = (MIDI_PPQN * static_cast<mx::u32>(ga)) / 100U;


			// compute probability
			const mx::i8 rp = static_cast<mx::i8>(::rand() % 100U);
			if (rp >= pr) {
				return;
			}

			if (_nts.count == 0U)
				_nts.add(60);
			if (_chs.count == 0U)
				_chs.add(0);
			//std::cout << "Gate: " << gate << '\n';

			for (mx::u8 i = 0U; i < _chs.count; ++i) {
				const mx::i8 ch = _chs.active[i];

				for (mx::u8 j = 0U; j < _nts.count; ++j) {
					mx::i8 nt = _nts.active[j];

					nt += (oc * 12);
					nt += se;

					// probability, will be implemented later...

					engine.note_on(ch, nt, vl, ga);
				}
			}
		}


		auto add(const pa::id id, const mx::i8 va) noexcept -> void {

			switch (id) {

				case pa::note:
					_nts.add(va);
					break;

				case pa::gate:
					_ga.add(va);
					break;

				case pa::velo:
					_vl.add(va);
					break;

				case pa::octa:
					_oc += va; // not good at evaluation rate // maybe OK
					break;

				case pa::semi:
					_se += va; // not good at evaluation rate // maybe OK
					break;

				case pa::chan:
					_chs.add(va);
					break;

				case pa::prob:
					_pr.add(va);
					break;


				default:
					break;
			}
		}



		auto has_trig(void) const noexcept -> bool {
			return _trig;
		}
	};



	struct frame2 final {
		mx::usz node;
		mx::usz hash;
		mx::frac time;
		mx::frac speed;

		frame2(void) noexcept
		: node{0U},
		  hash{0U},
		  time{},
		  speed{1U, 1U} {
		}

		frame2(const mx::usz node,
			  const mx::frac& time) noexcept
		: node{node},
		  hash{0U},
		  time{time},
		  speed{1U, 1U} {
		}

		frame2(const mx::usz node,
			  const mx::usz hash,
			  const mx::frac& time,
			  const mx::frac& speed) noexcept
		: node{node},
		  hash{hash},
		  time{time},
		  speed{speed} {
		}
	};



	struct play_ctx final {

		const as::tree& tree;
		const tk::tokens& tokens;

		mx::string& hi;
		mx::midi_engine& engine;
		inline static std::vector<frame2> stack{};
		inline static as::hash_run hashes{};

		inline static std::vector<as::event> events{};
		as::frame2 fr;

		mx::frac absolute;

		play_ctx(const as::tree& t, const tk::tokens& tks, mx::string& h, mx::midi_engine& e) noexcept
		: tree{t}, tokens{tks}, hi{h}, engine{e}, fr{} {
		}


		auto push(const mx::usz node,
				  const mx::frac& time,
				  const mx::frac& speed) -> void {
			stack.emplace_back(
					node, hashes.hash_combine(fr.hash, node),
					time, speed);
		}

		auto push(const mx::usz node, const mx::frac& time) -> void {
			stack.emplace_back(node, time);
		}

		auto next(void) -> bool {
			if (stack.empty() == true)
				return false;
			fr = stack.back();
			stack.pop_back();
			return true;
		}
		auto next_until(const mx::usz mark) -> bool {
			if (!(stack.size() > mark))
				return false;
			fr = stack.back();
			stack.pop_back();
			return true;
		}

	};





} // namespace as

#endif // language_ast_play_ctx_hpp
