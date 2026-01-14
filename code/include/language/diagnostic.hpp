#ifndef language_diagnostic_hpp
#define language_diagnostic_hpp

#include "language/tokens/token_view.hpp"
#include <vector>


namespace an {


	class diagnostic final {

		public:

			struct entry final {
				const char*   msg;
				tk::range   range;
			};


			std::vector<entry> _entries;


			auto push(const char* msg,
					  const tk::range& range) -> void {
				_entries.emplace_back(
						an::diagnostic::entry{
							msg,
							range
						}
				);
			}

			auto push(const char* msg,
					  const tk::token_view& tv) -> void {

				_entries.emplace_back(
						an::diagnostic::entry{
							msg,
							tv.first_chunk().range
						}
				);
			}

			auto push(const char* msg,
					  const tk::const_token_view& tv) -> void {

				_entries.emplace_back(
						an::diagnostic::entry{
							msg,
							tv.first_chunk().range
						}
				);
			}

			auto push(const char* msg,
					  const mx::usz ln,
					  const mx::usz cs,
					  const mx::usz ce) -> void {
				_entries.emplace_back(
						an::diagnostic::entry{
							msg,
							tk::range{
								ln,
								cs,
								ce
							}
						}
				);
			}

			auto clear(void) noexcept -> void {
				_entries.clear();
			}

			auto has_errors(void) const noexcept -> bool {
				return _entries.empty() == false;
			}
	};

} // namespace an

#endif // language_diagnostic_hpp
