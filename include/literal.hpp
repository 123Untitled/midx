#ifndef ml_literal_hpp
#define ml_literal_hpp


namespace ml {


	template <unsigned N>
	struct literal final {


		// -- assertions ------------------------------------------------------

		/* check for null size */
		static_assert(N > 0U, "literal: template parameter N must be greater than zero");


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::literal<N>;


		public:

			// -- public constants --------------------------------------------

			/* size */
			static constexpr unsigned size = N - 1U; // assume null-terminated


			// -- public members ----------------------------------------------

			/* data */
			char data[N];


			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			literal(void) = delete;

			/* array constructor */
			template <unsigned S>
			consteval literal(const char (&str)[S]) noexcept
			: data{} {

				static_assert(S == N,
					"literal: array size must be equal to template parameter N");

				for (unsigned i = 0U; i < N; ++i)
					data[i] = str[i];
			}


			/* copy constructor */
			consteval literal(const self&) noexcept = default;

			/* move constructor */
			consteval literal(self&&) noexcept = default;

			/* destructor */
			constexpr ~literal(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			consteval auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			consteval auto operator=(self&&) noexcept -> self& = default;

	}; // class literal


	// -- deduction guides ----------------------------------------------------

	template <unsigned N>
	literal(const char (&)[N]) -> literal<N>;

} // namespace ml

#endif // ml_literal_hpp
