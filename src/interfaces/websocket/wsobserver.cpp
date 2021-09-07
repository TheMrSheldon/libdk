#include "wsobserver.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <future>
#include <vector>

using namespace dki;
using json = nlohmann::json;

WSObserver::WSObserver(wsconnection_ptr endpoint) : endpoint(endpoint) {}

void WSObserver::notifyStateUpdate(const dk::State& state) noexcept {
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

void WSObserver::notifyPlaceCard(int player, dk::Card card) noexcept {
	json packet;
	packet["type"] = "place";
	packet["player"] = player;
	packet["card"] = to_string(card);
	endpoint->send(packet.dump());
}

void WSObserver::notifyRoundEnd(int winner, std::array<dk::Card, 4> cards) noexcept {
	json packet;
	packet["type"] = "round_end";
	packet["winner"] = winner;
	std::vector<std::string> cardsStr;
	std::transform(std::begin(cards), std::end(cards), std::back_inserter(cardsStr), dk::to_string);
	packet["cards"] = cardsStr;
	endpoint->send(packet.dump());
}

void WSObserver::notifyAnnouncement(int player) noexcept {}