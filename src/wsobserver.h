#ifndef WSOBSERVER_H
#define WSOBSERVER_H

#include <doppelkopf/doppelkopf.h>
#include <doppelkopf/observer.h>

#include <server_ws.hpp>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <future>

namespace dki {

	class WSObserver final : public dk::Observer {
		friend class WSObserverMgr;
	private:
		using wsserver = SimpleWeb::SocketServer<SimpleWeb::WS>;
		using wsconnection = wsserver::Connection;
		using wsconnection_ptr = std::shared_ptr<wsconnection>;
		using json = nlohmann::json;

		const wsconnection_ptr endpoint;
	public:
		WSObserver(wsconnection_ptr endpoint) : endpoint(endpoint) {}

		virtual void notifyStateUpdate(const dk::State& state) noexcept override {
			json packet;
			packet["type"] = "stateupdate";
			packet["can_serve"] = state.canServe();
			packet["turn"] = state.getTurn();
			packet["hands"] = {"", "", "", ""};
			for (auto player = 0; player < dk::Doppelkopf::NumPlayers; ++player) {
				const auto& hand = state.getHand(player);
				std::vector<std::string> cards;
				std::transform(std::begin(hand), std::end(hand), std::back_inserter(cards), dk::to_string);
				packet["hands"][player] = cards;
			}
			endpoint->send(packet.dump());
		}

		virtual void notifyPlaceCard(int player, dk::Card card) noexcept override {
			json packet;
			packet["type"] = "place";
			packet["player"] = player;
			packet["card"] = to_string(card);
			endpoint->send(packet.dump());
		}

		virtual void notifyRoundEnd(int winner, std::array<dk::Card, 4> cards) noexcept override {
			json packet;
			packet["type"] = "round_end";
			packet["winner"] = winner;
			std::vector<std::string> cardsStr;
			std::transform(std::begin(cards), std::end(cards), std::back_inserter(cardsStr), dk::to_string);
			packet["cards"] = cardsStr;
			endpoint->send(packet.dump());
		}

		virtual void notifyAnnouncement(int player) noexcept override {
			
		}
	};

} // namespace dki

#endif