#include "language/ast/eval.hpp"
#include "language/ast/tree.hpp"
#include "language/highlight_tracker.hpp"


// -- E V A L -----------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
as::eval::eval(void) noexcept
: _tree{nullptr}, _tokens{nullptr},
  _hashes{},
  _hls{nullptr} {
}


// -- public methods ----------------------------------------------------------

/* init
   initialize evaluator */
auto as::eval::init(as::tree& tree,
					const tk::tokens& tokens,
					mx::highlight_tracker& hls) noexcept -> void {
	_tree   = &tree;
	_tokens = &tokens;
	_hls = &hls;

	_cross.clear();
	_hashes.clear();
}


/* evaluate
   evaluate the AST and produce MIDI events */
auto as::eval::evaluate(mx::midi_engine& engine,
					    const mx::frac& time) -> void {

	_engine = &engine;
	_absolute = time;

	//_hashes.swap_now();

	as::expr_result r;

	// call dispatch on root frame
	dispatch(as::frame{time}, r);

	r.flush(*_engine);
}


/* group
   evaluate a group node */
template <typename T>
auto as::eval::group(const as::frame& f, T& r) -> void {

	// get group node
	const auto& g = _tree->node<as::group>(f.node);

		  auto it  = g.range.start;
	const auto end = g.range.end();

	auto time = g.header.mod(f.local_time());

	// loop over child nodes
	for (; it < end; ++it) {
		const auto node = _tree->remap_index(it);
		const auto& dur = _tree->header(node).dur;

		if (time < dur) {
			as::eval::dispatch<T>(f.propagate(node, time), r);
			return;
		}

		time -= dur;
	}
}

/* tempo
   evaluate a tempo node */
template <typename T>
auto as::eval::tempo(const as::frame& f, T& r) -> void {

	const auto& t = _tree->node<as::tempo>(f.node);

	auto time = t.header.mod(f.local_time());

		  auto  it = t.frac_start;
		  auto  tk = t.token_start;
	const auto end = t.frac_start + t.count;

	// Multiple tempos play sequentially: find which tempo segment we're in
	// Each segment has duration = child.dur / tempo
	const auto& child_dur = _tree->header(t.child).dur;

	for (; it < end; ++it, ++tk) {
		const auto& tempo_frac = _tree->frac_at(it);

		// Duration of this tempo segment (wall-clock time)
		const auto segment_dur = child_dur / tempo_frac;

		if (time < segment_dur) {
			// We're in this tempo segment
			// Convert wall-clock time to child's local time: child_time = time * tempo
			const auto child_time = time * tempo_frac;

			// Calculate new tempo_factor for child
			// If parent has factor X, and we apply tempo Y:
			// 1 unit child time = (1/Y) units parent time = (1/Y * X) units global time
			const auto child_tempo_factor = f.tempo_factor / tempo_frac;

			// Calculate new modulo_limit for child
			// If parent has modulo_limit in wall-clock space, convert to child's local space
			// child_limit = parent_limit * tempo (because tempo speeds up time)
			const auto child_modulo_limit = f.modulo_limit * tempo_frac;

			// Evaluate child with new time, tempo_factor and modulo_limit
			as::eval::dispatch<T>(f.forward(t.child, child_time, child_tempo_factor, child_modulo_limit), r);

			if (has_edge(time, f.hash.re_hash(it))) {
				static int c = 0;
					std::cout << ++c << " EDGE\n";

					// highlight with expiration
					const auto actual_end = (f.modulo_limit < segment_dur) ? f.modulo_limit : segment_dur;
					const auto remaining_local = actual_end - time;
					const auto remaining_global = remaining_local * f.tempo_factor;
					const auto expire = _absolute + remaining_global;
					_hls->mark_active(tk, "Underlined", expire);
			}
			_hashes[f.hash] = time;


			break;
		}

		// Move to next segment
		time -= segment_dur;

	}

	// highlight with expiration
	//const auto& frac_value = _tree->frac_at(tk);
	//const auto expire = f.time + frac_value;
	//_hl_tracker->mark_active(tk, "Underlined", expire, *_hi);
}

/* modulo
   evaluate a modulo node */
template <typename T>
auto as::eval::modulo(const as::frame& f, T& r) -> void {

	const auto& m = _tree->node<as::modulo>(f.node);

	auto time = m.header.mod(f.local_time());

		  auto  it = m.frac_start;
		  auto  tk = m.token_start;
	const auto end = m.frac_start + m.count;

	for (; it < end; ++it, ++tk) {
		const auto& frac = _tree->frac_at(it);

		if (time < frac) {
			// Calculate modulo_limit for child: take minimum of parent limit and this modulo
			const auto child_modulo_limit = (frac < f.modulo_limit) ? frac : f.modulo_limit;
			//const auto child_modulo_limit = frac;

			// Evaluate child with modulo limit
			as::eval::dispatch<T>(f.forward(m.child, time, f.tempo_factor, child_modulo_limit), r);
			break;
		}
		time -= frac;
	}

	// highlight with expiration
	//const auto& frac_value = _tree->frac_at(tk);
	//const auto expire = f.time + frac_value;
	//_hl_tracker->mark_active(tk, "Underlined", expire, *_hi);
}

/* parallel
   evaluate a parallel node */
template <typename T>
auto as::eval::parallel(const as::frame& f, T& r) -> void {

	const auto& p = _tree->node<as::parallel>(f.node);

		  auto it   = p.range.start;
	const auto end  = p.range.end();
	const auto time = p.header.mod(f.time);

	// loop over child nodes
	for (; it < end; ++it) {
		const auto node = _tree->remap_index(it);

		as::eval::dispatch<T>(f.propagate(node, time), r);
	}
}

/* crossfade
   evaluate a crossfade node */
template <typename T>
auto as::eval::crossfade(const as::frame& f, T& r) -> void {

	const auto&  cf = _tree->node<as::crossfade>(f.node);
	const auto time = cf.header.mod(f.time);


	T lr, rr;
	as::eval::dispatch(f.propagate(cf.left,  time), lr);
	as::eval::dispatch(f.propagate(cf.right, time), rr);

	if constexpr (as::is_param_accum<T>) {

		// discrete (note, trig, chan)
		if constexpr (T::is_discrete) {

			auto& c = _cross[f.hash];

			if (lr.edge || rr.edge) {
				c.evaluate(time, cf.header.progress(time));
				//std::cout << (c.side ? "\x1b[35mB\x1b[0m" : "\x1b[33mA\x1b[0m") << std::flush;
			}

			// merge according to side
			r.merge(c.side ? rr : lr);
		}

		// continuous (velo, octa, semi, gate, prob)
		else if constexpr (T::is_continuous) {

			const auto progress = cf.header.progress2(time);

			// linear interpolation
			auto lvalue = lr.value();
			auto rvalue = rr.value();

			const auto interpolated = lvalue * (1.0 - progress) + rvalue * progress;
			// clear screen and go home
			std::cout << "\x1b[2K\x1b[H";
			std::cout << interpolated << " " << std::flush;

			r.accumulate(static_cast<typename T::type>(interpolated),
							lr.edge || rr.edge);
		}
	}

	else if constexpr (mx::is_same<T, as::expr_result>) {

		auto& c = _cross[f.hash];

		if (!lr.is_empty() || !rr.is_empty()) {
			c.evaluate(time, cf.header.progress(time));

			std::cout << (c.side ? "\x1b[35mB\x1b[0m" : "\x1b[33mA\x1b[0m") << std::flush;
		}

		r.merge(c.side ? rr : lr);
	}
}




/* atomics
   evaluate an atomics node */
template <typename T>
auto as::eval::atomics(const as::frame& f, T& r) -> void {

	// only for parameter accumulators
	if constexpr (as::is_param_accum<T>) {

		const auto& a = _tree->node<as::atomics>(f.node);

		const auto time = a.header.mod(f.local_time());
		const auto step = a.header.dur.num != 1U ? time.num / time.den : 0U;


		bool edge = has_edge(time, f.hash.re_hash(step));

		//const auto it   = _hashes.find(hash);
		//bool edge = true;

		//if (it != _hashes.end())
		//	edge = time < it->second;
		//
		//_hashes[hash] = time;

		// get value at step
		const typename T::type value = _tree->value_at/*<T::type>*/(a.value_start + step);

		r.accumulate(value, edge);

		if (!edge)
			return;

		// Calculate when this step expires (limited by modulo if present)
		const auto next_step = mx::frac{step + 1U, 1U};
		const auto actual_end = (f.modulo_limit < next_step) ? f.modulo_limit : next_step;

		const auto remaining_local = actual_end - time;
		const auto remaining_global = remaining_local * f.tempo_factor;
		const auto expire = _absolute + remaining_global;

		const char* group    = "CurSearch";
		_hls->mark_active(a.token_start + step, group, expire);
	}
}


template <typename T>
auto as::eval::param(const as::frame& f, T& r) -> void {

	const auto& p = _tree->node<as::parameter>(f.node);

	auto time = p.header.mod(f.local_time());

		  auto it  = p.range.start;
	const auto end = p.range.end();

	for (; it < end; ++it) {
		const auto node = _tree->remap_index(it);
		const auto& dur = _tree->header(node).dur;

		if (time < dur) {
			as::eval::dispatch<T>(f.propagate(node, time), r);
			return;
		}

		time -= dur;
	}
}


template <typename T>
auto as::eval::track(const as::frame& f, T& expr) -> void {

    const auto& tr = _tree->node<as::track>(f.node);

	const auto time = tr.header.mod(f.local_time());


	if constexpr (mx::is_same<T, as::expr_result>) {

		as::track_result r;

		if (tr.params[pa::trig]) {
			as::eval::param(f.propagate(tr.params[pa::trig],
										time), r.trig);
		}

		if (tr.params[pa::note]) {
			as::eval::param(f.propagate(tr.params[pa::note],
										time), r.note);
		}

		if (tr.params[pa::chan]) {
			as::eval::param(f.propagate(tr.params[pa::chan],
										time), r.chan);
		}

		if (tr.params[pa::velo]) {
			as::eval::param(f.propagate(tr.params[pa::velo],
										time), r.velo);
		}

		if (tr.params[pa::gate]) {
			const auto n = tr.params[pa::gate];
			as::eval::param(f.propagate(n, time), r.gate);
		}

		if (tr.params[pa::prob]) {
			const auto n = tr.params[pa::prob];
			as::eval::param(f.propagate(n, time), r.prob);
		}

		if (tr.params[pa::octa]) {
			const auto n = tr.params[pa::octa];
			as::eval::param(f.propagate(n, time), r.octa);
		}

		if (tr.params[pa::semi]) {
			const auto n = tr.params[pa::semi];
			as::eval::param(f.propagate(n, time), r.semi);
		}

		expr.accumulate(r);
		//r.flush(*_engine);
	}
}

/* references
   evaluate a reference node */
template <typename T>
auto as::eval::references(const as::frame& f, T& r) -> void {

	const auto& ref = _tree->node<as::references>(f.node);

		  auto it  = ref.ref_start;
		  auto tk  = ref.tok_start;
	const auto end = ref.ref_start + ref.count;

	auto time = ref.header.mod(f.local_time());

	// loop over referenced nodes
	for (; it < end; ++it, ++tk) {
		const auto node = _tree->ref_at(it);
		const auto& dur = _tree->header(node).dur;

		if (time < dur) {

			// highlight with expiration (limited by modulo if present)
			const auto actual_end = (f.modulo_limit < dur) ? f.modulo_limit : dur;

			const auto remaining_local = actual_end - time;
			const auto remaining_global = remaining_local * f.tempo_factor;
			const auto expire = _absolute + remaining_global;
			//_hls->mark_active(tk, "Underlined", expire);

			// evaluate referenced node
			as::eval::dispatch<T>(f.propagate(node, time), r);
			break;
		}
		time -= dur;
	}



	// highlight with expiration
	//if (node != 0U) {
	//	const auto expire = f.time + _tree->header(node).dur;
	//	_hl_tracker->mark_active(tk, "Underlined", expire, *_hi);
	//}
}


/* program
   evaluate a program node */
template <typename T>
auto as::eval::program(const as::frame& f, T& r) -> void {

	const auto& p = _tree->node<as::program>(f.node);

		  auto it  = p.range.start;
	const auto end = p.range.end();

	// loop over child nodes
	for (; it < end; ++it) {
		const auto node = _tree->remap_index(it);
		as::eval::dispatch(f.propagate(node, f.time), r); // optimize this
	}
}


template <typename T>
auto as::eval::dispatch(const as::frame& f, T& r) -> void {

	// get type of node
	const auto type = _tree->header(f.node).type;

	switch (type) {

		case as::type::atomics:
			as::eval::atomics<T>(f, r);
			return;

		case as::type::group:
			as::eval::group<T>(f, r);
			return;

		case as::type::parallel:
			as::eval::parallel<T>(f, r);
			return;

		case as::type::crossfade:
			as::eval::crossfade<T>(f, r);
			return;

		case as::type::track:
			as::eval::track<T>(f, r);
			return;

		case as::type::references:
			as::eval::references<T>(f, r);
			return;

		case as::type::tempo:
			as::eval::tempo<T>(f, r);
			return;

		case as::type::modulo:
			as::eval::modulo<T>(f, r);
			return;

		case as::type::permutation:
			break;

		case as::type::program:
			as::eval::program<T>(f, r);
			return;

		default:
			throw std::runtime_error("Unhandled node type in dispatch_play");
	}
}
