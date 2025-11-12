#ifndef protocol_hpp
#define protocol_hpp

#include "diagnostics/runtime_error.hpp"
#include "types.hpp"
#include <string>
#include <iostream>
#include "reader.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- forward declarations ------------------------------------------------

	class application;


	// -- P R O T O C O L -----------------------------------------------------

	class protocol final {

		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::protocol;


			enum command : unsigned {
				UPDATE,
				PLAY,
				STOP,
				TOGGLE,
				PAUSE,
				UNKNOWN
			};

			enum state : unsigned {
				READ_STATUS,
				READ_SIZE,
				READ_DATA,
				COMPLETE
			};

			using state_func = auto (mx::protocol::*)(const mx::u8*,
													  const mx::u8*) -> void;


			// -- private members ---------------------------------------------

			/* states */
			state_func _states[2U];

			/* index */
			mx::usz _index;

			/* command */
			command _cmd;

			/* size */
			mx::usz _size;

			/* data */
			std::string _data;

			/* complete */
			bool _complete;



			auto _skip_whitespaces(const mx::u8* it,
								   const mx::u8* end) -> void {

				while (it < end
				&& (*it == ' ' || *it == '\n' || *it == '\r' || *it == '\t'))
					++it;

				if (it < end) {
					_index = 0U;
					(this->*_states[0U])(it, end);
				}
			}

			auto _read_status(const mx::u8* it, const mx::u8* end) -> void {

				const mx::u8* checkpoint = it;

				while (it < end && (*it >= 'A' && *it <= 'Z'))
					++it;

				_data.append(
					reinterpret_cast<const char*>(checkpoint),
					static_cast<mx::usz>(it - checkpoint)
				);

				if (it == end)
					// need more data
					return;

				if (_data == "UPDATE") {
					_cmd = command::UPDATE;
					_states[0U] = &mx::protocol::_read_size;
					_data.clear();
				}
				else if (_data == "PLAY") {
					_cmd = command::PLAY;
					_states[0U] = &mx::protocol::_start;
					_data.clear();
				}
				else if (_data == "STOP") {
					_cmd = command::STOP;
					_states[0U] = &mx::protocol::_start;
					_data.clear();
				}
				else if (_data == "TOGGLE") {
					_cmd = command::TOGGLE;
					_states[0U] = &mx::protocol::_start;
					_data.clear();
				}
				else if (_data == "PAUSE") {
					_cmd = command::PAUSE;
					_states[0U] = &mx::protocol::_start;
					_data.clear();
				}
				else {
					// uncompleted command
					// need to implement error handling
					_data.clear();
					return;
				}

				_index = 1U;
				(this->*_states[1U])(it, end);
			}

			auto _read_size(const mx::u8* it, const mx::u8* end) -> void {

				constexpr mx::usz max = UINT64_MAX / 10U;

				while (it < end && (*it >= '0' && *it <= '9')) {

					// size overflow
					if (_size > max)
						throw mx::runtime_error{"protocol size overflow"};

					_size *= 10U;
					mx::usz digit = static_cast<mx::usz>(*it - '0');

					// size overflow
					if (_size > (UINT64_MAX - digit))
						throw mx::runtime_error{"protocol size overflow"};

					_size += digit;

					++it;
				}

				if (it == end)
					// need more data
					return;

				if (*it != '\n')
					throw mx::runtime_error{"protocol size termination error"};

				_states[0U] = &mx::protocol::_read_data;
				self::_read_data(++it, end);
			}

			auto _read_data(const mx::u8* it, const mx::u8* end) -> void {

				const auto left = static_cast<mx::usz>(end - it);

				const auto min = (_size < left) ? _size : left;

				// read data according to size
				_data.append(
					reinterpret_cast<const char*>(it),
					min
				);

				_size -= min;
				it += min;

				if (_size == 0U) {
					_complete = true;
					_states[0U] = &mx::protocol::_start;
					_index = 0U;
					return;
				}
			}


			auto _start(const mx::u8* it, const mx::u8* end) -> void {

				_complete = false;
				_cmd = command::UNKNOWN;
				_data.clear();
				_size = 0U;
				_states[0U] = &mx::protocol::_read_status;
				_index = 1U;

				(this->*_states[1U])(it, end);
			}


		public:
			auto reset(void) noexcept -> void {
				_complete = false;
				_cmd = command::UNKNOWN;
				_data.clear();
				_size = 0U;
				_states[0U] = &mx::protocol::_read_status;
				_index = 1U;
			}

			protocol(void) noexcept
			: _states{&mx::protocol::_start,
					  &mx::protocol::_skip_whitespaces},
			  _index{1U},
			  _cmd{command::UNKNOWN}, _size{0U}, _data{},
			  _complete{false} {
			}



			auto require_update(void) const noexcept -> bool {
				return _cmd == command::UPDATE
				&& _complete == true;
			}

			template <mx::usz N>
			auto feed(const mx::reader<N>& reader, mx::application& app) -> void {

				// process data according to protocol
				self::_feed(reader.begin(), reader.end(), app);
			}

			auto _feed(const mx::u8*, const mx::u8*, mx::application&) -> void;

			auto data(void) noexcept -> std::string& {
				return _data;
			}

	}; // class protocol

} // namespace mx

#endif // protocol_hpp
