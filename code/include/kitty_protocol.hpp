
//#include "debug_chars.hpp"
//
//auto has_sync_output() noexcept -> bool {
//	mx::tty tty;
//	tty.blocking_raw();
//	::write(STDOUT_FILENO, "\x1b[?2026$p", 9U);
//	char buff[1024U];
//	auto r = ::read(STDIN_FILENO, buff, 1024U);
//
//	if (r == -1) {
//		tty.restore();
//		return false;
//	}
//	std::cout << "\r\n";
//	for (mx::usz i = 0U; i < static_cast<mx::usz>(r); ++i) {
//		std::cout << mx::dbc{buff[i]} << " ";
//	}
//	std::cout << "\r\n";
//
//
//	if (r >= 11U) {
//
//		// response: CSI ? 2026 ; 0 $ y
//		if (buff[ 0U] == '\x1b'
//		 && buff[ 1U] == '['
//		 && buff[ 2U] == '?'
//		 && buff[ 3U] == '2'
//		 && buff[ 4U] == '0'
//		 && buff[ 5U] == '2'
//		 && buff[ 6U] == '6'
//		 && buff[ 7U] == ';'
//		 && buff[ 8U] == '2'
//		 && buff[ 9U] == '$'
//		 && buff[10U] == 'y') {
//			tty.restore();
//			return true;
//		}
//	}
//
//	// 0 | Mode is not recognized | not supported
//	// 1 | Set                    | supported and screen updates are not shown to the user until mode is disabled
//	// 2 | Reset                  | supported and screen updates are shown as usual (e.g. as soon as they arrive)
//	// 3 | Permanently set        | undefined
//	// 4 | Permanently reset      | not supported
//
//	tty.restore();
//	return false;
//}
//
//auto has_cap(const char* cap) noexcept -> bool {
//	mx::tty tty;
//	tty.blocking_raw();
//	::write(STDOUT_FILENO, cap, mx::strlen(cap));
//	char buff[1024U];
//	auto r = ::read(STDIN_FILENO, buff, 1024U);
//
//	if (r == -1) {
//		tty.restore();
//		return false;
//	}
//
//	std::cout << "\r\n";
//	for (mx::usz i = 0U; i < static_cast<mx::usz>(r); ++i) {
//		std::cout << mx::dbc{buff[i]} << " ";
//	}
//	std::cout << "\r\n";
//
//	tty.restore();
//	return false;
//}
//
//
//namespace kp {
//
//	enum event_type : mx::u8 {
//		press   = 1U,
//		repeat  = 2U,
//		release = 3U,
//	}; // enum event_type
//
//	enum modifier : mx::u8 {
//		shift     =   1U,
//		alt       =   2U,
//		ctrl      =   4U,
//		super     =   8U,
//		hyper     =  16U,
//		meta      =  32U,
//		caps_lock =  64U,
//		num_lock  = 128U,
//	}; // enum modifier
//
//	enum mode :mx::u8 {
//		disambiguate    =  1U,
//		event_types     =  2U,
//		alternate_keys  =  4U,
//		all_as_codes    =  8U,
//		associated_text = 16U,
//	}; // enum mode
//
//
//	class key_event final {
//
//
//		private:
//
//			mx::u32 _code;
//			kp::modifier _mod;
//			kp::event_type _ev;
//
//
//		public:
//
//
//	}; // class key_event
//
//
//	class parser final {
//
//		private:
//
//
//			enum class state : mx::u8 {
//				ground,
//				escape,
//				csi,
//				delimiter
//			}; // enum state
//
//			// -- private members ---------------------------------------------
//
//			/* state */
//			kp::parser::state _state;
//
//
//		public:
//
//			parser(void);
//
//
//			auto feed(const mx::u8 byte, kp::key_event& ev) -> bool {
//
//
//				switch (_state) {
//
//					case state::ground: {
//						if (byte == 0x1bU)
//							_state = state::escape;
//						// normal character
//						// should not happen here
//						break;
//					}
//
//					case state::escape: {
//						if (byte == '[') {
//							_state = state::csi;
//							return false;
//						}
//						// not a CSI sequence
//						reset();
//						break;
//					}
//
//					case state::csi: {
//						break;
//					}
//
//					default: {
//					}
//
//				}
//
//				return false;
//			}
//
//			auto reset(void) -> void {
//				_state = state::ground;
//			}
//
//			auto pending(void) const -> bool {
//				return false;
//			}
//
//	}; // class parser
//
//
//
//};
//
//namespace mx {
//
//
//
//	class keyboard_protocol final {
//
//		public:
//
//			keyboard_protocol(void) {
//
//				constexpr mx::uint mode = 1;
//					//
//
//				constexpr mx::uint flag =
//					// disambiguate escape codes
//					1U |
//					// report event types
//					2U |
//					// report alternate keys
//					4U |
//					// report all keys as escape codes
//					8U |
//					// report associated text
//					16U;
//
//				constexpr mx::chars_encoder enc{"\x1b[>", flag, "u"};
//				constexpr auto size = enc.size();
//				char buff[size];
//				enc.encode(buff);
//
//				const auto ret = ::write(STDOUT_FILENO, buff, size);
//				if (ret == -1)
//					throw mx::system_error{"write"};
//			}
//
//			~keyboard_protocol(void) noexcept {
//				static_cast<void>(::write(STDOUT_FILENO, "\x1b[<u", 4));
//			}
//
//
//	};
//
//
//	template <typename T>
//	auto write(const T& str) -> void {
//		const auto s = mx::strlen(str);
//		const auto w = ::write(STDOUT_FILENO, mx::strptr(str), s);
//		if (w < 0)
//			throw mx::system_error{"write"};
//		if (static_cast<mx::usz>(w) != s)
//			throw mx::runtime_error{"write"};
//	}
//
//} // namespace mx
//
//auto has_keyboard_protocol(void) -> bool {
//
//	mx::tty tty;
//	tty.blocking_raw(1);
//
//	mx::string_view esc{"\x1b[?u"};
//
//	mx::write(esc);
//
//	char buff[1024U];
//	const auto r = ::read(STDIN_FILENO, buff, 1024U);
//
//	if (r < 0)
//		throw mx::system_error{"read"};
//
//	if (r == 0)
//		return false;
//
//	for (mx::usz i = 0U; i < static_cast<mx::usz>(r); ++i) {
//		std::cout << mx::dbc{buff[i]} << " ";
//	}
//	std::cout << "\r\n";
//
//	const auto size = static_cast<mx::usz>(r);
//
//	// expect CSI ? [0-9]+ u
//	if (size < 5U)
//		return false;
//
//	// check CSI ?
//	if (buff[0U] != '\x1b'
//	 || buff[1U] != '['
//	 || buff[2U] != '?')
//		return false;
//
//	mx::usz i = 3U;
//
//	// check digits
//	if (buff[i] < '0' || buff[i] > '9')
//		return false;
//
//	for (; i < size - 1U; ++i) {
//		if (buff[i] < '0' || buff[i] > '9')
//			break;
//	}
//
//	// check final 'u'
//	return (i == size - 1U && buff[i] == 'u');
//}
//
//auto test_input() -> void {
//
//	mx::tty tty;
//	tty.blocking_raw();
//
//	mx::keyboard_protocol kp;
//
//	std::cout << "Type some keys (ESC to exit):\r\n";
//
//	char buff[1024U];
//
//	mx::uint count = 0U;
//	while (true) {
//		auto r = ::read(STDIN_FILENO, buff, 1024U);
//		if (r == -1)
//			throw mx::system_error{"read"};
//
//		for (mx::usz i = 0U; i < static_cast<mx::usz>(r); ++i) {
//			std::cout << mx::dbc{buff[i]} << " ";
//			// exit on ctrl-c or ctrl-d
//			//if (buff[i] == 'q')
//			//	return;
//		}
//		std::cout << "\r\n";
//		++count;
//		if (count >= 10U)
//			break;
//	}
//}
//
