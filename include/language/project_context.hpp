#ifndef ml_language_project_context_hpp
#define ml_language_project_context_hpp


// -- M L  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- forward declarations ------------------------------------------------

	/* project */
	class project;

	/* pattern */
	class pattern;

	/* track */
	class track;


	class project_context final {

		private:
			mx::project* _project;
			mx::pattern* _pattern;
			mx::track*   _track;


		public:

			auto set_project(mx::project& p) noexcept -> void {
				_project = &p;
			}

			auto pattern(void) noexcept -> mx::pattern& {
				return *_pattern;
			}

			auto track(void) noexcept -> mx::track& {
				return *_track;
			}

	}; // class project_context

} // namespace mx

#endif // ml_language_project_context_hpp
