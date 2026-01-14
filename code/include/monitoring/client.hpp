#ifndef monitoring_client_hpp
#define monitoring_client_hpp

#include "system/socket.hpp"
#include "monitoring/watcher.hpp"
#include "reader.hpp"

#include <string>
#include <deque>

#include "string_pool.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- forward declarations ------------------------------------------------

	/* monitor */
	class monitor;


	class send_buffer final {

		private:
			mx::string _data;
			mx::usz _sent;


		public:

			explicit send_buffer(mx::string&& msg)
			: _data{std::move(msg)}, _sent{0U} {
			}

			auto data(void) const noexcept -> const char* {
				return _data.data() + _sent;
			}

			auto size(void) const noexcept -> mx::usz {
				return _data.size() - _sent;
			}
			auto advance(const mx::usz size) noexcept -> void {
				_sent += size;
			}

			auto is_sent(void) const noexcept -> bool {
				return _sent >= _data.size();
			}

			auto store(void) -> void {
				mx::string_pool::store(std::move(_data));
			}

	}; // class send_buffer


	// -- C L I E N T ---------------------------------------------------------

	class client final : public mx::watcher {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::client;


			// -- private members ---------------------------------------------

			/* monitoring */
			const mx::monitor* _monitor;

			/* socket */
			mx::socket _socket;

			/* buffer */
			mx::reader<4096U> _reader;

			/* send buffer */
			std::deque<mx::send_buffer> _queue;


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			client(void) = delete;

			/* monitor constructor */
			client(const mx::monitor&) noexcept;

			/* deleted copy constructor */
			client(const self&) = delete;

			/* move constructor */
			client(self&&) noexcept = default;

			/* destructor */
			~client(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;



			// -- public methods ----------------------------------------------

			/* initialize */
			auto initialize(mx::socket&&) -> void;

			/* send */
			auto send(mx::string&&) -> void;

			/* is connected */
			auto is_connected(void) const noexcept -> bool;


			// -- public overrides --------------------------------------------

			/* on event */
			auto on_event(mx::application&, const struct ::kevent&) -> void override final;

			/* ident */
			auto ident(void) const noexcept -> int override final;


		private:

			// -- private methods ---------------------------------------------

			/* disconnect */
			auto _disconnect(void) -> void;

			/* read */
			auto _read(mx::application&) -> void;

			/* write */
			auto _write(mx::application&) -> void;

	}; // class client

} // namespace mx

#endif // monitoring_client_hpp
