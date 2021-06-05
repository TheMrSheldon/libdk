#include <doppelkopf/pattern.h>

using namespace dk;

bool Pattern::matches(const Card& card) const noexcept {
	const auto suitmatch = suit < 0 || suit == (int)card.suit;
	const auto valuematch = value < 0 || value == (int)card.value;
	return suitmatch && valuematch;
}