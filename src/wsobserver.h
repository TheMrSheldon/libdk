#ifndef WSOBSERVER_H
#define WSOBSERVER_H

#include <doppelkopf/observer.h>

#include <server_ws.hpp>

#include <future>

namespace dki {

	class WSObserver final : public dk::Observer {
	private:
		using wsserver = SimpleWeb::SocketServer<SimpleWeb::WS>;
		using wsconnection = wsserver::Connection;

		wsserver server;
		wsserver::Endpoint& endpoint;
		std::thread thread;
	public:
		WSObserver(int port) : endpoint(server.endpoint["^/observer/?$"]) {
			server.config.port = port;
			endpoint.on_open = [](std::shared_ptr<wsconnection> connection) {
				std::cout << "New Connection: " << connection.get() << std::endl;
			};
			endpoint.on_message = [](std::shared_ptr<wsconnection> connection, std::shared_ptr<wsserver::InMessage> in_message) {
				std::cout << "Server: Message received: \"" << in_message->string() << "\" from " << connection.get() << std::endl;
			};
			endpoint.on_close = [](std::shared_ptr<wsconnection> connection, int code, const std::string& message) {
				
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

		virtual void notifyStateUpdate(const dk::State& state) noexcept override {
			
		}

		virtual void notifyPlaceCard(int player, dk::Card card) noexcept override {
			
		}

		virtual void notifyAnnouncement(int player) noexcept override {
			
		}
	};

} // namespace dki

#endif