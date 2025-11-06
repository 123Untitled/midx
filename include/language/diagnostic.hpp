#ifndef language_diagnostic_hpp
#define language_diagnostic_hpp

#include "types.hpp"
#include "language/tokens.hpp"
#include <vector>
#include <sstream>


namespace an {


	class diagnostic final {

		public:

			enum class level_type : ml::usz {
				WARNING,
				ERROR
			};

			struct entry final {
				level_type  level;
				const char*   msg;
				ml::usz      line;
				ml::usz col_start;
				ml::usz   col_end;
			};


			std::vector<entry> _entries;

			template <level_type LEVEL>
			auto push(const char* msg,
					  const ml::usz line,
					  const ml::usz col_start,
					  const ml::usz col_end) -> void {
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
							  const ml::usz line,
							  const ml::usz col_start,
							  const ml::usz col_end) -> void {
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
							const ml::usz line,
							const ml::usz col_start,
							const ml::usz col_end) -> void {
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

			auto to_json(void) -> std::string {
				std::stringstream ss;
				ss << "{\n\t\"type\": \"diagnostic\",\n\t\"entries\": [\n";

				bool first = true;

				for (const auto& e : _entries) {

					const char* level_str = (e.level == level_type::ERROR) ? "Error" : "Warning";

					if (first == false)
						ss << ",\n";
					else
						first = false;

					ss << "\t\t{ \"severity\": \"" << level_str
					   << "\", \"message\": \"" << e.msg
					   << "\", \"line\": " << e.line
					   << ", \"col_start\": " << e.col_start
					   << ", \"col_end\": " << e.col_end << " }";
				}

				ss << "\n\t]\n}\n";

				return ss.str();
			}
	};

} // namespace an

#endif // language_diagnostic_hpp
