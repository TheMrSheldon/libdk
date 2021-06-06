#include <doppelkopf/action.h>

using dk::Action;

Action::Action() noexcept : data(AnnouncementData{}) {}

Action::Action(Card card) noexcept : data(PlacementData{card}) {}

Action Action::NewAnnouncement() noexcept {
	return Action();
}

Action Action::NewPlacementAction(Card card) noexcept {
	return Action(card);
}