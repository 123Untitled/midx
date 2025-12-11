#include "protocol1.hpp"
#include "application.hpp"



auto mx::protocol1::_read_status(void) -> void {

	const mx::u8* checkpoint = _it;

	while (_it < _end && (*_it >= 'A' && *_it <= 'Z'))
		++_it;

	const mx::string_view view{
		reinterpret_cast<const char*>(checkpoint),
		static_cast<mx::usz>(_it - checkpoint)
	};

	_data.append(view);

	if (_it == _end)
		return;

	while (*_it == '\n')
		++_it;

	if (mx::str_equal(_data, "UPDATE")) {
		_state = &mx::protocol1::_read_size;
		_size = 0U;
		_data.clear();
	}

	else if (mx::str_equal(_data, "TOGGLE")) {
		_state = &mx::protocol1::_read_status;
		_app->toggle();
		_data.clear();
	}
	else
		_state = &mx::protocol1::_read_status;
}

auto mx::protocol1::_read_size(void) -> void {

	constexpr mx::usz max = UINT64_MAX / 10U;

	while (_it < _end && (*_it >= '0' && *_it <= '9')) {

		// size overflow
		if (_size > max)
			throw mx::runtime_error{"protocol size overflow"};

		_size *= 10U;
		mx::usz digit = static_cast<mx::usz>(*_it - '0');

		// size overflow
		if (_size > (UINT64_MAX - digit))
			throw mx::runtime_error{"protocol size overflow"};

		_size += digit;

		++_it;
	}

	// need more data
	if (_it == _end)
		return;

	if (*_it != '\n')
		throw mx::runtime_error{"protocol size termination error"};

	_state = &mx::protocol1::_read_data;
	++_it;
}

auto mx::protocol1::_read_data(void) -> void {

	const auto left = static_cast<mx::usz>(_end - _it);

	const auto min = (_size < left) ? _size : left;

	const mx::string_view view{
		reinterpret_cast<const char*>(_it),
		min
	};

	// read data according to size
	_data.append(view);

	_size -= min;
	_it   += min;

	if (_size == 0U) {
		_app->reparse(std::move(_data));
		_state = &mx::protocol1::_read_status;
		_data = mx::string_pool::query();
		return;
	}
}
