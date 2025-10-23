#ifndef ml_system_file_hpp
#define ml_system_file_hpp

#include <fcntl.h>
#include "system/unix_descriptor.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- F I L E -------------------------------------------------------------

	class file final : public ml::unix_descriptor {


		private:

			// -- private members ---------------------------------------------

			/* self type */
			using self = ml::file;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			file(void) noexcept = default;

			/* path constructor */
			template <typename... Tp>
			file(const char* path, const Tp&... args)
			: ml::unix_descriptor{::open(path, args...)} {

				if (ml::unix_descriptor::operator int() == -1)
					throw ml::system_error{"open"};
			}

			/* deleted copy constructor */
			file(const self&) = delete;

			/* move constructor */
			file(self&&) noexcept = default;

			/* destructor */
			~file(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public conversion operators ---------------------------------

			/* int conversion operator */
			operator int(void) const noexcept;

	}; // class file

} // namespace ml

#endif // ml_system_file_hpp
