#ifndef ml_language_project_context_hpp
#define ml_language_project_context_hpp


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- forward declarations ------------------------------------------------

	/* project */
	class project;

	/* pattern */
	class pattern;

	/* track */
	class track;


	class project_context final {

		private:
			ml::project* _project;
			ml::pattern* _pattern;
			ml::track*   _track;


		public:

			auto set_project(ml::project& p) noexcept -> void {
				_project = &p;
			}

			auto pattern(void) noexcept -> ml::pattern& {
				return *_pattern;
			}

			auto track(void) noexcept -> ml::track& {
				return *_track;
			}

	}; // class project_context

} // namespace ml

#endif // ml_language_project_context_hpp
