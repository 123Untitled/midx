#ifndef language_ast_node_hpp
#define language_ast_node_hpp

#include "language/syntax/parameter.hpp"
#include "language/ast/type.hpp"
//#include "language/tokens/def.hpp"
#include "language/tokens/token_view.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------


namespace as {



	struct alignas(std::max_align_t) header final {
		mx::u32 size;
		as::type type;
		mx::usz steps;
		double duration;
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


		static consteval auto make_header(void) noexcept -> as::header {
			return as::header{
				sizeof(as::program),
				as::type::program,
				0U, 0.0
			};
		}

		program(void) noexcept
		: header{make_header()}, range{} {
		}

	}; // struct program



	// -- A T O M I C  V A L U E S --------------------------------------------

	struct atomic_values final {

		/* header */
		as::header header;

		mx::usz token_start; // tokens start index
		mx::usz value_start; // values start index
		// count is steps in header


		static constexpr auto make_header(const mx::usz count) noexcept -> as::header {
			return as::header{
				sizeof(as::atomic_values),
				as::type::atomic_values,
				count, 0.0
			};
		}


		atomic_values(void) = delete;

		atomic_values(const mx::usz tstart,
					  const mx::usz vstart,
					  const mx::usz count,
					  const double dur) noexcept
		: header{sizeof(as::atomic_values),
		         as::type::atomic_values,
		         count, dur},
		  token_start{tstart}, value_start{vstart} {
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
				   const mx::f64 dur) noexcept
		: header{sizeof(as::references),
		         as::type::references,
		         steps, dur},
		  tok_start{tok_start}, ref_start{ref_start}, count{count} {
		}
	};


	// -- T R A C K -----------------------------------------------------------

	struct track final {

		/* header */
		as::header header;

		/* param ranges */
		as::remap_range ranges[pa::max_params];


		static consteval auto make_header(void) noexcept -> as::header {
			return as::header{
				sizeof(as::track),
				as::type::track,
				0U, 0.0
			};
		}

		track(void) noexcept
		: header{make_header()}, ranges{} {
		}
	};


	// -- P A R A M E T E R ---------------------------------------------------

	struct parameter final {

		/* header */
		as::header header;

		/* child (direct node index) */
		mx::usz child;

		/* token view */
		tk::token_view tv;


		parameter(void) = delete;

		parameter(const mx::usz& child,
				  const tk::token_view& view,
				  const mx::usz steps,
				  const double dur) noexcept
		: header{sizeof(as::parameter),
		         as::type::parameter,
		         steps, dur},
			child{child}, tv{view} {
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
			  const double duration) noexcept
		: header{sizeof(as::group),
		         as::type::group,
		         steps, duration}, range{range} {
		}
	};


	// -- P A R A L L E L -----------------------------------------------------

	struct parallel final {

		/* header */
		as::header header;

		/* range */
		as::remap_range range;


		static constexpr auto make_header(const mx::usz dur) noexcept -> as::header {
			return as::header{
				sizeof(as::parallel),
				as::type::parallel,
				dur, 0.0
			};
		}

		parallel(void) = delete;

		parallel(const as::remap_range& range,
				 const mx::usz steps,
				 const double dur) noexcept
		: header{sizeof(as::parallel),
		         as::type::parallel,
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
				  const mx::usz steps,
				  const double dur) noexcept
		: header{sizeof(as::crossfade),
		         as::type::crossfade,
		         steps, dur},
		  left{left}, right{right} {
		}
	};


	// -- P E R M U T A T I O N -----------------------------------------------

	struct permutation final {

		/* header */
		as::header header;

		/* range */
		as::remap_range range;


		static consteval auto make_header(void) noexcept -> as::header {
			return as::header{
				sizeof(as::permutation),
				as::type::permutation,
				0U, 0.0
			};
		}

		permutation(void) noexcept
		: header{make_header()}, range{} {
		}
	};


	// -- T E M P O -----------------------------------------------------------

	struct tempo final {

		/* header */
		as::header header;

		/* factor */
		double factor;

		/* child node */
		mx::usz child;

		tempo(void) = delete;

		tempo(const double factor,
			  const mx::usz child,
			  const mx::usz steps,
			  const double dur) noexcept
		: header{sizeof(as::tempo),
		         as::type::tempo,
		         steps, dur},
		  factor{factor}, child{child} {
		}
	};

} // namespace as

#endif // language_ast_node_hpp
