#include "protocol.hpp"
#include "application.hpp"




/* feed */
auto mx::protocol::_feed(const mx::u8* it, const mx::u8* end, mx::application& app) -> void {
	(this->*_states[_index])(it, end);

	if (_cmd == command::UPDATE && _complete == true) {
		app.reparse(std::move(_data));
		_data = mx::string_pool::query();
		self::reset();
		return;
	}
	if (_cmd == command::TOGGLE) {
		app.toggle_play();
		self::reset();
	}
}
