#ifndef language_identifier_key_hpp
#define language_identifier_key_hpp

#include "language/lexer/lexeme.hpp"


// -- S X  N A M E S P A C E --------------------------------------------------

namespace sx {


	// -- I D E N T I F I E R  K E Y ------------------------------------------

	class identifier_key final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sx::identifier_key;


			// -- private members ---------------------------------------------

			/* lexeme */
			const lx::lexeme* _lexeme;


		public:


			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			identifier_key(void) noexcept = delete;

			/* constructor */
			identifier_key(const lx::lexeme& l) noexcept
			: _lexeme{&l} {
			}


			// -- public structs ----------------------------------------------

			/* hash */
			struct hash final {


				public:

					// -- public operators ------------------------------------

					/* function operator */
					auto operator()(const self& key) const noexcept -> mx::usz {
						return key._lexeme->hash();
					}

			}; // struct hash


			// -- public operators --------------------------------------------

			/* equality operator */
			auto operator==(const self& other) const noexcept -> bool {
				// compare lexemes
				return *_lexeme == *(other._lexeme);
			}

	}; // class identifier_key

} // namespace sx

#endif // language_identifier_key_hpp
