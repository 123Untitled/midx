#include "protocol/parser.hpp"
#include "string_pool.hpp"
#include "application.hpp"


// -- P A R S E R -------------------------------------------------------------

// -- private methods ---------------------------------------------------------

/* handle header */
auto pc::parser::_handle_header(void) -> void {

	const auto to_copy = _to_copy();

	mx::u8* dst = (_header + sizeof(pc::header)) - _needed;

	// copy data
	mx::memcpy(dst, _it, to_copy);

	_needed -= to_copy;
		_it += to_copy;

	if (_needed != 0U)
		return;

	const auto& h = _as_header();

	if (!h.is_valid()) {
		_state = state::resync;
		return;
	}

	const auto len = h.length();

	// no payload
	if (len == 0U) {
		_process_method();
		_needed = sizeof(pc::header);
		return;
	}

	_needed = len;
	_state  = state::on_payload;
	_payload.reserve(len);
}

/* handle payload */
auto pc::parser::_handle_payload(void) -> void {

	const auto to_copy = _to_copy();

	const auto view = _as_string_view(_it, to_copy);
	_payload.append(view);

	_needed -= to_copy;
		_it += to_copy;

	if (_needed != 0U)
		return;

	// done payload
	_needed = sizeof(pc::header);
	_state  = state::on_header;

	// process payload
	_process_payload();
}

/* handle resync */
auto pc::parser::_handle_resync(void) -> void {

	static constexpr mx::u8 magic[4U] {
		'M', 'I', 'D', 'X'
	};
	std::cout << "Resyncing...\r\n";

	for (; _it < _end; ++_it) {

		if (*_it != magic[_resync]) {
			// restart match
			_resync = 0U;
			continue;
		}

		if (++_resync != 4U)
			continue;

		// magic found, rebuild header
		mx::memcpy(_header, magic, 4U);

		_state  = state::on_header;
		_needed = sizeof(header) - sizeof(magic);
		_resync = 0U;

		return;
	}
}

/* process method */
auto pc::parser::_process_method(void) -> void {

	const auto& h = _as_header();
	const auto  m = h.method();

	switch (m) {

		case pc::method::play:
			_app->play();
			break;

		case pc::method::stop:
			_app->stop();
			break;

		case pc::method::toggle:
			_app->toggle();
			break;

		case pc::method::state:
			// app.handle_state(mx::move(_payload));
			break;

		default:
			std::cerr << "Method [" << pc::method_name(m)
					  << "] requires payload\r\n";
			return;
	}
}

/* process payload */
auto pc::parser::_process_payload(void) -> void {

	const auto& h = _as_header();
	const auto  m = h.method();

	switch (m) {

		case pc::method::update:
			_app->reparse(mx::move(_payload));
			break;

		default:
			std::cerr << "Method [" << pc::method_name(m)
					  << "] does not accept payload\r\n";
			_payload.clear();
			return;
	}

	_payload = mx::string_pool::query();
}


// -- public lifecycle --------------------------------------------------------

/* default constructor */
pc::parser::parser(void) noexcept
: _it{nullptr}, _end{nullptr}, _app{nullptr},
  _state{state::on_header},
  _needed{sizeof(pc::header)},
  _resync{0U},
  _header{},
  _payload{} {
}


// -- public methods ----------------------------------------------------------

/* reset */
auto pc::parser::reset(void) noexcept -> void {
	_state  = state::on_header;
	_needed = sizeof(pc::header);
	_resync = 0U;
	_payload.clear();
}
