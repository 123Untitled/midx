#ifndef protocol_parser_hpp
#define protocol_parser_hpp

#include "protocol/header.hpp"

#include "core/string/string.hpp"
#include "reader.hpp"


// -- forward declarations ----------------------------------------------------

namespace mx { class application; }


// -- P C  N A M E S P A C E --------------------------------------------------


namespace pc {


	// -- P A R S E R ---------------------------------------------------------

	class parser final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = pc::parser;


			/* state */
			enum class state : mx::u32 {
				on_header,
				on_payload,
				resync
			};


			// -- private members ---------------------------------------------

			/* iterator */
			const mx::u8* _it;

			/* end */
			const mx::u8* _end;

			/* application */
			mx::application* _app;


			/* current state */
			state _state;

			/* bytes needed */
			mx::u32 _needed;

			/* resync counter */
			mx::u32 _resync;

			/* header storage */
			mx::u8 _header[sizeof(pc::header)];

			/* payload storage */
			mx::string _payload;




			// -- private methods ---------------------------------------------

			/* as header */
			auto _as_header(void) const noexcept -> const pc::header& {
				return *reinterpret_cast<const pc::header*>(_header);
			}

			/* as string view */
			auto _as_string_view(const mx::u8* data,
								 const mx::usz size) const noexcept -> mx::string_view {
				return mx::string_view{reinterpret_cast<const char*>(data), size};
			}


			/* handle header */
			auto _handle_header(void) -> void;

			/* handle payload */
			auto _handle_payload(void) -> void;

			/* handle resync */
			auto _handle_resync(void) -> void;


			/* process method */
			auto _process_method(void) -> void;

			/* process payload */
			auto _process_payload(void) -> void;


			/* to copy */
			auto _to_copy(void) const noexcept -> mx::usz {
				const mx::usz left = static_cast<mx::usz>(_end - _it);
				return _needed < left ? _needed : left;
			}


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			parser(void) noexcept;

			/* destructor */
			~parser(void) noexcept = default;


			// -- public methods ----------------------------------------------

			/* reset */
			auto reset(void) noexcept -> void;

			/* feed */
			template <auto N>
			auto feed(const mx::reader<N>& rd, mx::application& app) -> void {

				_it  = rd.begin();
				_end = rd.end();
				_app = &app;

				while (_it < _end) {

					switch (_state) {

						case state::on_header:
							_handle_header();
							break;

						case state::on_payload:
							_handle_payload();
							break;

						case state::resync:
							_handle_resync();
							break;
					}
				}
			}

	}; // class parser

} // namespace pc

#endif // protocol_parser_hpp
