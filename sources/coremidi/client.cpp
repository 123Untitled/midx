#include "coremidi/client.hpp"
#include "coremidi/string.hpp"

#if defined(midx_macos)

#include <unistd.h>


// -- public lifecycle --------------------------------------------------------

/* name constructor */
cm::client::client(const char* name) {

	// create cfstring
	cm::string cstr{name};

	// create client
	const cm::os_status status = ::MIDIClientCreate(cstr, _notification, this, &_ref);

	// check if there was an error
	if (status != noErr)
		throw cm::exception{status, "MIDIClientCreate"};
}

/* move constructor */
cm::client::client(self&& other) noexcept
: cm::object{other._ref} {
	other._ref = 0U;
}

/* destructor */
cm::client::~client(void) noexcept {
	// dispose client
	self::_dispose();
}


// -- public assignment operators ---------------------------------------------

/* move assignment operator */
auto cm::client::operator=(self&& other) noexcept -> self& {

	// check for self assignment
	if (this == &other)
		return *this;

	// dispose client
	self::_dispose();

	// move reference
	_ref       = other._ref;
	other._ref = 0U;

	return *this;
}


// -- private methods ---------------------------------------------------------

/* dispose */
auto cm::client::_dispose(void) noexcept -> void {

	// check client validity
	if (_ref == 0U)
		return;

	return; // no need to dispose client

	// dispose client
	static_cast<void>(
		::MIDIClientDispose(_ref)
	);
}


// -- private static methods --------------------------------------------------

/* notification */
auto cm::client::_notification(const ::MIDINotification* message, void* data) noexcept -> void {

	// get client reference
	self& client = *static_cast<self*>(data);

	switch (message->messageID) {

		// Some aspect of the current MIDI setup changed.
		case kMIDIMsgSetupChanged:
			::write(STDOUT_FILENO, "setup changed", 13);
			break;

		// The system added a device, entity, or endpoint.
		case kMIDIMsgObjectAdded:
			::write(STDOUT_FILENO, "object added", 12);
			break;

		// The system removed a device, entity, or endpoint.
		case kMIDIMsgObjectRemoved:
			::write(STDOUT_FILENO, "object removed", 14);
			break;

		// An object’s property value changed.
		case kMIDIMsgPropertyChanged:
			::write(STDOUT_FILENO, "property changed", 16);
			break;

		// The system created or disposed of a persistent MIDI Thru connection.
		case kMIDIMsgThruConnectionsChanged:
			::write(STDOUT_FILENO, "thru connection changed", 23);
			break;

		// The system changed a serial port owner.
		case kMIDIMsgSerialPortOwnerChanged:
			::write(STDOUT_FILENO, "serial port owner changed", 25);
			break;

		// An I/O error occurred.
		case kMIDIMsgIOError:
			::write(STDOUT_FILENO, "io error", 8);
			break;

		default:
			::write(STDOUT_FILENO, "unknown notification", 20);
	}
}

#endif // midx_macos
