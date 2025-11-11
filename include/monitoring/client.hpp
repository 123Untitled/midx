#ifndef monitoring_client_hpp
#define monitoring_client_hpp

#include "system/socket.hpp"
#include "monitoring/watcher.hpp"
#include "reader.hpp"

#include <sys/event.h>
#include <string>


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- forward declarations ------------------------------------------------

	/* monitor */
	class monitor;


	// -- C L I E N T ---------------------------------------------------------

	class client final : public ml::watcher {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::client;


			// -- private members ---------------------------------------------

			/* monitoring */
			const ml::monitor* _monitor;

			/* socket */
			ml::socket _socket;

			/* buffer */
			mx::reader<4096U> _reader;

			/* send buffer */
			std::string _send_buffer;



			// -- private methods ---------------------------------------------

			auto disconnect(const ml::monitor&) -> void;


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			client(void) = delete;

			/* monitor constructor */
			client(const ml::monitor&) noexcept;

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



			/* initialize */
			auto initialize(ml::socket&&, const ml::monitor&) -> void;


			auto send(std::string&&) -> void;


			auto is_connected(void) const noexcept -> bool {
				return !!_socket;
			}


			// -- public overrides --------------------------------------------

			/* on event */
			auto on_event(ml::application&, const struct ::kevent&) -> void override final;

			/* make add */
			auto make_add(void) noexcept -> struct ::kevent override final;

			/* make del */
			auto make_del(void) const noexcept -> struct ::kevent override final;
	};

} // namespace ml

#endif // monitoring_client_hpp
