#ifndef language_syntax_specifier_hpp
#define language_syntax_specifier_hpp

#include "types.hpp"
#include "literal.hpp"
#include "core/containers/flat_trie.hpp"


// -- S P  N A M E S P A C E --------------------------------------------------

namespace sp {


	/* specifier ids */
	enum class id : mx::usz {

		// atomic specifiers
		trig = 0U,
		note,
		gate,
		velo,
		octa,
		semi,
		chan,
		prob,

		// track specifier
		track,

		// pattern specifier
		pattern,

		// song specifier
		song,

		invalid,

		count
	}; // enum class id



	template <sp::id I, mx::literal... Ws>
	using entry = mx::entry<sp::id, I, Ws...>;


	inline auto to_id(const lx::lexeme& l) noexcept -> sp::id {

		static constexpr mx::flat_trie<
				sp::id,
				sp::id::invalid, 
				sp::entry<sp::id::trig,    "trig", "tr">,
				sp::entry<sp::id::note,    "note", "nt">,
				sp::entry<sp::id::gate,    "gate", "ga">,
				sp::entry<sp::id::velo,    "velo", "vl">,
				sp::entry<sp::id::octa,    "octa", "oc">,
				sp::entry<sp::id::semi,    "semi", "se">,
				sp::entry<sp::id::chan,    "chan", "ch">,
				sp::entry<sp::id::prob,    "prob", "pr">,
				sp::entry<sp::id::track,   "track">,
				sp::entry<sp::id::pattern, "pattern">,
				sp::entry<sp::id::song,    "song">
					> t;

		return t.find(l);
	}

} // namespace sp

#endif // language_syntax_specifier_hpp
