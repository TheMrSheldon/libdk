#include "wsmanager.h"

using namespace dki;
using wsserver = SimpleWeb::SocketServer<SimpleWeb::WS>;

WSManager::WSManager(int port) {
	server.config.port = port;
}

dki::WSPlayer WSManager::newPlayer(std::string endpoint) noexcept {
	return dki::WSPlayer(server.endpoint[endpoint]);
}

void WSManager::start() {
	std::promise<unsigned short> server_port;
	auto thread = std::thread([this, &server_port]() {
		// Start server
		server.start([&server_port](unsigned short port) {
			server_port.set_value(port);
		});
	});
	thread.detach();
	std::cout << "Server listening on port " << server_port.get_future().get() << std::endl;
}

void WSManager::stop() {
	server.stop();
}