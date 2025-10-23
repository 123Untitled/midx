#ifndef ml_system_mapped_file_hpp
#define ml_system_mapped_file_hpp

#include "types.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- M A P P E D  F I L E ------------------------------------------------

	class mapped_file final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::mapped_file;


			// -- private members ---------------------------------------------

			/* mapping */
			void* _mapping;

			/* size */
			ml::usz _size;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			mapped_file(void) noexcept;

			/* fd constructor */
			mapped_file(const int);

			/* deleted copy constructor */
			mapped_file(const self&) = delete;

			/* move constructor */
			mapped_file(self&&) noexcept;

			/* destructor */
			~mapped_file(void) noexcept;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self&;


			// -- public accessors --------------------------------------------

			/* begin */
			auto begin(void) const noexcept -> const ml::u8*;

			/* end */
			auto end(void) const noexcept -> const ml::u8*;

			/* size */
			auto size(void) const noexcept -> ml::usz;

	}; // class mapped_file

} // namespace ml

#endif // ml_system_mapped_file_hpp
