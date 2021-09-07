#ifndef INTERFACES_WEBSOCKET_WSOBSERVER_H
#define INTERFACES_WEBSOCKET_WSOBSERVER_H

#include <doppelkopf/doppelkopf.h>
#include <doppelkopf/observer.h>

#include <server_ws.hpp>

namespace dki {

	class WSObserver final : public dk::Observer {
		friend class WSObserverMgr;
	private:
		using wsserver = SimpleWeb::SocketServer<SimpleWeb::WS>;
		using wsconnection = wsserver::Connection;
		using wsconnection_ptr = std::shared_ptr<wsconnection>;

		const wsconnection_ptr endpoint;
	public:
		WSObserver(wsconnection_ptr endpoint);
		virtual void notifyStateUpdate(const dk::State& state) noexcept override;
		virtual void notifyPlaceCard(int player, dk::Card card) noexcept override;
		virtual void notifyRoundEnd(int winner, std::array<dk::Card, 4> cards) noexcept override;
		virtual void notifyAnnouncement(int player) noexcept override;
	};

} // namespace dki

#endif