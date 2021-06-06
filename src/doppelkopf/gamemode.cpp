#include <doppelkopf/gamemode.h>

#include <algorithm>

using dk::Gamemode, dk::Pattern, dk::Card;

Gamemode::Gamemode(std::vector<Pattern> trumpPatterns) noexcept : trumpPatterns(std::move(trumpPatterns)) {}

bool Gamemode::isTrump(const Card& card) const noexcept {
	return std::any_of(trumpPatterns.begin(), trumpPatterns.end(), [card](const auto& pattern){ return pattern.matches(card);});
}

bool Gamemode::serves(const Card& served, const Card& other) const noexcept {
	return (isTrump(served) && isTrump(other)) ||
		(served.suit == other.suit);
}

bool Gamemode::beats(const Card& tobeat, const Card& other) const noexcept {
	if (isTrump(tobeat) != isTrump(other)) //exactly one is trump
		return isTrump(other);
	return other > tobeat;
	//FIXME: this does not work
	//E.g.
	// No-solo: queen of diamonds beats king of diamonds
	// Fleshless: king of diamonds beats queen of diamonds
	//E.g.
	// No-solo: 10 of hearts beats ace of hearts
	// Fleshless: ace of hearts beats 10 of hearts
}