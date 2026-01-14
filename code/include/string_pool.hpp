#ifndef string_pool_hpp
#define string_pool_hpp

#include "core/string/string.hpp"
#include <vector>


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- S T R I N G  P O O L ------------------------------------------------

	class string_pool final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::string_pool;


			// -- private members ---------------------------------------------

			/* storage */
			std::vector<mx::string> _storage;


			// -- private lifecycle -------------------------------------------

			/* default constructor */
			string_pool(void) noexcept = default;


			// -- private static methods --------------------------------------

			/* shared */
			static auto shared(void) noexcept -> self& {
				static self ins;
				return ins;
			}


		public:


			// -- public static methods ---------------------------------------

			/* store */
			static auto store(mx::string&& str) -> void {
				auto& pool = self::shared();
				str.clear();
				pool._storage.emplace_back(std::move(str));
			}

			/* query */
			static auto query(void) noexcept -> mx::string {
				auto& pool = self::shared();

				if (pool._storage.empty())
					return mx::string{};

				auto str = std::move(pool._storage.back());
				pool._storage.pop_back();
				return str;
			}

	}; // class string_pool

} // namespace mx

#endif // string_pool_hpp
