#ifndef monitoring_server_hpp
#define monitoring_server_hpp

#include "monitoring/watcher.hpp"
#include "monitoring/client.hpp"


// -- M L  N A M E S P A C E --------------------------------------------------

namespace ml {


	// -- forward declarations ------------------------------------------------

	/* monitor */
	class monitor;


	// -- S E R V E R ---------------------------------------------------------

	class server final : public ml::watcher {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ml::server;


			// -- private members ---------------------------------------------

			/* socket */
			ml::socket _socket;

			/* client */
			ml::client _client;


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			server(void) = delete;

			/* monitor constructor */
			server(const ml::monitor&);


			// -- public methods ----------------------------------------------

			/* broadcast */
			auto broadcast(const std::string&) -> void;


			// -- public overrides --------------------------------------------

			/* on event */
			auto on_event(ml::application&, const struct ::kevent&) -> void override final;

			/* make add */
			auto make_add(void) noexcept -> struct ::kevent override final;

			/* make del */
			auto make_del(void) const noexcept -> struct ::kevent override final;

	}; // class server

} // namespace ml

#endif // monitoring_server_hpp
