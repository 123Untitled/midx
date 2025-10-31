#ifndef ml_language_transition_hpp
#define ml_language_transition_hpp


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- T R A N S I T I O N -------------------------------------------------

	template <typename T>
	class transition final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::transition<T>;

			/* state type */
			using state_type = typename T::state_type;

			/* action type */
			using action_type = auto (T::*)(void) -> void;


		public:

			// -- public members ----------------------------------------------

			/* state */
			state_type state;

			union {

				/* last state */
				state_type last;

				/* error */
				const char* error;
			};

			/* action */
			action_type action;



			// -- public lifecycle --------------------------------------------

			/* default constructor */
			transition(void) noexcept = default;
			//transition(void) = delete;

			/* members constructor */
			constexpr transition(const state_type& s,
								 const action_type a) noexcept
			: state{s}, error{nullptr}, action{a} {
			}

			/* members constructor */
			constexpr transition(const state_type& s,
								 const char* err,
								 const action_type a) noexcept
			: state{s}, error{err}, action{a} {
			}

			/* members constructor with last state */
			constexpr transition(const state_type& s,
								 const state_type& l,
								 const action_type a) noexcept
			: state{s}, last{l}, action{a} {
			}

			/* deleted copy constructor */
			transition(const self&) = delete;

			/* deleted move constructor */
			transition(self&&) = delete;

			/* destructor */
			constexpr ~transition(void) noexcept = default;


			// -- public assignment operators -----------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- public methods ------------------------------------------

			/* execute */
			auto execute(T& sm) const -> void {
				(sm.*action)();
			}

	}; // class transition

} // namespace ml

#endif // ml_language_transition_hpp
