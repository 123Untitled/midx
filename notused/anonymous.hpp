#ifndef anonymous_hpp
#define anonymous_hpp

#include <unistd.h>
#include <stdlib.h>
#include <unordered_set>
#include <string>

namespace mx {


	class anonymous final {


		private:

			std::unordered_set<unsigned> _ids;


		public:

			anonymous(void) noexcept {
				::srand(static_cast<unsigned>(::getpid()));
			}

			~anonymous(void) noexcept = default;

			auto clear (void) noexcept -> void {
				_ids.clear();
			}

			auto new_anon(void) -> std::string {

				// generate new id
				const unsigned id = static_cast<unsigned>(::rand());

				// check for collision
				if (_ids.contains(id))
					return this->new_anon();

				_ids.insert(id);

				return "__anon_" + std::to_string(id);
			}

	}; // class anonymous

} // namespace mx

#endif // anonymous_hpp
