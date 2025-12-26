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
				pa::entry<pa::prob, "prob", "pr">
				> t;

		if (tv.num_chunks() == 2U)
			return t.find(tv.last_chunk().lexeme);

		const auto& l = tv.first_chunk().lexeme;
		lx::lexeme lm = { l.data + 1U, l.size - 1U };

		return t.find(lm);
	}



	struct trig final {
		non_instantiable_class(trig);
		using type = mx::u8;
		static constexpr type min = 0U;
		static constexpr type max = 1U;
		static constexpr bool neg = false;
		static constexpr pa::id id = pa::trig;
	}; // struct trig

	struct note final {
		non_instantiable_class(note);
		using type = mx::u8;
		static constexpr type min = 0U;
		static constexpr type max = 127U;
		static constexpr bool neg = false;
		static constexpr pa::id id = pa::note;
	}; // struct note


	struct gate final {
		non_instantiable_class(gate);
		using type = float;
		static constexpr type min = 0U;
		static constexpr type max = 65535U;
		static constexpr bool neg = false;
		static constexpr pa::id id = pa::gate;
	}; // struct gate

	struct velo final {
		non_instantiable_class(velo);
		using type = mx::u8;
		static constexpr type min = 0U;
		static constexpr type max = 127U;
		static constexpr bool neg = false;
		static constexpr pa::id id = pa::velo;
	}; // struct velo

	struct octa final {
		non_instantiable_class(octa);
		using type = mx::i8;
		static constexpr type min = -128 / 12;
		static constexpr type max = +127 / 12;
		static constexpr bool neg = true;
		static constexpr pa::id id = pa::octa;
	}; // struct octa

	struct semi final {
		non_instantiable_class(semi);
		using type = mx::i8;
		static constexpr type min = -128;
		static constexpr type max = +127;
		static constexpr bool neg = true;
		static constexpr pa::id id = pa::semi;
	}; // struct semi

	struct chan final {
		non_instantiable_class(chan);
		using type = mx::u8;
		static constexpr type min = 1U;
		static constexpr type max = 16U;
		static constexpr bool neg = false;
		static constexpr pa::id id = pa::chan;
	}; // struct chan

	struct grou final {
		non_instantiable_class(grou);
		using type = mx::u8;
		static constexpr type min = 1U;
		static constexpr type max = 16U;
		static constexpr bool neg = false;
		static constexpr pa::id id = pa::chan;
	}; // struct grou

	struct prob final {
		non_instantiable_class(prob);
		using type = mx::u8;
		static constexpr type min = 0U;
		static constexpr type max = 100U;
		static constexpr bool neg = false;
		static constexpr pa::id id = pa::prob;
	}; // struct prob

} // namespace pa

#endif // language_syntax_parameter_hpp
