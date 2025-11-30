#ifndef language_ast_eval_hpp
#define language_ast_eval_hpp

#include "language/ast/hash_run.hpp"
#include "midi/midi_engine.hpp"
#include "math.hpp"
#include "language/ast/param_accum.hpp"


// -- forward declarations ----------------------------------------------------

namespace tk {
	class tokens;
}
namespace mx {

	class random final {

		private:
			using self = mx::random;

			static auto _shared(void) noexcept -> self& {
				static self ins;
				return ins;
			}

			random(void) noexcept {
				::srand(::getpid());
			}

			auto _rand(void) const noexcept -> int {
				return ::rand();
			}
		public:

			// -- public static methods ---------------------------------------

			static auto gen(void) noexcept -> int {
				return _shared()._rand();
			}

	}; // class random
}


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- forward declarations ------------------------------------------------

	class tree;


	struct frame final {
		mx::usz node;
		mx::usz hash;
		mx::frac time;
		mx::frac speed;

		frame(void) noexcept
		: node{0U},
		  hash{0U},
		  time{},
		  speed{1U, 1U} {
		}

		frame(const mx::frac& time) noexcept
		: node{0U}, // root node
		  hash{0U},
		  time{time},
		  speed{1U, 1U} {
		}

		frame(const mx::usz node,
			  const mx::usz hash,
			  const mx::frac& time,
			  const mx::frac& speed) noexcept
		: node{node},
		  hash{hash},
		  time{time},
		  speed{speed} {
		}

			/* compute_hash
			   compute a hash for a given frame */
			static auto compute_hash(mx::u64 h, const mx::u64 v) noexcept -> mx::usz {
				h ^= v + 0x9e3779b97f4a7c15ULL
					   + (h << 6U)
					   + (h >> 2U);
				return h;
			}

			/* local time
			   compute the local time of the frame */
			auto local_time(void) const noexcept -> mx::frac {
				return time * speed;
			}

			/* fork
			   create a new frame with given node and time */
			auto fork(const mx::usz n,
					  const mx::frac& t) const noexcept -> as::frame {
				return as::frame{n, compute_hash(hash, n),
								(t / speed), speed};
			}

			/* fork with speed
			   create a new frame with given node, time and speed */
			auto fork(const mx::usz n,
					  const mx::frac& t,
					  const mx::frac& s) const noexcept -> as::frame {
				return as::frame{n, compute_hash(hash, n),
								(t / speed), s * speed};
			}

			/* forward
			   create a new frame with given node and scaled time */
			auto forward(const mx::usz n,
						 const mx::frac& t) const noexcept -> as::frame {
				return as::frame{n, compute_hash(hash, n), t, speed};
			}
	};





	struct track_result {

		as::trig_accum  trig;
		as::note_accum  note;
		as::chan_accum  chan;

		as::velo_accum  velo;
		as::gate_accum  gate;
		as::prob_accum  prob;

		as::octa_accum  octa;
		as::semi_accum  semi;


		auto flush(mx::midi_engine& engine) -> void {

			if (trig.edge == false || trig.value == 0U)
				return;

			mx::u8 vl = velo.value();
			mx::i8 oc = octa.value();
			// compute final offset
			oc *= 12;

			note.ensure_default();
			chan.ensure_default();


			for (mx::u8 i = 0U; i < chan.count; ++i) {
				const mx::u8 ch = chan.active[i];

				for (mx::u8 j = 0U; j < note.count; ++j) {
					mx::u8 nt = note.active[j];
					nt += oc;

					engine.note_on(ch, nt, vl, 40U);
				}
			}
		}
	};

	struct expr_result {

		private:

			using self = as::expr_result;


			/* maximum notes (16 channels x 128 notes = 2048) */
			constexpr static mx::usz max_notes = 16U * 128U;

			struct event final {
				mx::u16 gate;
				mx::u16 velocity;
				mx::u16 count;
				event(void) noexcept
				: gate{0U}, velocity{0U}, count{0U} {
				}
			}; // struct state

			event   _events[max_notes];
			mx::u16 _active[max_notes];
			mx::u16 _count;


			static auto to_idx(const mx::u8 ch,
							   const mx::u8 no) noexcept -> mx::u16 {
				return (static_cast<mx::usz>(ch) << 7U) | no;
			}
			static auto to_ch(const mx::u16 index) noexcept -> mx::u8 {
				return static_cast<mx::u8>(index >> 7U);
			}
			static auto to_no(const mx::u16 index) noexcept -> mx::u8 {
				return static_cast<mx::u8>(index & 0x7FU);
			}


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			expr_result(void) noexcept
			: _events{},
			  _active{},
			  _count{0U} {
			}


			auto accumulate(as::track_result& tr) noexcept -> void {

				// check trigger
				if (!tr.trig.edge || !tr.trig.value)
					return;

				// check probability
				const auto prob = tr.prob.value();
				if (prob < 100U) {
					const auto r = mx::random::gen() % 100U;
					if (r >= prob)
						return;
				}

				// ensure defaults
				tr.note.ensure_default();
				tr.chan.ensure_default();


				for (mx::u8 i = 0U; i < tr.chan.count; ++i) {

					const mx::u8 ch = tr.chan.active[i];

					for (mx::u8 j = 0U; j < tr.note.count; ++j) {

						mx::u8 nt = tr.note.active[j];

						nt = tr.octa.apply_on(nt);
						nt = tr.semi.apply_on(nt);

						const mx::u16 idx = to_idx(ch, nt);

						auto& ev = _events[idx];

						ev.gate     += tr.gate.value();
						ev.velocity += tr.velo.value();
						++ev.count;

						// first time active
						if (ev.count == 1U) {
							_active[_count] = idx;
							++_count;
						}
					}
				}
			}

			/* merge
			   merge another expression result into this one */
			auto merge(const self& other) noexcept -> void {

				for (mx::u16 i = 0U; i < other._count; ++i) {
					const mx::u16 idx = other._active[i];
					auto& ev = _events[idx];
					const auto& oev = other._events[idx];

					// check if first time active
					if (ev.count == 0U) {
						_active[_count] = idx;
						++_count;
					}
					ev.count    += oev.count;
					ev.gate     += oev.gate;
					ev.velocity += oev.velocity;
				}
			}

			auto flush(mx::midi_engine& engine) -> void {

				for (mx::u16 i = 0U; i < _count; ++i) {
					const mx::u16 idx = _active[i];
					auto& ev = _events[idx];

					const mx::u8 ch = to_ch(idx);
					const mx::u8 nt = to_no(idx);

					// compute average gate and velocity
					const mx::u8 gt = static_cast<mx::u8>(ev.gate / ev.count);
					const mx::u8 vl = static_cast<mx::u8>(ev.velocity / ev.count);


					engine.note_on(ch, nt, vl, gt);

					// reset event
					ev = {};
				}

				_count = 0U;
			}

			auto is_empty(void) const noexcept -> bool {
				return _count == 0U;
			}
	};




	// -- E V A L -------------------------------------------------------------

	class eval final {


		private:

			// -- private members ---------------------------------------------

			/* tree reference */
			const as::tree* _tree;

			/* tokens reference */
			const tk::tokens* _tokens;

			/* hashes */
			as::hash_run _hashes;

			/* engine */
			mx::midi_engine* _engine;

			/* highlight stream */
			std::string* _hi;

			struct cross_state {
				mx::frac time;
				mx::f64 error;
				mx::usz  side;
				cross_state(void) noexcept
				: time{}, error{0.0}, side{0U} {
				}

				auto evaluate(const mx::frac& local, const mx::f64 progress) noexcept -> void {

					// reset error on time rewind
					if (local < time) {
						std::cout << "\n\x1b[32mResetting crossfade error\x1b[0m\n";
						error = 0.0;
						side  = 0U;
					}

					time = local;

					const auto value = progress + error;

					if (value >= 0.5) {
						side  = 1U;
						error = value - 1.0;
					} else {
						side  = 0U;
						error = value;
					}
				}
			};

			std::unordered_map<mx::usz, cross_state> _cross;

		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			eval(void) noexcept;



			// -- public methods ----------------------------------------------

			/* init
			   initialize evaluator */
			auto init(as::tree&, const tk::tokens&) noexcept -> void;


			/* evaluate
			   evaluate the AST and produce MIDI events */
			auto evaluate(std::string&,
						   mx::midi_engine&,
					 const mx::frac&) -> void;


			/* dispatch
			   dispatch evaluation based on node type */
			template <typename T>
			auto dispatch(const as::frame&, T&) -> void;




			/* atomics
			   evaluate an atomics node */
			template <typename T>
			auto atomics(const as::frame&, T&) -> void;



			/* group
			   evaluate a group node */
			template <typename T>
			auto group(const as::frame&, T&) -> void;

			/* tempo
			   evaluate a tempo node */
			template <typename T>
			auto tempo(const as::frame&, T&) -> void;

			/* modulo
			   evaluate a modulo node */
			template <typename T>
			auto modulo(const as::frame&, T&) -> void;



			/* parallel
			   evaluate a parallel node */
			template <typename T>
			auto parallel(const as::frame&, T&) -> void;

			/* crossfade
			   evaluate a crossfade node */
			template <typename T>
			auto crossfade(const as::frame&, T&) -> void;



			/* track
			   evaluate a track node */
			template <typename T>
			auto track(const as::frame&, T&) -> void;

			/* param
			   evaluate a parameter node */
			template <typename T>
			auto param(const as::frame&, T&) -> void;



			/* references
			   evaluate a reference node */
			template <typename T>
			auto references(const as::frame&, T&) -> void;

			/* program
			   evaluate a program node */
			template <typename T>
			auto program(const as::frame&, T&) -> void;




			// -- public accessors --------------------------------------------

			/* is evaluable
			   check if the evaluator is initialized */
			auto is_evaluable(void) const noexcept -> bool {
				return _tree != nullptr && _tokens != nullptr;
			}


	}; // class eval

} // namespace as

#endif // language_ast_eval_hpp
