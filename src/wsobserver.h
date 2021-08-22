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
		
		static std::string to_string(dk::Card card) noexcept {
			std::string str = "";
			switch (card.suit){
				case dk::Suit::Club: str += 'C'; break;
				case dk::Suit::Spade: str += 'S'; break;
				case dk::Suit::Heart: str += 'H'; break;
				case dk::Suit::Diamond: str += 'D'; break;
				default: str += '-'; break;
			}
			switch(card.value) {
				case dk::Value::Ace: str += 'A'; break;
				case dk::Value::King: str += 'K'; break;
				case dk::Value::Queen: str += 'Q'; break;
				case dk::Value::Jack: str += 'J'; break;
				case dk::Value::Ten: str += "10"; break;
				case dk::Value::Nine: str += '9'; break;
				default: str += '-'; break;
			}
			return str;
		}
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
				std::transform(std::begin(hand), std::end(hand), std::back_inserter(cards), to_string);
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
			std::transform(std::begin(cards), std::end(cards), std::back_inserter(cardsStr), to_string);
			packet["cards"] = cardsStr;
			endpoint->send(packet.dump());
		}

		virtual void notifyAnnouncement(int player) noexcept override {
			
		}
	};

} // namespace dki

#endif