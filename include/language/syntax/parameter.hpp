#ifndef language_syntax_parameter_hpp
#define language_syntax_parameter_hpp

#include "language/syntax/specifier.hpp"
#include "language/lexer/lexeme.hpp"
#include "core/containers/flat_trie.hpp"


namespace pa {

	/* parameter ids */
	enum class id : ml::usz {

		// atomic parameters
		seq = 0U,
		rpt,
		lnk,

		// track parameters
		trig, note,
		gate, velo,
		octa, semi,
		chan, prob,

		// pattern parameters
		tracks,

		// song parameters
		patterns,

		invalid,
		count
	}; // enum class id



	template <pa::id I, ml::literal... Ws>
	using entry = mx::entry<pa::id, I, Ws...>;


	template <mx::is_entry... Es>
	auto to_id(const lx::lexeme& l) noexcept -> pa::id {

		static constexpr mx::flat_trie<
				pa::id,
				pa::id::invalid, Es...> t;

		return t.find(l);
	}

	/* search from atomic */
	static auto search_from_atomic = &pa::to_id<
		pa::entry<pa::id::seq, "sequence", "seq">,
		pa::entry<pa::id::rpt, "repeat", "rpt">,
		pa::entry<pa::id::lnk, "linked", "lnk">
	>;

	/* search from track */
	static auto search_from_track = &pa::to_id<
		pa::entry<pa::id::trig, "trig", "tr">,
		pa::entry<pa::id::note, "note", "nt">,
		pa::entry<pa::id::gate, "gate", "ga">,
		pa::entry<pa::id::velo, "velo", "vl">,
		pa::entry<pa::id::octa, "octa", "oc">,
		pa::entry<pa::id::semi, "semi", "se">,
		pa::entry<pa::id::chan, "chan", "ch">,
		pa::entry<pa::id::prob, "prob", "pr">
	>;

	/* search from pattern */
	static auto search_from_pattern = &pa::to_id<
		pa::entry<pa::id::tracks, "tracks", "trks">
	>;

	/* search from song */
	static auto search_from_song = &pa::to_id<
		pa::entry<pa::id::patterns, "patterns", "ptns">
	>;

	/* search from invalid */
	inline auto search_from_invalid(const lx::lexeme&) noexcept -> pa::id {
		return pa::id::invalid;
	}


	using search_fn_t = auto(*)(const lx::lexeme&) noexcept -> pa::id;


	/* search table */
	static search_fn_t search_table[static_cast<ml::usz>(sp::id::count)] {

		search_from_atomic, // trig
		search_from_atomic, // note
		search_from_atomic, // gate
		search_from_atomic, // velo
		search_from_atomic, // octa
		search_from_atomic, // semi
		search_from_atomic, // chan
		search_from_atomic, // prob

		search_from_track,    // track
		search_from_pattern,  // pattern
		search_from_song,     // song

		search_from_invalid  // invalid
	};


	static sp::id param_to_spec[
		static_cast<ml::usz>(pa::id::count)
	] {
		// seq
		sp::id::invalid,
		// rpt
		sp::id::invalid,
		// lnk
		sp::id::invalid,

		// trig
		sp::id::trig,
		// note
		sp::id::note,
		// gate
		sp::id::gate,
		// velo
		sp::id::velo,
		// octa
		sp::id::octa,
		// semi
		sp::id::semi,
		// chan
		sp::id::chan,
		// prob
		sp::id::prob,

		// tracks
		sp::id::track,

		// patterns
		sp::id::pattern,

		// invalid
		sp::id::invalid
	};



} // namespace sx


#endif // language_syntax_parameter_hpp
