#ifndef language_diagnostic_hpp
#define language_diagnostic_hpp

#include "types.hpp"
#include "language/tokens.hpp"
#include <vector>


namespace an {


	class diagnostic final {

		public:

			struct entry final {
				const char*   msg;
				tk::raw::range   range;
			};


			std::vector<entry> _entries;

			auto push(const char* msg,
					  const tk::raw::range& range) -> void {
				_entries.emplace_back(
						an::diagnostic::entry{
							msg,
							range
						}
				);
			}

			auto push(const char* msg,
					  const tk::raw::token& tk) -> void {
				_entries.emplace_back(
						an::diagnostic::entry{
							msg,
							tk.range
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
							tk::raw::range{
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
