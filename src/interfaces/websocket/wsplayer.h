#ifndef INTERFACES_WEBSOCKET_WSPLAYER_H
#define INTERFACES_WEBSOCKET_WSPLAYER_H

#include <doppelkopf/doppelkopf.h>
#include <doppelkopf/player.h>
#include <doppelkopf/interface/command.h>

#include <server_ws.hpp>
#include <nlohmann/json.hpp>

#include <functional>
#include <future>
#include <map>
#include <string>

namespace dki {

	class WSPlayer final : public dk::Player {
		friend class WSManager;
	private:
		using wsserver = SimpleWeb::SocketServer<SimpleWeb::WS>;
		using wsconnection = wsserver::Connection;
		using wsconnection_ptr = std::shared_ptr<wsconnection>;
		using json = nlohmann::json;

		wsserver::Endpoint& endpoint;
		std::thread thread;
		wsconnection_ptr connection;

		std::promise<nullptr_t> endpointConnected;
		std::promise<dki::AnyCommand> response;
		
		WSPlayer(wsserver::Endpoint& endpoint);
	public:
		void waitForConnection();
		virtual bool hasReservations() override;
		virtual void notifyHasReservation(unsigned player) override;
		virtual void notifyPlaceCard(unsigned player, dk::Card card) override;
		virtual void notifyRoundEnd(int winner, std::array<dk::Card, 4> center) override;
		virtual void setState(int pos, std::vector<dk::Card> hand) override;
		virtual dk::Action getAction() override;
	
	private:
		using MsgHandler = std::function<void(WSPlayer* self, json message)>;
		void handlePlacementMsg(json message);

		static const std::map<std::string, MsgHandler> messageHandlers;

		void handleMessage(json message) noexcept;
	};

} // namespace dki

#endif