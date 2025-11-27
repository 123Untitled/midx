#ifndef language_ast_storage_hpp
#define language_ast_storage_hpp

#include "core/types.hpp"
#include "core/memory/align_up.hpp"
#include "language/tokens/iterator.hpp"
#include "math.hpp"
#include <vector>


// -- A S  N A M E S P A C E --------------------------------------------------

namespace as {


	// -- S T O R A G E -------------------------------------------------------

	class storage final {


		private:

			// -- private members ---------------------------------------------

			/* contiguous data */
			std::vector<mx::u8> _data;


		public:

			// -- public modifiers --------------------------------------------

			/* push frac
			   push frac onto the storage */
			auto push_frac(const mx::frac& frac) -> mx::usz {

				const mx::usz offset = mx::align_up<mx::frac>(_data.size());
				constexpr auto  size = sizeof(mx::frac);

				// allocate space
				_data.resize(offset + size);

				// construct in place
				::new (_data.data() + offset) mx::frac{frac};

				return offset;
			}


			/* push i8
			   push i8 onto the storage */
			auto push_i8(const mx::i8 byte) -> mx::usz {

				const mx::usz offset = mx::align_up<mx::i8>(_data.size());
				constexpr auto  size = sizeof(mx::i8);

				// allocate space
				_data.resize(offset + size);

				// construct in place
				::new (&_data[offset]) mx::i8{byte};

				return offset;
			}

			auto parse(tk::iterator& it,
					   const tk::iterator& end) -> void {
			}


			// -- public accessors --------------------------------------------

			/* frac at
			   access frac by direct index */
			auto frac_at(const mx::usz index) const /*noexcept*/ -> const mx::frac& {
				if (index + sizeof(mx::frac) > _data.size())
					throw std::runtime_error{"Storage frac index out of bounds"};
				return *reinterpret_cast<const mx::frac*>(&_data[index]);
			}

			/* i8 at
			   access i8 by direct index */
			auto i8_at(const mx::usz index) const /*noexcept*/ -> const mx::i8& {
				if (index + sizeof(mx::i8) > _data.size())
					throw std::runtime_error{"Storage i8 index out of bounds"};
				return *reinterpret_cast<const mx::i8*>(&_data[index]);
			}

	}; // class storage

} // namespace as

#endif // language_ast_storage_hpp
