#ifndef model_note_hpp
#define model_note_hpp

#include "types.hpp"
#include "time/signature.hpp"
#include <vector>


// -- M O  N A M E S P A C E --------------------------------------------------

namespace mo {


	class sequence final {


		private:

			std::vector<ml::i8> _data;


		public:

			sequence(void) noexcept
			: _data{} {
			}

			~sequence(void) noexcept = default;


			auto push(const ml::i8 value) -> void {
				_data.emplace_back(value);
			}


	}; // class note

} // namespace mo

#endif // model_note_hpp
