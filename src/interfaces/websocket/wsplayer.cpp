#include "wsplayer.h"

using namespace dki;
using namespace std::literals::string_literals;
using json = nlohmann::json;

WSPlayer::WSPlayer(wsserver::Endpoint& endpoint) : endpoint(endpoint) {
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
}

void WSPlayer::waitForConnection() {
	endpointConnected.get_future().get();
}

bool WSPlayer::hasReservations() {
	//TODO: implement
	return false;
}

void WSPlayer::notifyHasReservation(unsigned player) {
	//TODO: implement
}

void WSPlayer::notifyPlaceCard(unsigned player, dk::Card card) {
	json packet;
	packet["type"] = "place";
	packet["player"] = player;
	packet["card"] = dk::to_string(card);
	connection->send(packet.dump());
}

void WSPlayer::notifyRoundEnd(int winner, std::array<dk::Card, 4> center) {
	json packet;
	packet["type"] = "roundend";
	packet["winner"] = winner;
	std::vector<std::string> cards;
	std::transform(std::begin(center), std::end(center), std::back_inserter(cards), dk::to_string);
	packet["center"] = cards;
	connection->send(packet.dump());
}

void WSPlayer::setState(int pos, std::vector<dk::Card> hand) {
	json packet;
	packet["type"] = "stateupdate";
	packet["position"] = pos;
	std::vector<std::string> cards;
	std::transform(std::begin(hand), std::end(hand), std::back_inserter(cards), dk::to_string);
	packet["hand"] = cards;
	connection->send(packet.dump());
}

dk::Action WSPlayer::getAction() {
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

using MsgHandler = std::function<void(WSPlayer* self, json message)>;
void WSPlayer::handlePlacementMsg(json message) {
	response.set_value(dki::PlacementActionCmd(std::tuple(dk::str_to_card(message["card"].get<std::string>()))));
}

const std::map<std::string, MsgHandler> WSPlayer::messageHandlers = {
	{"place", (MsgHandler)&WSPlayer::handlePlacementMsg}
};

void WSPlayer::handleMessage(json message) noexcept {
	auto& type = message["type"];
	//assert(type.is_string())
	auto& callback = messageHandlers.at(type.get<std::string>());
	callback(this, message);
}