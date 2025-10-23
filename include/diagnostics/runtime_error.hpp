#ifndef midilang_runtime_error_hpp
#define midilang_runtime_error_hpp

#include <exception>


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- R U N T I M E  E R R O R --------------------------------------------

	class runtime_error : public std::exception {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::runtime_error;


			// -- private members ---------------------------------------------

			/* what */
			const char* _what;


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			runtime_error(void) = delete;

			/* where constructor */
			runtime_error(const char* what)
			: _what{what} {
			}

			/* deleted copy constructor */
			runtime_error(const self&) = delete;

			/* deleted move constructor */
			runtime_error(self&&) = delete;

			/* destructor */
			~runtime_error(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- public overrides --------------------------------------------

			/* what */
			auto what(void) const noexcept -> const char* override {
				return _what;
			}

	}; // class runtime_error

} // namespace ml

#endif // midilang_runtime_error_hpp
