#ifndef ml_language_state_driver_hpp
#define ml_language_state_driver_hpp

#include "types.hpp"
#include "literal.hpp"
#include <vector>


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- forward declarations ------------------------------------------------

	class action_table;
	class parser_context;


	// -- S T A T E  D R I V E R ----------------------------------------------

	class state_driver final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::state_driver;


		public:

			// -- public types ------------------------------------------------

			/* action type */
			using action_type = auto (self::*)(void) -> void;


		private:

			// -- private members ---------------------------------------------

			/* context */
			ml::parser_context* _ctx;

			/* stack */
			std::vector<const ml::action_table*> _stack;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			state_driver(void);


			// -- public methods ----------------------------------------------

			auto parse(ml::parser_context&) -> void;


			/* call actions */
			template <action_type...>
			auto call(void) -> void;

			/* switch state */
			template <typename>
			auto switch_state(void) noexcept -> void;

			/* push state */
			template <typename>
			auto push_state(void) -> void;

			/* pop state */
			auto pop_state(void) -> void;


			/* newline */
			auto newline(void) noexcept -> void;

			/* push error */
			template <ml::literal>
			auto push_error(void) -> void;


			auto action1(void) noexcept -> void {
				// fictive example
			}
			auto action2(void) noexcept -> void {
				// fictive example
			}
			auto action3(void) noexcept -> void {
				// fictive example
			}
	};

} // namespace ml

#endif //ml_language_state_driver_hpp
