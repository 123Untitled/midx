#ifndef monitoring_server_hpp
#define monitoring_server_hpp

#include "monitoring/watcher.hpp"
#include "monitoring/client.hpp"


// -- M X  N A M E S P A C E --------------------------------------------------

namespace mx {


	// -- forward declarations ------------------------------------------------

	/* monitor */
	class monitor;


	// -- S E R V E R ---------------------------------------------------------

	class server final : public mx::watcher {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = mx::server;


			// -- private members ---------------------------------------------

			/* socket */
			mx::socket _socket;

			/* client */
			mx::client _client;


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			server(void) = delete;

			/* monitor constructor */
			server(const mx::monitor&);


			// -- public methods ----------------------------------------------

			/* broadcast */
			auto broadcast(const std::string&) -> void;


			// -- public overrides --------------------------------------------

			/* on event */
			auto on_event(mx::application&, const struct ::kevent&) -> void override final;

			/* ident */
			auto ident(void) const noexcept -> int override final;

	}; // class server

} // namespace mx

#endif // monitoring_server_hpp
