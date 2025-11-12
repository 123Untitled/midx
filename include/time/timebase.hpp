#ifndef midilang_time_timebase_hpp
#define midilang_time_timebase_hpp


// -- M L  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- T I M E B A S E -----------------------------------------------------

	class timebase final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::timebase;


			// -- private members ---------------------------------------------

			/* milliseconds to absolute clock */
			double _ms_to_absolute;

			/* absolute clock to nanoseconds */
			double _absolute_to_nano;

			/* nanoseconds to absolute clock */
			double _nano_to_absolute;


			// -- private lifecycle -------------------------------------------

			/* default constructor */
			timebase(void);

			/* deleted copy constructor */
			timebase(const self&) = delete;

			/* deleted move constructor */
			timebase(self&&) = delete;

			/* destructor */
			~timebase(void) noexcept = default;


			// -- private assignment operators --------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- private static methods --------------------------------------

			/* shared */
			static auto _shared(void) -> self&;


		public:

			// -- public static methods ---------------------------------------

			/* ms to absolute */
			static auto ms_to_absolute(void) noexcept -> double;

			/* absolute to nano */
			static auto absolute_to_nano(void) noexcept -> double;

			/* nano to absolute */
			static auto nano_to_absolute(void) noexcept -> double;

	}; // class timebase

} // namespace mx

#endif // midilang_time_timebase_hpp
