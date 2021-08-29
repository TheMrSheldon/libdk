#ifndef WSPLAYER_H
#define WSPLAYER_H

#include <doppelkopf/doppelkopf.h>
#include <doppelkopf/player.h>

#include <server_ws.hpp>
#include <nlohmann/json.hpp>

#include <functional>
#include <future>
#include <map>
#include <string>

namespace dki {

	class WSPlayer final : public dk::Player {
	private:
		//using namespace std::literals::string_literals;

		using wsserver = SimpleWeb::SocketServer<SimpleWeb::WS>;
		using wsconnection = wsserver::Connection;
		using wsconnection_ptr = std::shared_ptr<wsconnection>;
		using json = nlohmann::json;

		wsserver server;
		wsserver::Endpoint& endpoint;
		std::thread thread;
		wsconnection_ptr connection;

		std::promise<nullptr_t> endpointConnected;
		std::promise<dki::AnyCommand> response;
	public:
		WSPlayer(int port, int playerindex) : endpoint(server.endpoint[std::string("^/player/player")+std::to_string(playerindex)+std::string("?$")]) {
			server.config.port = port;
			endpoint.on_open = [this](std::shared_ptr<wsconnection> connection) {
				this->connection = connection;
				endpointConnected.set_value(nullptr);
			};
			endpoint.on_message = [this](std::shared_ptr<wsconnection> connection, std::shared_ptr<wsserver::InMessage> in_message) {
				std::cout << "Server: Message received: \"" << in_message->string() << "\" from " << connection.get() << std::endl;
				auto message = json::parse(in_message->string());
				handleMessage(message);
			};
			endpoint.on_close = [this](std::shared_ptr<wsconnection> connection, int code, const std::string& message) {
				endpointConnected = {};
			};

			std::promise<unsigned short> server_port;
			thread = std::thread([this, &server_port]() {
				// Start server
				server.start([&server_port](unsigned short port) {
					server_port.set_value(port);
				});
			});
			thread.detach();
			std::string hostname = "localhost";//TODO: retrieve ip
			std::cout << "Tell player " << playerindex << " to connect to ws://" << hostname << ':' << server_port.get_future().get() << "/player/player" << playerindex << std::endl;
		}

		void waitForConnection() {
			endpointConnected.get_future().get();
		}

		virtual bool hasReservations() override {
			//TODO: implement
			return false;
		}

		virtual void notifyHasReservation(unsigned player) override {
			//TODO: implement
		}

		virtual void notifyPlaceCard(unsigned player, dk::Card card) {
			json packet;
			packet["type"] = "place";
			packet["player"] = player;
			packet["card"] = dk::to_string(card);
			connection->send(packet.dump());
		}

		virtual void notifyRoundEnd(int winner, std::array<dk::Card, 4> center) override {
			json packet;
			packet["type"] = "roundend";
			packet["winner"] = winner;
			std::vector<std::string> cards;
			std::transform(std::begin(center), std::end(center), std::back_inserter(cards), dk::to_string);
			packet["center"] = cards;
			connection->send(packet.dump());
		}
		
		virtual void setState(int pos, std::vector<dk::Card> hand) override {
			json packet;
			packet["type"] = "stateupdate";
			packet["position"] = pos;
			std::vector<std::string> cards;
			std::transform(std::begin(hand), std::end(hand), std::back_inserter(cards), dk::to_string);
			packet["hand"] = cards;
			connection->send(packet.dump());
		}

		virtual dk::Action getAction() override {
			response = std::promise<dki::AnyCommand>();
			//Send request for move
			json packet;
			packet["type"] = "getaction";
			connection->send(packet.dump());
			//Wait for answer
			auto action = response.get_future().get();
			auto placementAction = std::get<dki::PlacementActionCmd>(action);
			//TODO: handle announcement actions
			return dk::Action::NewPlacementAction(std::get<0>(placementAction.args));
		}
	
	private:
		using MsgHandler = std::function<void(WSPlayer* self, json message)>;
		void handlePlacementMsg(json message) {
			response.set_value(dki::PlacementActionCmd(std::tuple(dk::str_to_card(message["card"].get<std::string>()))));
		}

		std::map<std::string, MsgHandler> messageHandlers = {
			{"place", (MsgHandler)&WSPlayer::handlePlacementMsg}
		};

		void handleMessage(json message) noexcept {
			auto& type = message["type"];
			//assert(type.is_string())
			messageHandlers[type.get<std::string>()](this, message);
		}
	};

} // namespace dki

#endif