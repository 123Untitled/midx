#include "protocol.hpp"
#include "application.hpp"




/* feed */
auto mx::protocol::_feed(const mx::u8* it, const mx::u8* end, mx::application& app) -> void {
	(this->*_states[_index])(it, end);

	if (_cmd == command::UPDATE && _complete == true) {
		app.reparse(std::move(_data));
		self::reset();
	}
}
