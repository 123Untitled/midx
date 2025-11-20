#ifndef language_ast_node_hpp
#define language_ast_node_hpp

#include "language/ast/type.hpp"
#include "language/tokens/def.hpp"
#include "language/tokens/token_view.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------

namespace tk {
	class tokens;
}

namespace as {

	class arena;
	struct node;


	struct ctx final {

		const as::arena& arena;
		const tk::tokens& tokens;
		std::vector<const as::node*> stack;
	};


	struct node_base {
		mx::usz duration;
	};


	// -- A T O M I C  S E Q U E N C E ----------------------------------------

	struct atomic_values final : public as::node_base {

		mx::usz token_start; // tokens start index
		mx::usz value_start; // values start index
		// count is duration in this case

		atomic_values(void) noexcept
		: as::node_base{}, token_start{0U}, value_start{0U} {
		}

		atomic_values(const mx::usz start,
					  const mx::usz count) noexcept
		: as::node_base{count},
		  token_start{start},
		  value_start{0U} {
		}

	};


	struct atomic_refs final : public as::node_base {

		mx::usz token_start; // tokens start index
		mx::usz value_start; // references start index as node index
		mx::usz count;       // count of references

		atomic_refs(void) noexcept
		: as::node_base{}, token_start{0U}, value_start{0U}, count{0U} {
		}
	};


	// -- N O D E -------------------------------------------------------------

	struct node final {

		union u {
			as::atomic_values values;
			as::atomic_refs   refs;
			u(void) noexcept {
			}

		} data;

		as::type type;
		//const tk::token* token;
		const tk::token_view token;
		mx::usz start;
		mx::usz count;

		explicit node(const as::type nt) noexcept
		: type{nt}, token{}, start{0U}, count{0U} {
		}

		//node(const as::type  nt,
		//	 const tk::token& tk) noexcept
		//: type{nt}, token{&tk}, start{0U}, count{0U} {
		//}
		//node(const as::type nt,
		//	 const mx::usz  s,
		//	 const mx::usz  c) noexcept
		//: type{nt}, token{nullptr}, start{s}, count{c} {
		//}

		node(const as::type  nt,
			 const tk::token_view& tk) noexcept
		: type{nt}, token{tk}, start{0U}, count{0U} {
		}
		node(const as::type nt,
			 const mx::usz  s,
			 const mx::usz  c) noexcept
		: type{nt}, token{}, start{s}, count{c} {
		}

	}; // struct node

} // namespace as

#endif // language_ast_node_hpp
