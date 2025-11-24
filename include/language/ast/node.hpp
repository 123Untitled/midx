#ifndef language_ast_node_hpp
#define language_ast_node_hpp

#include "language/syntax/parameter.hpp"
#include "language/ast/type.hpp"
//#include "language/tokens/def.hpp"
#include "language/tokens/token_view.hpp"
#include "language/syntax/parameter.hpp"

#include "math.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------


namespace as {



	struct alignas(std::max_align_t) header final {
		as::type type;
		mx::usz steps;
		mx::frac dur;

		header(const as::type type,
			   const mx::usz steps,
			   const mx::frac& dur) noexcept
		: type{type}, steps{steps}, dur{dur} {
		}
	};

	struct alignas(std::max_align_t) remap_range final {
		mx::usz start; // start index in arena
		mx::usz count; // count of nodes
		remap_range(void) noexcept
		: start{0U}, count{0U} {
		}
		remap_range(const mx::usz s,
					const mx::usz c) noexcept
		: start{s}, count{c} {
		}

		auto end(void) const noexcept -> mx::usz {
			return start + count;
		}
	};




	// -- P R O G R A M -------------------------------------------------------

	struct program final {

		/* header */
		as::header header;

		/* range */
		as::remap_range range;


		program(void) noexcept
		: header{as::type::program, 0U, {}},
		  range{} {
		}

	}; // struct program



	// -- A T O M I C  V A L U E S --------------------------------------------

	struct atomic_values final {

		/* header */
		as::header header;

		mx::usz token_start; // tokens start index
		mx::usz value_start; // values start index
		// count is steps in header

		pa::id param_id; // associated parameter id

		mutable mx::usz step;


		atomic_values(void) = delete;

		atomic_values(const pa::id pid,
					  const mx::usz tstart,
					  const mx::usz vstart,
					  const mx::usz steps,
					  const mx::frac& dur) noexcept
		: header{as::type::atomic_values,
		         steps, dur},
		  token_start{tstart}, value_start{vstart},
		  param_id{pid},
		  step{steps} {
		}

	}; // struct atomic_values


	// -- R E F E R E N C E S -------------------------------------------------

	struct references final {

		/* header */
		as::header header;

		mx::usz tok_start; // tokens start index
		mx::usz ref_start; // references start index
		mx::usz count;     // count of references


		references(void) = delete;

		references(const mx::usz tok_start,
				   const mx::usz ref_start,
				   const mx::usz count,
				   const mx::usz steps,
				   const mx::frac& dur) noexcept
		: header{as::type::references,
		         steps, dur},
		  tok_start{tok_start}, ref_start{ref_start}, count{count} {
		}
	};


	// -- T R A C K -----------------------------------------------------------

	struct track final {

		/* header */
		as::header header;

		/* params */
		mx::usz params[pa::max_params];

		/* values */
		mx::usz values[pa::max_params];


		track(const mx::usz (&params)[pa::max_params]) noexcept
		: header{as::type::track, 0U, {}}, /* uninitialized */
		  values{
			  // trig
			  0,
			  // note
			  60,
			  // gate
			  50,
			  // velo
			  100,
			  // octa
			  0,
			  // semi
			  0,
			  // chan
			  0,
			  // prob
			  100,
		  } {

			// copy params
			for (mx::usz i = 0U; i < pa::max_params; ++i)
				this->params[i] = params[i];
		}
	};


	// -- P A R A M E T E R ---------------------------------------------------

	struct parameter final {

		/* header */
		as::header header;

		/* range */
		as::remap_range range;


		parameter(void) = delete;

		parameter(const as::remap_range& range,
				  const mx::usz steps,
				  const mx::frac& dur) noexcept
		: header{as::type::parameter, steps, dur},
		  range{range} {
		}
	};


	// -- G R O U P -----------------------------------------------------------

	struct group final {

		/* header */
		as::header header;

		/* range */
		as::remap_range range;


		group(void) = delete;

		group(const as::remap_range& range,
			  const mx::usz steps,
			  const mx::frac& dur) noexcept
		: header{as::type::group,
		         steps, dur}, range{range} {
		}
	};


	// -- P A R A L L E L -----------------------------------------------------

	struct parallel final {

		/* header */
		as::header header;

		/* range */
		as::remap_range range;


		parallel(void) = delete;

		parallel(const as::remap_range& range,
				 const mx::usz steps,
				 const mx::frac& dur) noexcept
		: header{as::type::parallel,
		         steps, dur},
		  range{range} {
		}

	}; // struct parallel


	// -- C R O S S F A D E ---------------------------------------------------

	struct crossfade final {

		/* header */
		as::header header;

		/* left node index */
		mx::usz left;

		/* right node index */
		mx::usz right;


		/* deleted default constructor */
		crossfade(void) = delete;

		crossfade(const mx::usz left,
				  const mx::usz right,
				  const mx::usz steps) noexcept
		: header{as::type::crossfade,
		         steps, {}},
		  left{left}, right{right} {
		}
	};


	// -- P E R M U T A T I O N -----------------------------------------------

	struct permutation final {

		/* header */
		as::header header;

		/* range */
		as::remap_range range;


		permutation(void) noexcept
		: header{as::type::permutation,
		         0U, {}},
		  range{} {
		}
	};


	// -- T E M P O -----------------------------------------------------------

	struct tempo final {

		/* header */
		as::header header;

		/* ratio */
		mx::frac factor;

		/* runtime */
		bool runtime;

		/* child node */
		mx::usz child;

		tempo(void) = delete;

		tempo(const mx::frac& factor,
			  const bool runtime,
			  const mx::usz child,
			  const mx::usz steps,
			  const mx::frac& dur) noexcept
		: header{as::type::tempo,
		         steps, dur},
		  factor{factor}, runtime{runtime}, child{child} {
		}
	};

} // namespace as

#endif // language_ast_node_hpp
