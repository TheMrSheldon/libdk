#ifndef INTERFACES_WEBSOCKET_WSOBSERVERMANAGER_H
#define INTERFACES_WEBSOCKET_WSOBSERVERMANAGER_H

#include "wsobserver.h"

#include <doppelkopf/doppelkopf.h>

#include <server_ws.hpp>

#include <map>

namespace dki {

	class WSObserverMgr final {
	private:
		using wsserver = SimpleWeb::SocketServer<SimpleWeb::WS>;
		using wsconnection = wsserver::Connection;

		wsserver server;
		wsserver::Endpoint& endpoint;
		std::thread thread;
		dk::Doppelkopf& game;
		std::map<wsconnection*, std::unique_ptr<WSObserver>> observers;
	public:
		WSObserverMgr(int port, dk::Doppelkopf& game);
	};

} // namespace dki

#endif