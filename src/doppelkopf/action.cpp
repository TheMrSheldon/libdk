#include <doppelkopf/action.h>

using namespace dk;

Action::Action() noexcept : type(ActionType::AnnounceReContra) {}

Action::Action(Card card) noexcept : type(ActionType::PlaceCard), placement({card}) {}

Action Action::NewAnnouncement() noexcept {
	return Action();
}

Action Action::NewPlacementAction(Card card) noexcept {
	return Action(card);
}