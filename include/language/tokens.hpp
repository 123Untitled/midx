#ifndef language_tokens_hpp
#define language_tokens_hpp

#include "types.hpp"
#include "macros.hpp"


// -- T K  N A M E S P A C E --------------------------------------------------

namespace tk {


	/* token id */
	using id = ml::u8;


	/* token class */
	template <tk::id...>
	struct token_class;


	namespace impl {

		/* token set concatenation */
		template <typename...>
		struct token_concat;

		template <tk::id... A, tk::id... B, typename... Tp>
		struct token_concat<tk::token_class<A...>, tk::token_class<B...>, Tp...> final {
			non_instantiable_class(token_concat);
			using type = typename token_concat<token_class<A..., B...>, Tp...>::type;
		};

		template <tk::id... A>
		struct token_concat<tk::token_class<A...>> final {
			non_instantiable_class(token_concat);
			using type = token_class<A...>;
		};

	} // namespace impl


	/* token set */
	template <typename... Tp>
	using token_set = typename tk::impl::token_concat<Tp...>::type;



	/* counter start */
	namespace impl {
		constexpr unsigned counter_start = (__COUNTER__) + 1U;
	}

	/* generate char class */
	#define GENERATE_TOKEN_CLASS(class_name) \
	using class_name = tk::token_class<static_cast<tk::id>((__COUNTER__) - tk::impl::counter_start)>

	GENERATE_TOKEN_CLASS(bracket_open);
	GENERATE_TOKEN_CLASS(bracket_close);
	GENERATE_TOKEN_CLASS(at_sign);
	GENERATE_TOKEN_CLASS(slash);
	GENERATE_TOKEN_CLASS(dot);

	GENERATE_TOKEN_CLASS(identifier);
	GENERATE_TOKEN_CLASS(string);
	GENERATE_TOKEN_CLASS(integer);



	static constexpr tk::id parsing_token_count = static_cast<tk::id>((__COUNTER__)
											    - tk::impl::counter_start);

	GENERATE_TOKEN_CLASS(comment);
	GENERATE_TOKEN_CLASS(whitespace);

	static constexpr tk::id token_count = static_cast<tk::id>((__COUNTER__)
										- tk::impl::counter_start);


	#undef GENERATE_TOKEN_CLASS


	/* token class */
	template <tk::id... Is>
	struct token_class final {

		noexcept_default_class(token_class);
		static_assert(((Is < token_count) && ...), "token_class: token ID out of range");

		template <tk::id ID, tk::id... Rest>
		struct first_id final {
			static constexpr tk::id id = ID;
			static_assert(sizeof...(Is) == 1U, "first_id: token_class contains multiple IDs");
			non_instantiable_class(first_id);
		}; // struct first_id

		static constexpr tk::id id = first_id<Is...>::id;

	}; // class token_class


	// -- public aliases ------------------------------------------------------

	using text = tk::token_set<tk::identifier,
		  					   tk::string>;

	using block = tk::token_set<tk::bracket_open,
								tk::bracket_close>;



	// -- T O K E N -----------------------------------------------------------

	struct range final {
		ml::uint row_head;
		ml::uint col_head;
		ml::uint row_tail;
		ml::uint col_tail;
	}; // struct range


	struct token final {
		tk::id    type;
		ml::uint line;
		const ml::u8* begin;
		const ml::u8* end;
	}; // class token

		//ml::uint byte_begin;
		//ml::uint byte_end;
		// value range
		//ml::uint vindex;
		//ml::uint vcount;


	namespace impl {

		/* is token class false specialization */
		template <typename T>
		struct is_token_class final {
			static constexpr bool value = false;
			non_instantiable_class(is_token_class);
		}; // struct is_token_class

		/* is token class true specialization */
		template <tk::id... Is>
		struct is_token_class<tk::token_class<Is...>> final {
			static constexpr bool value = true;
			non_instantiable_class(is_token_class);
		}; // struct is_token_class
	}

	/* is token class */
	template <typename T>
	concept is_token_class = tk::impl::is_token_class<T>::value;



} // namespace tk

#endif // language_tokens_hpp
