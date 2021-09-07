#include <doppelkopf/gamemode.h>

#include <algorithm>

using dk::Gamemode, dk::Pattern, dk::Card;

Gamemode::Gamemode(std::vector<Pattern> trumpPatterns) noexcept : trumpPatterns(std::move(trumpPatterns)) {}

bool Gamemode::isTrump(const Card& card) const noexcept {
	return std::any_of(trumpPatterns.begin(), trumpPatterns.end(), [card](const auto& pattern){ return pattern.matches(card);});
}

bool Gamemode::serves(const Card& served, const Card& other) const noexcept {
	const bool noneTrump = !isTrump(served) && !isTrump(other);
	const bool bothTrump = isTrump(served) && isTrump(other);
	const bool sameSuit = served.suit == other.suit;
	return bothTrump || (noneTrump && sameSuit);
}

bool Gamemode::beats(const Card& tobeat, const Card& other) const noexcept {
	if (isTrump(tobeat) != isTrump(other)) //exactly one is trump
		return isTrump(other);
	return other > tobeat;
	//FIXME: this does not work for fleshless
	//E.g.
	// No-solo: 10 of hearts beats queen of hearts
	// Fleshless: queen of hearts beats 10 of hearts
}