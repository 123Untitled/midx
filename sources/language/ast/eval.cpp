#include "language/ast/eval.hpp"
#include "language/ast/tree.hpp"
#include "language/highlight_tracker.hpp"


// -- E V A L -----------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
as::eval::eval(void) noexcept
: _tree{nullptr}, //_tokens{nullptr},
  _hashes{},
  _engine{nullptr},
  _hls{nullptr},
  _absolute{0, 1},
  _last{0, 1} {
}


// -- public methods ----------------------------------------------------------

/* init
   initialize evaluator */
auto as::eval::init(const as::tree& tree, mx::highlight_tracker& hls) noexcept -> void {

	_tree = &tree;
	_hls  = &hls;

	_cross.clear();
	_hashes.clear();
}


/* evaluate
   evaluate the AST and produce MIDI events */
auto as::eval::evaluate(mx::midi_engine& engine,
					    const mx::frac& time) -> void {

	_engine = &engine;
	_absolute = time;

	as::expr_result r;

	// evaluate root node
	as::eval::program(time, r);

	// flush midi events
	r.flush(*_engine);
}


/* program
   evaluate a program node */
template <typename T>
auto as::eval::program(const mx::frac& time, T& r) -> void {

	const auto& p = _tree->node<as::program>(0U);

		  auto it   = p.range.start;
	const auto end  = p.range.end();
		  auto fr   = as::frame{time};

	// loop over child nodes
	for (; it < end; ++it) {
		fr.node = _tree->remap_index(it);
		fr.hash = as::hash{}.re_hash(fr.node);
		as::eval::dispatch(fr, r);
	}
}


/* group
   evaluate a group node */
template <typename T>
auto as::eval::group(const as::frame& f, T& r) -> void {

	// get group node
	const auto& g = _tree->node<as::group>(f.node);

		  auto it  = g.range.start;
	const auto end = g.range.end();

	auto time = g.header.mod(f.time);

	if (f.last.den == 0U)
		goto diverged;

	{
		auto last = g.header.mod(f.last);

		// loop over child nodes
		for (; it < end; ++it) {

			const auto node = _tree->remap_index(it);
			const auto& dur = _tree->header(node).dur;

			if (time < dur) {
				if (last >= dur)
					last.den = 0U;

				const as::frame fr{node, f.hash.re_hash(node),
								   time, last};

				as::eval::dispatch(fr, r);
				return;
			}

			time -= dur;

			if (last < dur) {
				++it; goto diverged;
			}

			last -= dur;
		}
	}

	diverged:
	for (; it < end; ++it) {
		const auto node = _tree->remap_index(it);
		const auto& dur = _tree->header(node).dur;

		if (time < dur) {
			const as::frame fr{
				node, f.hash.re_hash(node),
				time, mx::frac{nullptr}
			};

			as::eval::dispatch(fr, r);
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

	auto time = t.header.mod(f.time);

	const auto& dur = _tree->header(t.child).dur;

		  auto  it = t.frac_start;
		  auto  tk = t.token_start;
	const auto end = t.frac_start + t.count;


	if (f.last.den == 0U)
		goto diverged;

	{
		auto last = t.header.mod(f.last);

		for (; it < end; ++it, ++tk) {

			const auto&    tempo = _tree->frac_at(it);
			const auto local_dur = dur / tempo;

			if (time < local_dur) {
				if (last >= local_dur)
					last.den = 0U;

				const as::frame fr{
					t.child, f.hash.re_hash(t.child),
					time * tempo,
					last * tempo
				};

				as::eval::dispatch(fr, r);
				_hls->mark_active(tk, "Underlined");
				return;
			}

			// Move to next segment
			time -= local_dur;

			if (last < local_dur) {
				++it; ++tk; goto diverged;
			}
			last -= local_dur;
		}
	}

	diverged:

	for (; it < end; ++it, ++tk) {

		const auto&    tempo = _tree->frac_at(it);
		const auto local_dur = dur / tempo;

		if (time < local_dur) {

			const as::frame fr{
				t.child, f.hash.re_hash(t.child),
				time * tempo, mx::frac{nullptr}
			};

			as::eval::dispatch(fr, r);
			_hls->mark_active(tk, "Underlined");
			return;
		}

		time -= local_dur;
	}
}

/* modulo
   evaluate a modulo node */
template <typename T>
auto as::eval::modulo(const as::frame& f, T& r) -> void {

	const auto& m = _tree->node<as::modulo>(f.node);

		  auto  it = m.frac_start;
		  auto  tk = m.token_start;
	const auto end = m.frac_start + m.count;

	auto time = m.header.mod(f.time);

	if (f.last.den == 0U)
		goto diverged;

	{
		auto last = m.header.mod(f.last);

		// loop over modulus
		for (; it < end; ++it, ++tk) {

			const auto& mod = _tree->frac_at(it);

			if (time < mod) {
				if (last >= mod)
					last.den = 0U;

				const as::frame fr{
					m.child, f.hash.re_hash(m.child),
					time, last
				};

				as::eval::dispatch(fr, r);
				_hls->mark_active(tk, "Underlined");
				return;
			}

			time -= mod;

			if (last < mod) {
				++it; ++tk; goto diverged;
			}

			last -= mod;
		}
	}

	diverged:

	for (; it < end; ++it, ++tk) {

		const auto& mod = _tree->frac_at(it);

		if (time < mod) {

			const as::frame fr{
				m.child, f.hash.re_hash(m.child),
				time, mx::frac{nullptr}
			};

			as::eval::dispatch(fr, r);
			_hls->mark_active(tk, "Underlined");
			return;
		}

		time -= mod;
	}
}

/* parallel
   evaluate a parallel node */
template <typename T>
auto as::eval::parallel(const as::frame& f, T& r) -> void {

	const auto& p = _tree->node<as::parallel>(f.node);

		  auto it   = p.range.start;
	const auto end  = p.range.end();
	const auto time = p.header.mod(f.time);
	const auto last = f.last.den != 0U ? p.header.mod(f.last) : mx::frac{nullptr};

	// loop over child nodes
	for (; it < end; ++it) {
		const auto node = _tree->remap_index(it);
		const as::frame fr{node, f.hash.re_hash(node), time, last};
		as::eval::dispatch(fr, r);
	}
}

/* crossfade
   evaluate a crossfade node */
template <typename T>
auto as::eval::crossfade(const as::frame& f, T& r) -> void {

	const auto&  cf = _tree->node<as::crossfade>(f.node);
	const auto time = cf.header.mod(f.time);


	T lr, rr;
	as::eval::dispatch(f.propagate(cf.left,  time, f.last), lr);
	as::eval::dispatch(f.propagate(cf.right, time, f.last), rr);

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
			//std::cout << "\x1b[2K\x1b[H";
			//std::cout << interpolated << " " << std::flush;

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

		const auto time = a.header.mod(f.time);

		auto step = 0U;
		bool edge = true;

			// step = ? time.num / time.den : 0U;
		if (a.header.dur.num == 1U) {

			if (f.last.den != 0U)
				edge = time < a.header.mod(f.last); // maybe just time(not mod) < last
		}
		else {

			if (f.last.den != 0U) {
				const auto last = a.header.mod(f.last);
				step = time.num / time.den;
				edge = step != (last.num / last.den);
			}
		}

		// get value at step
		const typename T::type value = _tree->value_at/*<T::type>*/(a.value_start + step);

		r.accumulate(value, edge);

		const char* group = "CurSearch";
		_hls->mark_active(a.token_start + step, group);
	}
}


template <typename T>
auto as::eval::param(const as::frame& f, T& r) -> void {

	// same as group node

	const auto& p = _tree->node<as::parameter>(f.node);

		  auto it  = p.range.start;
	const auto end = p.range.end();

	auto time = p.header.mod(f.time);

	if (f.last.den == 0U)
		goto diverged;

	{
		auto last = p.header.mod(f.last);

		// loop over child nodes
		for (; it < end; ++it) {
			const auto node = _tree->remap_index(it);
			const auto& dur = _tree->header(node).dur;

			if (time < dur) {
				if (last >= dur)
					last.den = 0U;

				const as::frame fr{node, f.hash.re_hash(node),
								   time, last};
				as::eval::dispatch(fr, r);
				return;
			}

			time -= dur;

			if (last < dur) {
				++it; goto diverged;
			}

			last -= dur;
		}
	}

	diverged:
	for (; it < end; ++it) {
		const auto node = _tree->remap_index(it);
		const auto& dur = _tree->header(node).dur;

		if (time < dur) {
			const as::frame fr{
				node, f.hash.re_hash(node),
				time, mx::frac{nullptr}
			};
			as::eval::dispatch(fr, r);
			return;
		}
		time -= dur;
	}
}


template <typename T>
auto as::eval::track(const as::frame& f, T& expr) -> void {

    const auto& tr = _tree->node<as::track>(f.node);

	const auto time  = tr.header.mod(f.time);
	const auto last  = f.last.den != 0U ? tr.header.mod(f.last) : mx::frac{nullptr};


	if constexpr (mx::is_same<T, as::expr_result>) {

		as::track_result r;

		if (tr.params[pa::trig]) {
			as::eval::param(f.propagate(tr.params[pa::trig],
										time, last), r.trig);
		}

		if (tr.params[pa::note]) {
			as::eval::param(f.propagate(tr.params[pa::note],
										time, last), r.note);
		}

		if (tr.params[pa::chan]) {
			as::eval::param(f.propagate(tr.params[pa::chan],
										time, last), r.chan);
		}

		if (tr.params[pa::velo]) {
			as::eval::param(f.propagate(tr.params[pa::velo],
										time, last), r.velo);
		}

		if (tr.params[pa::gate]) {
			const auto n = tr.params[pa::gate];
			as::eval::param(f.propagate(n, time, last), r.gate);
		}

		if (tr.params[pa::prob]) {
			const auto n = tr.params[pa::prob];
			as::eval::param(f.propagate(n, time, last), r.prob);
		}

		if (tr.params[pa::octa]) {
			const auto n = tr.params[pa::octa];
			as::eval::param(f.propagate(n, time, last), r.octa);
		}

		if (tr.params[pa::semi]) {
			const auto n = tr.params[pa::semi];
			as::eval::param(f.propagate(n, time, last), r.semi);
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

	auto time = ref.header.mod(f.time);

	if (f.last.den == 0U)
		goto diverged;

	{
		auto last = ref.header.mod(f.last);

		// loop over referenced nodes
		for (; it < end; ++it, ++tk) {
			const auto node = _tree->ref_at(it);
			const auto& dur = _tree->header(node).dur;

			if (time < dur) {
				if (last >= dur)
					last.den = 0U;

				const as::frame fr{
					node, f.hash.re_hash(node),
					time, last
				};

				as::eval::dispatch(fr, r);
				_hls->mark_active(tk, "Underlined");
				return;
			}
			time -= dur;

			if (last < dur) {
				++it; ++tk; goto diverged;
			}
			last -= dur;
		}
	}

	diverged:
	for (; it < end; ++it, ++tk) {
		const auto node = _tree->ref_at(it);
		const auto& dur = _tree->header(node).dur;

		if (time < dur) {
			const as::frame fr{
				node, f.hash.re_hash(node),
				time, mx::frac{nullptr}
			};

			as::eval::dispatch(fr, r);
			_hls->mark_active(tk, "Underlined");
			return;
		}
		time -= dur;
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
			// not implemented...
			break;

		default:
			std::cout << _tree->what_is(f.node) << "\n";
			throw std::runtime_error("Unhandled node type in dispatch_play");
	}
}
