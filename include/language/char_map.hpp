#ifndef ml_language_char_map_hpp
#define ml_language_char_map_hpp

#include "types.hpp"
#include "macros.hpp"


// -- C C  N A M E S P A C E --------------------------------------------------

namespace cc {


	// -- forward declarations ------------------------------------------------

	template <ml::u8...>
	struct char_class;


	namespace impl {

		/* char set concatenation */
		template <typename...>
		struct char_concat;

		template <ml::u8... A, ml::u8... B, typename... Tp>
		struct char_concat<cc::char_class<A...>, cc::char_class<B...>, Tp...> final {
			non_instantiable_class(char_concat);
			using type = typename cc::impl::char_concat<char_class<A..., B...>, Tp...>::type;
		};

		template <ml::u8... A>
		struct char_concat<cc::char_class<A...>> final {
			non_instantiable_class(char_concat);
			using type = cc::char_class<A...>;
		};

		/* empty set */
		template <>
		struct char_concat<> final {
			non_instantiable_class(char_concat);
			using type = cc::char_class<>;
		};

	} // namespace impl


	/* char set */
	template <typename... Tp>
	using char_set = typename cc::impl::char_concat<Tp...>::type;


	namespace impl {

		/* is char class false specialization */
		template <typename T>
		struct is_char_class final {
			static constexpr bool value = false;
			non_instantiable_class(is_char_class);
		}; // struct is_char_class

		/* is char class true specialization */
		template <ml::u8... Is>
		struct is_char_class<cc::char_class<Is...>> final {
			static constexpr bool value = true;
			non_instantiable_class(is_char_class);
		}; // struct is_char_class

	} // namespace impl

	/* is char class */
	template <typename T>
	concept is_char_class = cc::impl::is_char_class<T>::value;




	/* counter start */
	namespace impl {
		constexpr unsigned counter_start = (__COUNTER__) + 1U;
	}

	/* generate char class */
	#define GENERATE_CHAR_CLASS(class_name) \
	using class_name = cc::char_class<static_cast<ml::u8>((__COUNTER__) - cc::impl::counter_start)>

	GENERATE_CHAR_CLASS(control);

	GENERATE_CHAR_CLASS(line_feed);
	GENERATE_CHAR_CLASS(carriage_return);

	GENERATE_CHAR_CLASS(horizontal_tab);
	GENERATE_CHAR_CLASS(space);

	GENERATE_CHAR_CLASS(digit_zero);
	GENERATE_CHAR_CLASS(digit_one);
	GENERATE_CHAR_CLASS(digit_octal);
	GENERATE_CHAR_CLASS(digit_decimal);

	GENERATE_CHAR_CLASS(upper_hex);
	GENERATE_CHAR_CLASS(upper_g);
	GENERATE_CHAR_CLASS(upper_other);

	GENERATE_CHAR_CLASS(lower_hex);
	GENERATE_CHAR_CLASS(lower_x);
	GENERATE_CHAR_CLASS(lower_b);
	GENERATE_CHAR_CLASS(lower_o);
	GENERATE_CHAR_CLASS(lower_other);

	GENERATE_CHAR_CLASS(exclamation_mark);
	GENERATE_CHAR_CLASS(double_quote);
	GENERATE_CHAR_CLASS(number_sign);
	GENERATE_CHAR_CLASS(dollar_sign);
	GENERATE_CHAR_CLASS(percent);
	GENERATE_CHAR_CLASS(ampersand);
	GENERATE_CHAR_CLASS(single_quote);
	GENERATE_CHAR_CLASS(paren_open);
	GENERATE_CHAR_CLASS(paren_close);
	GENERATE_CHAR_CLASS(asterisk);
	GENERATE_CHAR_CLASS(plus);
	GENERATE_CHAR_CLASS(comma);
	GENERATE_CHAR_CLASS(minus);
	GENERATE_CHAR_CLASS(dot);
	GENERATE_CHAR_CLASS(slash);
	GENERATE_CHAR_CLASS(colon);
	GENERATE_CHAR_CLASS(semicolon);
	GENERATE_CHAR_CLASS(less_than);
	GENERATE_CHAR_CLASS(equal);
	GENERATE_CHAR_CLASS(greater_than);
	GENERATE_CHAR_CLASS(question_mark);
	GENERATE_CHAR_CLASS(at_sign);
	GENERATE_CHAR_CLASS(bracket_open);
	GENERATE_CHAR_CLASS(backslash);
	GENERATE_CHAR_CLASS(bracket_close);
	GENERATE_CHAR_CLASS(circumflex);
	GENERATE_CHAR_CLASS(underscore);
	GENERATE_CHAR_CLASS(accent);
	GENERATE_CHAR_CLASS(brace_open);
	GENERATE_CHAR_CLASS(vertical_bar);
	GENERATE_CHAR_CLASS(brace_close);
	GENERATE_CHAR_CLASS(tilde);

	GENERATE_CHAR_CLASS(extended);

	GENERATE_CHAR_CLASS(end_of_chunk);
	GENERATE_CHAR_CLASS(end_of_file);

	#undef GENERATE_CHAR_CLASS



	// -- C H A R  M A P ------------------------------------------------------

	class char_map final {


		private:

			template <typename T>
			struct first_char;

			template <ml::u8 F, ml::u8... R>
			struct first_char<cc::char_class<F, R...>> {
				non_instantiable_class(first_char);
				static constexpr ml::u8 value = F;
			};

			template <typename T>
			static constexpr ml::u8 id = first_char<T>::value;


		public:

			// -- public static members ---------------------------------------

			/* size */
			static constexpr ml::u8 size = static_cast<ml::u8>((__COUNTER__) - cc::impl::counter_start);

			/* table */
			static constexpr ml::u8 table[256U] {

				// 0x00 - 0x07 (invalid)
				id<control>, id<control>, id<control>, id<control>, id<control>, id<control>, id<control>, id<control>,

				// 0x08 backspace (invalid)
				id<control>,

				// 0x09 (horizontal tab)
				id<horizontal_tab>,

				// 0x0A (line feed)
				id<line_feed>,

				// 0x0B - 0x0C vertical tab, form feed (invalid)
				id<control>, id<control>,

				// 0x0D (carriage return)
				id<carriage_return>,

				// 0x0E - 0x1F (invalid)
				id<control>, id<control>, id<control>, id<control>, id<control>, id<control>, id<control>, id<control>,
				id<control>, id<control>, id<control>, id<control>, id<control>, id<control>, id<control>, id<control>,
				id<control>, id<control>,

				// 0x20 (space)
				id<space>,

				// 0x21 ! (exclamation mark)
				id<exclamation_mark>,

				// 0x22 " (double quote)
				id<double_quote>,

				// 0x23 # (number sign)
				id<number_sign>,

				// 0x24 $ (dollar sign)
				id<dollar_sign>,

				// 0x25 % (percent)
				id<percent>,

				// 0x26 & (ampersand)
				id<ampersand>,

				// 0x27 ' (single quote)
				id<single_quote>,

				// 0x28 ( (parenthesis open)
				id<paren_open>,

				// 0x29 ) (parenthesis close)
				id<paren_close>,

				// 0x2A * (asterisk)
				id<asterisk>,

				// 0x2B + (plus)
				id<plus>,

				// 0x2C , (comma)
				id<comma>,

				// 0x2D - (minus)
				id<minus>,

				// 0x2E . (dot)
				id<dot>,

				// 0x2F / (slash)
				id<slash>,

				// 0x30 0 (zero digit)
				id<digit_zero>,

				// 0x31 1 (one digit)
				id<digit_one>,

				// 0x32 - 0x37 2 - 7 (octal digit)
				id<digit_octal>, id<digit_octal>, id<digit_octal>, id<digit_octal>, id<digit_octal>, id<digit_octal>,

				// 0x38 - 0x39 8 - 9 (decimal digit)
				id<digit_decimal>, id<digit_decimal>,

				// 0x3A : (colon)
				id<colon>,

				// 0x3B ; (semicolon)
				id<semicolon>,

				// 0x3C < (less than)
				id<less_than>,

				// 0x3D = (equal)
				id<equal>,

				// 0x3E > (greater than)
				id<greater_than>,

				// 0x3F ? (question mark)
				id<question_mark>,

				// 0x40 @ (at sign)
				id<at_sign>,

				// 0x41 - 0x46 A - F (hex uppercase)
				id<upper_hex>, id<upper_hex>, id<upper_hex>, id<upper_hex>, id<upper_hex>, id<upper_hex>,

				// 0x47 G (note)
				id<upper_g>,

				// 0x48 - 0x5A H - Z (uppercase other)
				id<upper_other>, id<upper_other>, id<upper_other>, id<upper_other>, id<upper_other>, id<upper_other>, id<upper_other>, id<upper_other>,
				id<upper_other>, id<upper_other>, id<upper_other>, id<upper_other>, id<upper_other>, id<upper_other>, id<upper_other>, id<upper_other>,
				id<upper_other>, id<upper_other>, id<upper_other>,

				// 0x5B [ (open bracket)
				id<bracket_open>,

				// 0x5C \ (backslash)
				id<backslash>,

				// 0x5D ] (close bracket)
				id<bracket_close>,

				// 0x5E ^ (circumflex)
				id<circumflex>,

				// 0x5F _ (underscore)
				id<underscore>,

				// 0x60 ` (accent)
				id<accent>,

				// 0x61 a (lowercase hex)
				id<lower_hex>,

				// 0x62 b (binary prefix)
				id<lower_b>,

				// 0x63 - 0x66 c - f (lowercase hex)
				id<lower_hex>, id<lower_hex>, id<lower_hex>, id<lower_hex>,

				// 0x67 - 0x6E g - n (lowercase other)
				id<lower_other>, id<lower_other>, id<lower_other>, id<lower_other>, id<lower_other>, id<lower_other>, id<lower_other>, id<lower_other>,

				// 0x6F o (octal prefix)
				id<lower_o>,

				// 0x70 - 0x77 p - w (lowercase other)
				id<lower_other>, id<lower_other>, id<lower_other>, id<lower_other>, id<lower_other>, id<lower_other>, id<lower_other>, id<lower_other>,

				// 0x78 x (hex prefix)
				id<lower_x>,

				// 0x79 - 0x7A y - z (lowercase other)
				id<lower_other>, id<lower_other>,

				// 0x7B { (brace open)
				id<brace_open>,

				// 0x7C | (vertical bar)
				id<vertical_bar>,

				// 0x7D } (brace close)
				id<brace_close>,

				// 0x7E ~ (tilde)
				id<tilde>,

				// 0x7F delete (invalid)
				id<control>,

				// 0x80 - 0xFF (invalid)
				id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>,
				id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>,
				id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>,
				id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>,
				id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>,
				id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>,
				id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>,
				id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>,
				id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>,
				id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>,
				id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>,
				id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>,
				id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>,
				id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>,
				id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>,
				id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>, id<extended>,
			};

			/* eoc */
			static constexpr ml::u8 eoc = id<cc::end_of_chunk>;

			/* eof */
			static constexpr ml::u8 eof = id<cc::end_of_file>;

	}; // class char_map

	template <ml::u8... Is>
	struct char_class final {
		noexcept_default_class(char_class);
		static_assert(((Is < cc::char_map::size) && ...), "char_class: character ID out of range");
	}; // class char_class



	// -- public aliases ------------------------------------------------------

	using lower = cc::char_set<cc::lower_hex,
							   cc::lower_x,
							   cc::lower_b,
							   cc::lower_o,
							   cc::lower_other>;

	using upper = cc::char_set<cc::upper_hex,
							   cc::upper_g,
							   cc::upper_other>;

	using alpha = cc::char_set<cc::lower,
							   cc::upper>;

	using digit = cc::char_set<cc::digit_zero,
							   cc::digit_one,
							   cc::digit_octal,
							   cc::digit_decimal>;

	using xdigit = cc::char_set<cc::lower_hex,
								cc::upper_hex,
								cc::digit>;

	using alnum = cc::char_set<cc::alpha,
							   cc::digit>;

	using punct = cc::char_set<cc::exclamation_mark,
							   cc::double_quote,
							   cc::number_sign,
							   cc::dollar_sign,
							   cc::percent,
							   cc::ampersand,
							   cc::single_quote,
							   cc::paren_open,
							   cc::paren_close,
							   cc::asterisk,
							   cc::plus,
							   cc::comma,
							   cc::minus,
							   cc::dot,
							   cc::slash,
							   cc::colon,
							   cc::semicolon,
							   cc::less_than,
							   cc::equal,
							   cc::greater_than,
							   cc::question_mark,
							   cc::at_sign,
							   cc::bracket_open,
							   cc::backslash,
							   cc::bracket_close,
							   cc::circumflex,
							   cc::underscore,
							   cc::accent,
							   cc::brace_open,
							   cc::vertical_bar,
							   cc::brace_close,
							   cc::tilde>;

	using graph = cc::char_set<cc::alnum,
							   cc::punct>;

	using print = cc::char_set<cc::graph,
							   cc::space>;

	using blank = cc::char_set<cc::space,
							   cc::horizontal_tab>;

	using cntrl = cc::char_set<cc::control,
							   cc::line_feed,
							   cc::carriage_return,
							   cc::horizontal_tab>;

	// using whitespace
	// cannot define whitespace because '\f' and '\v' are grouped to control


	using note = cc::char_set<cc::upper_hex,
							  cc::upper_g>;


	using all = cc::char_set<cc::cntrl,
							 cc::print,
							 cc::extended>;

	using empty = cc::char_set<>;



	// -- T A B L E -----------------------------------------------------------

	template <typename>
	struct table;

	template <ml::u8... Is>
	struct table<cc::char_class<Is...>> final {
		private:
			struct array final {
				private:
					ml::u8 data[256U];

				public:
					constexpr array(void) noexcept
					: data{} {
						((data[Is] = 1u), ...);
					}
					constexpr auto operator[](const ml::u8 i) const noexcept -> bool {
						return static_cast<bool>(data[i]);
					}
			};
		public:
			noexcept_default_class(table);
			static constexpr array is;

	}; // struct table


	/* match */
	template <typename C>
	constexpr auto match(const ml::u8 c) noexcept -> bool {
		return cc::table<C>::is[c];
	}

} // namespace cc

#endif //ml_language_char_map_hpp
