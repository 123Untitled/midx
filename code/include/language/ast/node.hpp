#ifndef language_ast_node_hpp
#define language_ast_node_hpp

#include "language/syntax/parameter.hpp"
#include "language/ast/type.hpp"
#include "language/tokens/token_view.hpp"
#include "language/syntax/parameter.hpp"

#include "math.hpp"

#include "language/ast/header.hpp"
#include "language/ast/remap_range.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------


namespace as {


	// -- P R O G R A M -------------------------------------------------------

	struct program final {

		/* header */
		as::header header;

		/* range */
		as::remap_range range;


		program(void) noexcept
		: header{as::type::program, mx::frac{0U, 1U}},
		  range{} {
		}

	}; // struct program



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
				   const mx::frac& dur) noexcept
		: header{as::type::references, dur},
		  tok_start{tok_start},
		  ref_start{ref_start},
		  count{count} {
		}
	};


	// -- T R A C K -----------------------------------------------------------

	struct track final {

		/* header */
		as::header header;

		/* params */
		mx::usz params[pa::max_params];


		track(const mx::usz (&params)[pa::max_params]) noexcept
		: header{as::type::track, {}} /* uninitialized */ {

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
				  const mx::frac& dur) noexcept
		: header{as::type::parameter, dur},
		  range{range} {
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
				 const mx::frac& dur) noexcept
		: header{as::type::parallel, dur},
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
				  const mx::frac& dur) noexcept
		: header{as::type::crossfade, dur},
		  left{left}, right{right} {
		}
	};


	// -- P E R M U T A T I O N -----------------------------------------------

	struct permutation final {

		/* header */
		as::header header;

		/* range */
		as::remap_range range;

		mx::frac local;


		permutation(void) noexcept
		: header{as::type::permutation, {}},
		  range{} {
		}
	};


	// -- T E M P O -----------------------------------------------------------

	struct tempo final {

		/* header */
		as::header header;

		/* frac start */
		mx::usz frac_start;

		/* token start */
		mx::usz token_start;

		/* count */
		mx::usz count;

		/* child node */
		mx::usz child;


		tempo(void) = delete;

		tempo(const mx::usz frac_start,
			  const mx::usz token_start,
			  const mx::usz count,
			  const mx::usz child,
			  const mx::frac& dur) noexcept
		: header{as::type::tempo, dur},
		  frac_start{frac_start},
		  token_start{token_start},
		  count{count},
		  child{child} {
		}
	};


	// -- M O D U L O ---------------------------------------------------------

	struct modulo final {

		/* header */
		as::header header;

		/* frac start */
		mx::usz frac_start;

		/* token start */
		mx::usz token_start;

		/* count */
		mx::usz count;

		/* child node */
		mx::usz child;



		modulo(void) = delete;
		modulo(const mx::usz frac_start,
			   const mx::usz token_start,
			   const mx::usz count,
				const mx::usz child,
			   const mx::frac& dur) noexcept
		: header{as::type::modulo, dur},
		  frac_start{frac_start},
		  token_start{token_start},
		  count{count},
		  child{child} {
		}

	}; // struct modulo

} // namespace as

#endif // language_ast_node_hpp
