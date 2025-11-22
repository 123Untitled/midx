#ifndef language_syntax_parameter_hpp
#define language_syntax_parameter_hpp

#include "core/types.hpp"
#include "literal.hpp"
#include "core/containers/flat_trie.hpp"
#include "language/tokens/token_view.hpp"


// -- P A  N A M E S P A C E --------------------------------------------------

namespace pa {

	/* parameter id */
	using id = mx::u8;

	/* parameter ids */
	enum : pa::id {

		trig,
		note,
		gate,
		velo,
		octa,
		semi,
		chan,
		prob,

		dura,

		invalid,
		max_params
	};


	/* entry
	   alias for mx::entry specialized for pa::id */
	template <pa::id I, mx::literal... Ws>
	using entry = mx::entry<pa::id, I, Ws...>;


	/* to id
	   converts a lexeme to a parameter id */
	inline auto to_id(const tk::token_view& tv) noexcept -> pa::id {

		static constexpr mx::flat_trie<
				pa::id,
				pa::invalid,
				pa::entry<pa::trig, "trig", "tr">,
				pa::entry<pa::note, "note", "nt">,
				pa::entry<pa::gate, "gate", "ga">,
				pa::entry<pa::velo, "velo", "vl">,
				pa::entry<pa::octa, "octa", "oc">,
				pa::entry<pa::semi, "semi", "se">,
				pa::entry<pa::chan, "chan", "ch">,
				pa::entry<pa::prob, "prob", "pr">,
				pa::entry<pa::dura, "dura", "du">
				> t;

		if (tv.num_chunks() == 2U)
			return t.find(tv.last_chunk().lexeme);

		const auto& l = tv.first_chunk().lexeme;
		lx::lexeme lm = { l.data + 1U, l.size - 1U };

		return t.find(lm);
	}

} // namespace pa

#endif // language_syntax_parameter_hpp
