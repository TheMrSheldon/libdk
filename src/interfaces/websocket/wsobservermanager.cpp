#include "wsobservermanager.h"

#include <nlohmann/json.hpp>

#include <future>

using namespace dki;
using json = nlohmann::json;

WSObserverMgr::WSObserverMgr(int port, dk::Doppelkopf& game) : endpoint(server.endpoint["^/observer/?$"]), game(game) {
	server.config.port = port;
	endpoint.on_open = [this, &game](std::shared_ptr<wsconnection> connection) {
		std::cout << "New Connection: " << connection.get() << std::endl;
		auto observer = std::make_unique<WSObserver>(connection);
		game.addObserver(*observer);
		observers[connection.get()] = std::move(observer);
	};
	endpoint.on_message = [](std::shared_ptr<wsconnection> connection, std::shared_ptr<wsserver::InMessage> in_message) {
		std::cout << "Server: Message received: \"" << in_message->string() << "\" from " << connection.get() << std::endl;
	};
	endpoint.on_close = [](std::shared_ptr<wsconnection> connection, int code, const std::string& message) {
		//TODO: remove and destroy associated observer
	};

	std::promise<unsigned short> server_port;
	thread = std::thread([this, &server_port]() {
		// Start server
		server.start([&server_port](unsigned short port) {
			server_port.set_value(port);
		});
	});
	thread.detach();
	std::cout << "Server listening on port " << server_port.get_future().get() << std::endl;
}