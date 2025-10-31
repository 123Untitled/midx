#ifndef midilang_data_project_hpp
#define midilang_data_project_hpp

#include "data/pattern_pool.hpp"
#include "language/analyzer.hpp"
#include "player.hpp"

#include "system/mapped_file.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- P R O J E C T -------------------------------------------------------

	class project final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::project;


			// -- private members ---------------------------------------------

			/* analyzer */
			ml::analyzer _analyzer;

			/* patterns */
			ml::pattern_pool _patterns;


		public:

			project(void)
			: _patterns{} {
			}


			auto update(const ml::mapped_file& mf) -> void {

				self::reset();
				::write(STDOUT_FILENO, mf.begin(), mf.size());

				_analyzer.analyze(mf, *this);
				//_parser.reset();

			}


			// -- public modifiers --------------------------------------------

			/* reset */
			auto reset(void) noexcept -> void {
				_patterns.reset();
			}

	}; // class project

} // namespace ml

#endif // midilang_data_project_hpp
