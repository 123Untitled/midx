#ifndef language_ast_nodes_atomics_hpp
#define language_ast_nodes_atomics_hpp

#include "language/syntax/parameter.hpp"
#include "language/ast/header.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- forward declarations ------------------------------------------------

	struct play_ctx;


	// -- A T O M I C  V A L U E S --------------------------------------------

	struct atomics final {


		public:

			// -- public members ----------------------------------------------

			/* header */
			as::header header;

			/* tokens start index */
			mx::usz token_start;

			/* values start index */
			mx::usz value_start;

			/* parameter id */
			pa::id param;


			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			atomics(void) = delete;

			/* constructor */
			atomics(const pa::id param,
					const mx::usz tstart,
					const mx::usz vstart,
					const mx::usz count) noexcept
			: header{as::type::atomics, mx::frac{count, 1U}},
			  token_start{tstart},
			  value_start{vstart},
			  param{param} {
			}


			// -- public methods ----------------------------------------------

			/* play
			   play atomics node */
			auto play(as::play_ctx&) const -> void;


	}; // struct atomic_values

} // namespace as

#endif // language_ast_nodes_atomics_hpp
