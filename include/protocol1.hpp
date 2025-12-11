#ifndef protocol_hpp
#define protocol_hpp

#include "diagnostics/runtime_error.hpp"
#include "types.hpp"
#include <string>
#include <iostream>
#include "reader.hpp"
#include "string_pool.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- forward declarations ------------------------------------------------

	class application;


	// -- P R O T O C O L -----------------------------------------------------

	class protocol1 final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::protocol1;


			enum state : unsigned {
				READ_STATUS,
				READ_SIZE,
				READ_DATA,
				COMPLETE
			};

			using state_func = auto (mx::protocol1::*)(void) -> void;


			// -- private members ---------------------------------------------

			/* iterator */
			const mx::u8* _it;
			const mx::u8* _end;

			/* application */
			mx::application* _app;

			/* states */
			state_func _state;

			/* size */
			mx::usz _size;

			/* data */
			mx::string _data;



			auto _read_status(void) -> void;
			auto _read_size(void) -> void;
			auto _read_data(void) -> void;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			protocol1(void) noexcept
			: _it{nullptr}, _end{nullptr},
			  _app{nullptr},
			  _state{&mx::protocol1::_read_status},
			  _size{0U},
			  _data{} {
			}


			/* feed */
			template <mx::usz N>
			auto feed(const mx::reader<N>& reader, mx::application& app) -> void {

				 _it = reader.begin();
				_end = reader.end();
				_app = &app;

				while (_it < _end)
					(this->*_state)();
			}

	}; // class protocol

} // namespace mx

#endif // protocol_hpp
