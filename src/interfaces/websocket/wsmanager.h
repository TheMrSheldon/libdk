#ifndef INTERFACES_WEBSOCKET_WSMANAGER_H
#define INTERFACES_WEBSOCKET_WSMANAGER_H

#include "wsplayer.h"

#include <server_ws.hpp>

namespace dki {
	class WSManager {
	private:
		using wsserver = SimpleWeb::SocketServer<SimpleWeb::WS>;
		
		wsserver server;
	public:
		WSManager(int port);
		dki::WSPlayer newPlayer(std::string endpoint) noexcept;
		void start();
		void stop();
	};
}

#endif