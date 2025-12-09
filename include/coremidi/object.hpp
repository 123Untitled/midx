#ifndef coremidi_object_hpp
#define coremidi_object_hpp

#include "os.hpp"

#if defined(midx_macos)

#include "coremidi/types.hpp"
#include "coremidi/exception.hpp"
#include "coremidi/string.hpp"

#include "core/string/string.hpp"


// -- C O R E M I D I  N A M E S P A C E --------------------------------------

namespace cm {


	// -- O B J E C T ---------------------------------------------------------

	class object {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = cm::object;


		protected:

			// -- private members ---------------------------------------------

			/* reference */
			::MIDIObjectRef _ref;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			object(void) noexcept
			: _ref{0U} {
			}

			/* reference constructor */
			object(const cm::object_ref& ref) noexcept
			: _ref{ref} {
			}

			/* copy constructor */
			object(const self&) noexcept = default;

			/* move constructor */
			object(self&&) noexcept = default;

			/* destructor */
			~object(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public accessors --------------------------------------------

			/* name */
			auto name(void) const -> mx::string {
				return self::_get_string_property(kMIDIPropertyName);
			}

			/* model */
			auto model(void) const -> mx::string {
				return self::_get_string_property(kMIDIPropertyModel);
			}

			/* manufacturer */
			auto manufacturer(void) const -> mx::string {
				return self::_get_string_property(kMIDIPropertyManufacturer);
			}

			/* unique id */
			auto unique_id(void) const -> cm::i32 {
				return self::_get_integer_property(kMIDIPropertyUniqueID);
			}


			// -- public modifiers --------------------------------------------

			/* name */
			auto name(const char* name) -> void {
				self::_set_string_property(kMIDIPropertyName, name);
			}

			/* model */
			auto model(const char* model) -> void {
				self::_set_string_property(kMIDIPropertyModel, model);
			}

			/* manufacturer */
			auto manufacturer(const char* manufacturer) -> void {
				self::_set_string_property(kMIDIPropertyManufacturer, manufacturer);
			}

			/* unique id */
			auto unique_id(const cm::i32 uid) -> void {
				self::_set_integer_property(kMIDIPropertyUniqueID, uid);
			}



			// -- public boolean operators ------------------------------------

			/* boolean operator */
			explicit operator bool(void) const noexcept {
				return _ref != 0U;
			}

			/* not operator */
			auto operator!(void) const noexcept -> bool {
				return _ref == 0U;
			}


			// -- public accessors --------------------------------------------

			/* id */
			auto id(void) const noexcept -> cm::object_ref {
				return _ref;
			}


		private:

			// -- private methods ---------------------------------------------


			/* get string property */
			auto _get_string_property(const ::CFStringRef& id) const -> mx::string {

				// cfstring declaration
				cm::string cstr;

				// get string property
				const cm::os_status status = ::MIDIObjectGetStringProperty(_ref, id, &cstr._string);


				if (status != noErr)
					throw cm::exception{status, "MIDIObjectGetStringProperty"};

				// get property length
				const ::CFIndex len = cstr.length();

				// check if length is not null
				if (len == 0)
					return mx::string{};

				mx::string property;

				property.resize(static_cast<cm::u32>(len));

				// copy property to cstring
				::CFStringGetCString(cstr, property.data(), len + 1U, kCFStringEncodingASCII);

				// return allocated string
				return property;
			}

			/* set string property */
			auto _set_string_property(const ::CFStringRef& id, const char* value) -> void {

				// create cfstring
				cm::string cstr{value};

				// set name
				const cm::os_status status = ::MIDIObjectSetStringProperty(_ref, id, cstr);

				if (status != noErr)
					throw cm::exception{status, "MIDIObjectSetStringProperty"};
			}

			/* set integer property */
			auto _set_integer_property(const ::CFStringRef& id, const cm::i32 value) -> void {

				// set integer property
				const cm::os_status status = ::MIDIObjectSetIntegerProperty(_ref, id, value);

				if (status != noErr)
					throw cm::exception{status, "MIDIObjectSetIntegerProperty"};
			}

			/* get integer property */
			auto _get_integer_property(const ::CFStringRef& id) const -> cm::i32 {

				cm::i32 value = 0;

				// get integer property
				const cm::os_status status = ::MIDIObjectGetIntegerProperty(_ref, id, &value);

				if (status != noErr)
					throw cm::exception{status, "MIDIObjectGetIntegerProperty"};

				return value;
			}

	}; // class object

} // namespace coremidi

#endif // midx_macos

#endif // coremidi_object_hpp


	//midi::set_string_property(_client, kMIDIPropertyName, name);
	//midi::set_string_property(_client, kMIDIPropertyModel, "model_1.0");
	//midi::set_string_property(_client, kMIDIPropertyManufacturer, "TMS_Industry");
	//
	//// capabilities
	//
	//midi::set_integer_property(_client, kMIDIPropertySupportsMMC, 0);
	//midi::set_integer_property(_client, kMIDIPropertySupportsGeneralMIDI, 0);
	//midi::set_integer_property(_client, kMIDIPropertySupportsShowControl, 0);
	//
	//// banks
	//
	//midi::set_integer_property(_client, kMIDIPropertyReceivesBankSelectLSB, 0);
	//midi::set_integer_property(_client, kMIDIPropertyTransmitsBankSelectLSB, 0);
	//midi::set_integer_property(_client, kMIDIPropertyReceivesBankSelectMSB, 0);
	//midi::set_integer_property(_client, kMIDIPropertyTransmitsBankSelectMSB, 0);
	//
	//// notes
	//
	//midi::set_integer_property(_client, kMIDIPropertyReceivesNotes, 1);
	//midi::set_integer_property(_client, kMIDIPropertyTransmitsNotes, 1);
	//
	//
	//// program changes
	//
	//midi::set_integer_property(_client, kMIDIPropertyReceivesProgramChanges, 0);
	//midi::set_integer_property(_client, kMIDIPropertyTransmitsProgramChanges, 0);
	//
	//// channels
	//
	//// The maximum number of MIDI channels on which a device may simultaneously receive channel messages.
	//midi::set_integer_property(_client, kMIDIPropertyMaxReceiveChannels, 0x7F);
	//// The maximum number of MIDI channels on which a device may simultaneously transmit channel messages.
	//midi::set_integer_property(_client, kMIDIPropertyMaxTransmitChannels, 0x7F);
	//// The bitmap of channels on which the object receives messages.
	//midi::set_integer_property(_client, kMIDIPropertyReceiveChannels, 0x7F);
	//// The bitmap of channels on which the object transmits messages.
	//midi::set_integer_property(_client, kMIDIPropertyTransmitChannels, 0x7F);
	//
	//
	//// connections
	//
	//// A Boolean value that indicates whether the device or entity can route messages to or from external MIDI devices.
	//midi::set_integer_property(_client, kMIDIPropertyCanRoute, 0);
	//// A Boolean value that indicates whether the endpoint broadcasts messages to all of the other endpoints in the device.
	//midi::set_integer_property(_client, kMIDIPropertyIsBroadcast, 0);
	//// The unique identifier of an external device attached to this connection.
	//midi::set_integer_property(_client, kMIDIPropertyConnectionUniqueID, 0);
	//// A Boolean value that indicates whether this entity or endpoint has external MIDI connections.
	//midi::set_integer_property(_client, kMIDIPropertyIsEmbeddedEntity, 0);
	//// The 0-based index of the entity on which incoming real-time messages from the device appear to have originated.
	//midi::set_integer_property(_client, kMIDIPropertySingleRealtimeEntity, 0);
	//
