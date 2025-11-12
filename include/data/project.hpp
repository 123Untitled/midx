#ifndef midilang_data_project_hpp
#define midilang_data_project_hpp

#include "data/pattern_pool.hpp"
#include "language/analyzer.hpp"
#include "player.hpp"

#include "system/mapped_file.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- P R O J E C T -------------------------------------------------------

	class project final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::project;


			// -- private members ---------------------------------------------

			/* analyzer */
			mx::analyzer _analyzer;

			/* patterns */
			mx::pattern_pool _patterns;


		public:

			project(void)
			: _patterns{} {
			}


			auto update(const mx::mapped_file& mf) -> void {

				self::reset();
				::write(STDOUT_FILENO, mf.begin(), mf.size());

				_analyzer.analyze(mf);
				//_parser.reset();

			}

			auto highlights(void) const noexcept -> void {
				_analyzer.highlights();
			}

			// -- public modifiers --------------------------------------------

			/* reset */
			auto reset(void) noexcept -> void {
				_patterns.reset();
			}

	}; // class project

} // namespace mx

#endif // midilang_data_project_hpp
