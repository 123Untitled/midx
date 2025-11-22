#ifndef language_ast_node_hpp
#define language_ast_node_hpp

#include "language/syntax/parameter.hpp"
#include "language/ast/type.hpp"
//#include "language/tokens/def.hpp"
#include "language/tokens/token_view.hpp"

// max_align_t
//#include <new>

// -- A S  N A M E S P A C E --------------------------------------------------

namespace tk {
	class tokens;
}

namespace as {

	//class tree;

	//struct ctx final {
	//
	//	const as::tree& tree;
	//	const tk::tokens& tokens;
	//	//std::vector<const as::node*> stack;
	//};


	struct alignas(std::max_align_t) header final {
		mx::u32 size;
		as::type type;
		//as::category category;
		//as::shape shape;
		mx::usz duration;
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
				0U
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
		// count is duration in header.duration


		static constexpr auto make_header(const mx::usz count) noexcept -> as::header {
			return as::header{
				sizeof(as::atomic_values),
				as::type::atomic_values,
				count
			};
		}


		atomic_values(void) = delete;

		atomic_values(const mx::usz tstart,
					  const mx::usz vstart,
					  const mx::usz count) noexcept
		: header{make_header(count)},
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


		static constexpr auto make_header(const mx::usz dur) noexcept -> as::header {
			return as::header{
				sizeof(as::references),
				as::type::references,
				dur
			};
		}

		references(void) = delete;

		references(const mx::usz tok_start,
				   const mx::usz ref_start,
				   const mx::usz count,
				   const mx::usz dur) noexcept
		: header{make_header(dur)},
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
				0U
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


		static constexpr auto make_header(const mx::usz dur) noexcept -> as::header {
			return as::header{
				sizeof(as::parameter),
				as::type::parameter,
				dur
			};
		}

		parameter(void) = delete;

		parameter(const mx::usz& child, const tk::token_view& t, const mx::usz dur) noexcept
		: header{make_header(dur)}, child{child}, tv{t} {
		}
	};


	// -- G R O U P -----------------------------------------------------------

	struct group final {

		/* header */
		as::header header;

		/* range */
		as::remap_range range;


		static constexpr auto make_header(const mx::usz dur) noexcept -> as::header {
			return as::header{
				sizeof(as::group),
				as::type::group,
				dur
			};
		}

		group(void) = delete;

		group(const as::remap_range& r, const mx::usz dur) noexcept
		: header{make_header(dur)},
		  range{r} {
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
				dur
			};
		}

		parallel(void) = delete;

		parallel(const as::remap_range& range, const mx::usz dur) noexcept
		: header{make_header(dur)},
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


		static constexpr auto make_header(const mx::usz dur) noexcept -> as::header {
			return as::header{
				sizeof(as::crossfade),
				as::type::crossfade,
				dur
			};
		}

		/* deleted default constructor */
		crossfade(void) = delete;

		crossfade(const mx::usz l,
				  const mx::usz r,
				  const mx::usz d) noexcept
		: header{make_header(d)},
		  left{l}, right{r} {
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
				0U
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

		/* numerator */
		mx::usz num;

		/* denominator */
		mx::usz den;

		/* child node */
		mx::usz child;


		static constexpr auto make_header(const mx::usz dur) noexcept -> as::header {
			return as::header{
				sizeof(as::tempo),
				as::type::tempo,
				dur
			};
		}

		tempo(void) = delete;

		tempo(const mx::usz num,
			  const mx::usz den,
			  const mx::usz child,
			  const mx::usz dur) noexcept
		: header{make_header(dur)},
		  num{num}, den{den}, child{child} {
		}
	};

} // namespace as

#endif // language_ast_node_hpp
