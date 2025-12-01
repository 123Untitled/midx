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
			as::eval::dispatch<T>(f.fork(node, time), r);
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

	for (; it < end; ++it, ++tk) {
		const auto& frac = _tree->frac_at(it);

		// duration of child at this tempo
		const auto dur = _tree->header(t.child).dur;// / frac;

		if (time < dur) {
			// push child node with tempo-adjusted speed
			as::eval::dispatch<T>(f.fork(t.child, time, frac), r);
			break;
		}
		time -= dur;
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
			// push child node with modulated time
			as::eval::dispatch<T>(f.fork(m.child, time), r);
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

		  auto it = p.range.start;
	const auto end = p.range.end();

	const auto time  = p.header.mod(f.local_time());
	const auto local = time / f.speed;

	// loop over child nodes
	for (; it < end; ++it) {
		const auto node = _tree->remap_index(it);
		const auto& h   = _tree->header(node);

		as::eval::dispatch<T>(
				as::frame{
					node,
					as::frame::compute_hash(f.hash, node),
					local,
					f.speed},
		r);
	}
}

/* crossfade
   evaluate a crossfade node */
template <typename T>
auto as::eval::crossfade(const as::frame& f, T& r) -> void {

	const auto&  cf = _tree->node<as::crossfade>(f.node);
	const auto time = cf.header.mod(f.local_time());


	T lr, rr;
	as::eval::dispatch(f.fork(cf.left,  time), lr);
	as::eval::dispatch(f.fork(cf.right, time), rr);

	if constexpr (as::is_param_accum<T>) {

		// discrete (note, trig, chan)
		if constexpr (T::is_discrete) {

			auto& c = _cross[f.hash];

			if (lr.edge || rr.edge) {
				c.evaluate(time, cf.header.progress(time / f.speed));
				std::cout << (c.side ? "\x1b[35mB\x1b[0m" : "\x1b[33mA\x1b[0m") << std::flush;
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


		const auto hash = f.compute_hash(f.hash, step);
		const auto it   = _hashes.find(hash);

		bool edge  = true;

		if (it != _hashes.end())
			edge = time < it->second;

		_hashes[hash] = time;

		// get value at step
		const typename T::type value = _tree->value_at/*<T::type>*/(a.value_start + step);

		r.accumulate(value, edge);

		if (!edge)
			return;

		const auto next_step_local = mx::frac{step + 1, 1};
		const auto expire = _absolute + (next_step_local - time) / f.speed;

		//const auto remaining = mx::frac{1, 1} - (time - mx::frac{step, 1});
		//const auto expire    = _absolute + (remaining / f.speed);

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
			as::eval::dispatch<T>(f.fork(node, time), r);
			return;
		}

		time -= dur;
	}
}


template <typename T>
auto as::eval::track(const as::frame& f, T& expr) -> void {

    const auto& tr = _tree->node<as::track>(f.node);

	const auto time = tr.header.mod(f.local_time());// / f.speed;


	if constexpr (mx::is_same<T, as::expr_result>) {

		as::track_result r;

		if (tr.params[pa::trig]) {
			const auto n = tr.params[pa::trig];
			as::eval::param(f.forward(n, time), r.trig);
		}

		if (tr.params[pa::note]) {
			const auto n = tr.params[pa::note];
			as::eval::param(f.forward(n, time), r.note);
		}

		if (tr.params[pa::chan]) {
			const auto n = tr.params[pa::chan];
			as::eval::param(f.forward(n, time), r.chan);
		}

		if (tr.params[pa::velo]) {
			const auto n = tr.params[pa::velo];
			as::eval::param(f.forward(n, time), r.velo);
		}

		if (tr.params[pa::gate]) {
			const auto n = tr.params[pa::gate];
			as::eval::param(f.forward(n, time), r.gate);
		}

		if (tr.params[pa::prob]) {
			const auto n = tr.params[pa::prob];
			as::eval::param(f.forward(n, time), r.prob);
		}

		if (tr.params[pa::octa]) {
			const auto n = tr.params[pa::octa];
			as::eval::param(f.forward(n, time), r.octa);
		}

		if (tr.params[pa::semi]) {
			const auto n = tr.params[pa::semi];
			as::eval::param(f.forward(n, time), r.semi);
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

			const auto fc = f.fork(node, time);
			//const auto fc = f.forward(node, time);
			//
			//auto old = _hashes.find(fc.hash);
			//bool edge = true;
			//if (old != _hashes.end())
			//	edge = time < old->second;
			//_hashes[fc.hash] = time;


			//if (edge) {
				// highlight with expiration
				//const auto remaining_local = dur - time;
				//const auto remaining_global = remaining_local / f.speed;
				//const auto expire = _absolute + remaining_global;
				//_hls->mark_active(tk, "IncSearch", expire);
			//}

			// push node and local tick
			as::eval::dispatch<T>(fc, r);
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
		as::eval::dispatch(f.fork(node, f.time), r); // optimize this
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
