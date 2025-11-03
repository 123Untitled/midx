#ifndef protocol_hpp
#define protocol_hpp

#include "diagnostics/runtime_error.hpp"
#include "types.hpp"
#include <string>
#include <iostream>


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- P R O T O C O L -----------------------------------------------------

	class protocol final {

		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::protocol;


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

			using state_func = auto (ml::protocol::*)(const ml::u8*,
													  const ml::u8*) -> void;


			// -- private members ---------------------------------------------

			/* states */
			state_func _states[2U];

			/* index */
			ml::usz _index;

			/* command */
			command _cmd;

			/* size */
			ml::usz _size;

			/* data */
			std::string _data;

			/* complete */
			bool _complete;



			auto _skip_whitespaces(const ml::u8* it,
								   const ml::u8* end) -> void {

				while (it < end
				&& (*it == ' ' || *it == '\n' || *it == '\r' || *it == '\t'))
					++it;

				if (it < end) {
					_index = 0U;
					(this->*_states[0U])(it, end);
				}
			}

			auto _read_status(const ml::u8* it, const ml::u8* end) -> void {

				const ml::u8* checkpoint = it;

				while (it < end && (*it >= 'A' && *it <= 'Z'))
					++it;

				_data.append(
					reinterpret_cast<const char*>(checkpoint),
					static_cast<ml::usz>(it - checkpoint)
				);

				if (it == end)
					// need more data
					return;

				if (_data == "UPDATE") {
					_cmd = command::UPDATE;
					_states[0U] = &ml::protocol::_read_size;
					_data.clear();
				}
				else if (_data == "PLAY") {
					_cmd = command::PLAY;
					_states[0U] = &ml::protocol::_start;
					_data.clear();
				}
				else if (_data == "STOP") {
					_cmd = command::STOP;
					_states[0U] = &ml::protocol::_start;
					_data.clear();
				}
				else if (_data == "TOGGLE") {
					_cmd = command::TOGGLE;
					_states[0U] = &ml::protocol::_start;
					_data.clear();
				}
				else if (_data == "PAUSE") {
					_cmd = command::PAUSE;
					_states[0U] = &ml::protocol::_start;
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

			auto _read_size(const ml::u8* it, const ml::u8* end) -> void {

				constexpr ml::usz max = UINT64_MAX / 10U;

				while (it < end && (*it >= '0' && *it <= '9')) {

					// size overflow
					if (_size > max)
						throw ml::runtime_error{"protocol size overflow"};

					_size *= 10U;
					ml::usz digit = static_cast<ml::usz>(*it - '0');

					// size overflow
					if (_size > (UINT64_MAX - digit))
						throw ml::runtime_error{"protocol size overflow"};

					_size += digit;

					++it;
				}

				if (it == end)
					// need more data
					return;

				if (*it != '\n')
					throw ml::runtime_error{"protocol size termination error"};

				_states[0U] = &ml::protocol::_read_data;
				self::_read_data(++it, end);
			}

			auto _read_data(const ml::u8* it, const ml::u8* end) -> void {

				const auto left = static_cast<ml::usz>(end - it);

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
					_states[0U] = &ml::protocol::_start;
					_index = 0U;
					return;
				}
			}

			auto _start(const ml::u8* it, const ml::u8* end) -> void {

				_complete = false;
				_cmd = command::UNKNOWN;
				_data.clear();
				_size = 0U;
				_states[0U] = &ml::protocol::_read_status;
				_index = 1U;

				(this->*_states[1U])(it, end);
			}


		public:

			protocol(void) noexcept
			: _states{&ml::protocol::_start,
					  &ml::protocol::_skip_whitespaces},
			  _index{1U},
			  _cmd{command::UNKNOWN}, _size{0U}, _data{},
			  _complete{false} {
			}


			auto require_update(void) const noexcept -> bool {
				return _cmd == command::UPDATE
				&& _complete == true;
			}

			auto feed(std::string& data) -> void {
				// process data according to protocol
				(this->*_states[_index])(
					reinterpret_cast<const ml::u8*>(data.data()),
					reinterpret_cast<const ml::u8*>(data.data() + data.size())
				);

				data.clear();
			}

			auto data(void) const noexcept -> const std::string& {
				return _data;
			}

	}; // class protocol

} // namespace ml

#endif // protocol_hpp
