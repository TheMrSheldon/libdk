#include <doppelkopf/pattern.h>

using dk::Pattern, dk::Card;

bool Pattern::matches(const Card& card) const noexcept {
	const auto suitmatch = suit < 0 || suit == static_cast<int>(card.suit);
	const auto valuematch = value < 0 || value == static_cast<int>(card.value);
	return suitmatch && valuematch;
}