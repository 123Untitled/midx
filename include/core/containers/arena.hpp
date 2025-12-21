#ifndef arena_hpp
#define arena_hpp

#include "core/types.hpp"
#include "core/memory/align_up.hpp"
#include <vector>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- A R E N A -----------------------------------------------------------

	class arena final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::arena;


			// -- private members ---------------------------------------------

			std::vector<mx::u8> _data;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			arena(void) noexcept = default;


			// -- public methods ----------------------------------------------

			/* reset
			   clear all stored data */
			auto reset(void) noexcept -> void {
				_data.clear();
			}

			/* reserve
			   reserve space for at least size bytes */
			auto reserve(const mx::usz size) -> void {
				_data.reserve(size);
			}


			/* push
			   push a new object of type T constructed with args
			   returns the offset where the object was stored */
			template <typename T, typename... Ts>
			auto push(const Ts&... args) -> mx::usz {

				// compute aligned offset
				const mx::usz offset = mx::align_up<T>(_data.size());

				// allocate space
				_data.resize(offset + sizeof(T));

				// construct in place
				::new (&_data[offset]) T{args...};

				return offset;
			}

			template <typename T>
			auto at(const mx::usz index) /* noexcept */ -> T& {

				if (index + sizeof(T) > _data.size())
					throw std::runtime_error{"arena index out of bounds"};

				return *reinterpret_cast<T*>(&_data[index]);
			}

			template <typename T>
			auto at(const mx::usz index) const /* noexcept */ -> const T& {

				if (index + sizeof(T) > _data.size())
					throw std::runtime_error{"arena index out of bounds"};

				return *reinterpret_cast<const T*>(&_data[index]);
			}

	}; // class arena

} // namespace mx

#endif // arena_hpp
