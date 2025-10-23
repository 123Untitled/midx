#include "coremidi/source.hpp"
#include "coremidi/string.hpp"


// -- S O U R C E -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* id constructor */
cm::source::source(const ::MIDIEndpointRef& ref)
: cm::object{ref},
  _external{true} {
}

/* name constructor */
cm::source::source(const cm::client& client, const char* name)
: _external{false} {

	// create cfstring
	cm::string cstr{name};

	// create a new source
	const ::OSStatus err = ::MIDISourceCreateWithProtocol(client.id(),
														  cstr,
														  kMIDIProtocol_1_0,
														  &_ref);

	// check if there was an error
	if (err != noErr)
		throw cm::exception{err, "MIDISourceCreateWithProtocol"};
}

/* move constructor */
cm::source::source(self&& other) noexcept
: cm::object{other._ref},
  _external{other._external} {
	other._ref = 0U;
	other._external = true;
}

/* destructor */
cm::source::~source(void) noexcept {
	// dispose source
	self::_dispose();
}


// -- public assignment operators ---------------------------------------------

/* move assignment operator */
auto cm::source::operator=(self&& other) noexcept -> self& {

	// check self assignment
	if (this == &other)
		return *this;

	// dispose endpoint
	self::_dispose();

	// move source
	_ref       = other._ref;
	other._ref = 0U;
	_external  = other._external;
	other._external = true;

	// return self
	return *this;
}


// -- private methods ---------------------------------------------------------

/* dispose */
auto cm::source::_dispose(void) noexcept -> void {

	// check source mode
	if (_external)
		return;

	// check source validity
	if (_ref == 0U)
		return;

	// dispose endpoint
	static_cast<void>(
		::MIDIEndpointDispose(_ref)
		);
}
