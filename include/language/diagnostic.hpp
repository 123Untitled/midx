#ifndef language_diagnostic_hpp
#define language_diagnostic_hpp

#include "types.hpp"
#include "language/tokens.hpp"
#include <vector>


namespace an {


	class diagnostic final {

		public:

			enum class level_type : mx::usz {
				WARNING,
				ERROR
			};

			struct entry final {
				level_type  level;
				const char*   msg;
				mx::usz      line;
				mx::usz col_start;
				mx::usz   col_end;
			};


			std::vector<entry> _entries;

			template <level_type LEVEL>
			auto push(const char* msg,
					  const mx::usz line,
					  const mx::usz col_start,
					  const mx::usz col_end) -> void {
				_entries.emplace_back(
						an::diagnostic::entry{
							LEVEL,
							msg,
							line,
							col_start,
							col_end
						}
				);
			}

			auto push_warning(const char* msg,
							  const tk::token& tk) -> void {
				_entries.emplace_back(
						an::diagnostic::entry{
							level_type::WARNING,
							msg,
							tk.line,
							tk.col_head,
							tk.col_tail
						}
				);
			}

			auto push_error(const char* msg,
							const tk::token& tk) -> void {
				_entries.emplace_back(
						an::diagnostic::entry{
							level_type::ERROR,
							msg,
							tk.line,
							tk.col_head,
							tk.col_tail
						}
				);
			}

			auto push_warning(const char* msg,
							  const mx::usz line,
							  const mx::usz col_start,
							  const mx::usz col_end) -> void {
				_entries.emplace_back(
						an::diagnostic::entry{
							level_type::WARNING,
							msg,
							line,
							col_start,
							col_end
						}
				);
			}

			auto push_error(const char* msg,
							const mx::usz line,
							const mx::usz col_start,
							const mx::usz col_end) -> void {
				_entries.emplace_back(
						an::diagnostic::entry{
							level_type::ERROR,
							msg,
							line,
							col_start,
							col_end
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
