#ifndef language_identifier_key_hpp
#define language_identifier_key_hpp

#include "language/ast/block.hpp"


// -- S X  N A M E S P A C E --------------------------------------------------

namespace sx {


	// -- I D E N T I F I E R  K E Y ------------------------------------------

	class identifier_key final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sx::identifier_key;


			// -- private members ---------------------------------------------

			/* specifier id */
			const sp::id _spec_id;

			/* lexeme */
			const lx::lexeme* _lexeme;
			///* block */
			//const as::block* _block;


		public:


			// -- public lifecycle --------------------------------------------

			/* constructor */
			identifier_key(const as::block& b) noexcept
			//: _block{&b} {
			: _spec_id{b.spec_id()},
			  _lexeme{&b.identifier().lexeme} {
			}

			identifier_key(const sp::id id, const lx::lexeme& l) noexcept
			: _spec_id{id},
			  _lexeme{&l} {
			}


			// -- public structs ----------------------------------------------

			/* hash */
			struct hash final {


				public:

					// -- public operators ------------------------------------

					/* function operator */
					auto operator()(const self& key) const noexcept -> mx::usz {

						// assume token is not anonymous
						//auto&  tk = key._block->identifier();
						//sp::id id = key._block->spec_id();
						//
						//mx::usz h = tk.lexeme.hash();
						//return  h ^ static_cast<mx::usz>(id)
						//		  + 0x9e3779b9 + (h << 6U)
						//					   + (h >> 2U);

						mx::usz h = key._lexeme->hash();
						return  h ^ static_cast<mx::usz>(key._spec_id)
								  + 0x9e3779b9 + (h << 6U)
											   + (h >> 2U);
					}

			}; // struct hash


			// -- public operators --------------------------------------------

			/* equality operator */
			auto operator==(const self& other) const noexcept -> bool {

				// assume token is not anonymous

				// compare spec ids
				//if (_block->spec_id() != other._block->spec_id())
				//	return false;
				//
				//// compare lexemes
				//const auto& lx1 =       _block->identifier().lexeme;
				//const auto& lx2 = other._block->identifier().lexeme;
				//return lx1 == lx2;

				// compare spec ids
				if (_spec_id != other._spec_id)
					return false;

				// compare lexemes
				return *_lexeme == *(other._lexeme);
			}

	}; // class identifier_key

} // namespace sx

#endif // language_identifier_key_hpp
