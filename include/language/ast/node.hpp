#ifndef language_ast_node_hpp
#define language_ast_node_hpp

#include "language/ast/type.hpp"
#include "language/tokens/def.hpp"


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- N O D E -------------------------------------------------------------

	struct node final {

		as::type type;
		const tk::token* token;
		mx::usz start;
		mx::usz count;

		explicit node(const as::type nt) noexcept
		: type{nt}, token{nullptr}, start{0U}, count{0U} {
		}

		node(const as::type  nt,
			 const tk::token& tk) noexcept
		: type{nt}, token{&tk}, start{0U}, count{0U} {
		}
		node(const as::type nt,
			 const mx::usz  s,
			 const mx::usz  c) noexcept
		: type{nt}, token{nullptr}, start{s}, count{c} {
		}

	}; // struct node

} // namespace as

#endif // language_ast_node_hpp
