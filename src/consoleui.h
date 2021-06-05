#pragma once

#include <doppelkopf/observer.h>

class ConsoleUI final : public dk::Observer {
public:
	void notifyStateUpdate(const dk::State& state) noexcept {
		
	}

	virtual void notifyPlaceCard(int player, dk::Card card) noexcept {
		
	}

	virtual void notifyAnnouncement(int player) noexcept {
		
	}
};