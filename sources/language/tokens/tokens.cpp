#include "language/tokens.hpp"
#include "language/tokens/map_iterator.hpp"



// -- S Y N  T O K E N --------------------------------------------------------

/* constructor */
tk::syn::token::token(const tk::id id,
					  const mx::usz start,
					  const mx::usz count) noexcept
: id{id}, start{start}, count{count} {
}

//tk::syn::token::token(const tk::id id,
//		const tk::const_map_iterator& it,
//		const tk::const_map_iterator& end) noexcept
//: id{id}, start{it.index()}, count{end.index() - start} {
//}
