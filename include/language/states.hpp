#ifndef ml_language_states_hpp
#define ml_language_states_hpp

#include "language/action_table.hpp"

#define non_instantiable_class(class_name) \
	class_name(void) = delete; \
	~class_name(void) = delete; \
	class_name(const class_name&) = delete; \
	class_name(class_name&&) = delete; \
	auto operator=(const class_name&) -> class_name& = delete; \
	auto operator=(class_name&&) -> class_name& = delete


// -- S M  N A M E S P A C E --------------------------------------------------

namespace ml {

	class expect_block;
	class expect_block_name;
	class expect_line_feed;
	class in_comment;
	class on_return;


	class in_comment final {
		public:
			non_instantiable_class(in_comment);
			static constexpr const char* name = "in_comment";


			static constexpr auto table = ml::make_table<

				// control characters
				ml::add<
					ml::control,
					&sd::push_error<"invalid character">
				>,

				// line feed
				ml::add<
					set<'\n'>,
					&sd::newline, &sd::pop_state
				>,

				// carriage return
				ml::add<
					set<'\r'>,
					&sd::switch_state<ml::on_return>
				>,

					// C_WHITESPACE
					ml::add<ml::set<' ', '\t'>>,

				ml::end_of_chunk<>,
				ml::end_of_file<>
			>();

	}; // class in_comment


	class on_return final {
		public:
			non_instantiable_class(on_return);
			static constexpr const char* name = "on_return";
			static constexpr ml::action_table table = ml::make_table<
			>();
	}; // class on_return


	class expect_block_name final {
		public:
			non_instantiable_class(expect_block_name);
			static constexpr const char* name = "expect_block_name";
			static constexpr ml::action_table table = ml::make_table<
			>();
	}; // class expect_block_name

	class expect_line_feed final {
		public:
			non_instantiable_class(expect_line_feed);
			static constexpr const char* name = "expect_line_feed";
			static constexpr auto table = ml::make_table<
			>();
	}; // class expect_line_feed


	class expect_block final {

		public:
			non_instantiable_class(expect_block);
			static constexpr const char* name = "expect_block";

			static constexpr auto table =
				ml::make_table<

					// C_INVALID
					ml::add<
						ml::on_chars<
						ml::range<0x00, 0xff>>,
						&sd::push_error<"invalid character">
					>,
					// C_BRACKET_OPEN
					ml::add<
						ml::set<'['>,
						&sd::switch_state<expect_block_name>
					>,
					// C_LINE_FEED
					ml::add<
						ml::set<'\n'>,
						&sd::newline
					>,
					// C_CARRIAGE_RETURN
					ml::add<
						ml::set<'\r'>,
						&sd::push_state<ml::expect_block>
					>,
					// C_NUMBER_SIGN
					ml::add<
						ml::set<'#'>,
						&sd::push_state<ml::in_comment>
					>,
					// C_WHITESPACE
					ml::add<ml::set<' ', '\t'>>,

					ml::end_of_chunk<>,
					ml::end_of_file<>
			>();

	}; // class expect_block



} // namespace ml

#endif //ml_language_states_hpp
