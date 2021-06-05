#include <doppelkopf/gamemodes.h>

#include <catch2/catch.hpp>

#include <algorithm>

TEST_CASE("Default Gamemode Trumps", "[gamemode]") {
	constexpr dk::Card trumps[] = {
		{dk::Suit::Heart, dk::Value::Ten},
		{dk::Suit::Club, dk::Value::Queen}, {dk::Suit::Spade, dk::Value::Queen}, {dk::Suit::Heart, dk::Value::Queen}, {dk::Suit::Diamond, dk::Value::Queen},
		{dk::Suit::Club, dk::Value::Jack}, {dk::Suit::Spade, dk::Value::Jack}, {dk::Suit::Heart, dk::Value::Jack}, {dk::Suit::Diamond, dk::Value::Jack},
		{dk::Suit::Diamond, dk::Value::Ace}, {dk::Suit::Diamond, dk::Value::Ten}, {dk::Suit::Diamond, dk::Value::King}, {dk::Suit::Diamond, dk::Value::Nine}
	};
	constexpr dk::Card offcolor[] = {
		{dk::Suit::Club, dk::Value::Ace}, {dk::Suit::Club, dk::Value::Ten}, {dk::Suit::Club, dk::Value::King}, {dk::Suit::Club, dk::Value::Nine},
		{dk::Suit::Spade, dk::Value::Ace}, {dk::Suit::Spade, dk::Value::Ten}, {dk::Suit::Spade, dk::Value::King}, {dk::Suit::Spade, dk::Value::Nine},
		{dk::Suit::Heart, dk::Value::Ace}, {dk::Suit::Heart, dk::Value::King}, {dk::Suit::Heart, dk::Value::Nine}
	};
	dk::gm::Default gamemode;
	REQUIRE(std::all_of(std::begin(trumps), std::end(trumps), [&gamemode](const auto& card){ return gamemode.isTrump(card); }));
	REQUIRE(std::all_of(std::begin(offcolor), std::end(offcolor), [&gamemode](const auto& card){ return !gamemode.isTrump(card); }));
}

TEST_CASE("Club Solo Gamemode Trumps", "[gamemode]") {
	constexpr dk::Card trumps[] = {
		{dk::Suit::Heart, dk::Value::Ten},
		{dk::Suit::Club, dk::Value::Queen}, {dk::Suit::Spade, dk::Value::Queen}, {dk::Suit::Heart, dk::Value::Queen}, {dk::Suit::Diamond, dk::Value::Queen},
		{dk::Suit::Club, dk::Value::Jack}, {dk::Suit::Spade, dk::Value::Jack}, {dk::Suit::Heart, dk::Value::Jack}, {dk::Suit::Diamond, dk::Value::Jack},
		{dk::Suit::Club, dk::Value::Ace}, {dk::Suit::Club, dk::Value::Ten}, {dk::Suit::Club, dk::Value::King}, {dk::Suit::Club, dk::Value::Nine}
	};
	constexpr dk::Card offcolor[] = {
		{dk::Suit::Spade, dk::Value::Ace}, {dk::Suit::Spade, dk::Value::Ten}, {dk::Suit::Spade, dk::Value::King}, {dk::Suit::Spade, dk::Value::Nine},
		{dk::Suit::Heart, dk::Value::Ace}, {dk::Suit::Heart, dk::Value::King}, {dk::Suit::Heart, dk::Value::Nine},
		{dk::Suit::Diamond, dk::Value::Ace}, {dk::Suit::Diamond, dk::Value::Ten}, {dk::Suit::Diamond, dk::Value::King}, {dk::Suit::Diamond, dk::Value::Nine}
	};
	dk::gm::ClubSolo gamemode;
	REQUIRE(std::all_of(std::begin(trumps), std::end(trumps), [&gamemode](const auto& card){ return gamemode.isTrump(card); }));
	REQUIRE(std::all_of(std::begin(offcolor), std::end(offcolor), [&gamemode](const auto& card){ return !gamemode.isTrump(card); }));
}

TEST_CASE("Spade Solo Gamemode Trumps", "[gamemode]") {
	constexpr dk::Card trumps[] = {
		{dk::Suit::Heart, dk::Value::Ten},
		{dk::Suit::Club, dk::Value::Queen}, {dk::Suit::Spade, dk::Value::Queen}, {dk::Suit::Heart, dk::Value::Queen}, {dk::Suit::Diamond, dk::Value::Queen},
		{dk::Suit::Club, dk::Value::Jack}, {dk::Suit::Spade, dk::Value::Jack}, {dk::Suit::Heart, dk::Value::Jack}, {dk::Suit::Diamond, dk::Value::Jack},
		{dk::Suit::Spade, dk::Value::Ace}, {dk::Suit::Spade, dk::Value::Ten}, {dk::Suit::Spade, dk::Value::King}, {dk::Suit::Spade, dk::Value::Nine}
	};
	constexpr dk::Card offcolor[] = {
		{dk::Suit::Club, dk::Value::Ace}, {dk::Suit::Club, dk::Value::Ten}, {dk::Suit::Club, dk::Value::King}, {dk::Suit::Club, dk::Value::Nine},
		{dk::Suit::Heart, dk::Value::Ace}, {dk::Suit::Heart, dk::Value::King}, {dk::Suit::Heart, dk::Value::Nine},
		{dk::Suit::Diamond, dk::Value::Ace}, {dk::Suit::Diamond, dk::Value::Ten}, {dk::Suit::Diamond, dk::Value::King}, {dk::Suit::Diamond, dk::Value::Nine}
	};
	dk::gm::SpadeSolo gamemode;
	REQUIRE(std::all_of(std::begin(trumps), std::end(trumps), [&gamemode](const auto& card){ return gamemode.isTrump(card); }));
	REQUIRE(std::all_of(std::begin(offcolor), std::end(offcolor), [&gamemode](const auto& card){ return !gamemode.isTrump(card); }));
}

TEST_CASE("Heart Solo Gamemode Trumps", "[gamemode]") {
	constexpr dk::Card trumps[] = {
		{dk::Suit::Heart, dk::Value::Ten},
		{dk::Suit::Club, dk::Value::Queen}, {dk::Suit::Spade, dk::Value::Queen}, {dk::Suit::Heart, dk::Value::Queen}, {dk::Suit::Diamond, dk::Value::Queen},
		{dk::Suit::Club, dk::Value::Jack}, {dk::Suit::Spade, dk::Value::Jack}, {dk::Suit::Heart, dk::Value::Jack}, {dk::Suit::Diamond, dk::Value::Jack},
		{dk::Suit::Heart, dk::Value::Ace}, {dk::Suit::Heart, dk::Value::King}, {dk::Suit::Heart, dk::Value::Nine}
	};
	constexpr dk::Card offcolor[] = {
		{dk::Suit::Club, dk::Value::Ace}, {dk::Suit::Club, dk::Value::Ten}, {dk::Suit::Club, dk::Value::King}, {dk::Suit::Club, dk::Value::Nine},
		{dk::Suit::Spade, dk::Value::Ace}, {dk::Suit::Spade, dk::Value::Ten}, {dk::Suit::Spade, dk::Value::King}, {dk::Suit::Spade, dk::Value::Nine},
		{dk::Suit::Diamond, dk::Value::Ace}, {dk::Suit::Diamond, dk::Value::Ten}, {dk::Suit::Diamond, dk::Value::King}, {dk::Suit::Diamond, dk::Value::Nine}
	};
	dk::gm::HeartSolo gamemode;
	REQUIRE(std::all_of(std::begin(trumps), std::end(trumps), [&gamemode](const auto& card){ return gamemode.isTrump(card); }));
	REQUIRE(std::all_of(std::begin(offcolor), std::end(offcolor), [&gamemode](const auto& card){ return !gamemode.isTrump(card); }));
}

TEST_CASE("Diamond Solo Gamemode Trumps", "[gamemode]") {
	constexpr dk::Card trumps[] = {
		{dk::Suit::Heart, dk::Value::Ten},
		{dk::Suit::Club, dk::Value::Queen}, {dk::Suit::Spade, dk::Value::Queen}, {dk::Suit::Heart, dk::Value::Queen}, {dk::Suit::Diamond, dk::Value::Queen},
		{dk::Suit::Club, dk::Value::Jack}, {dk::Suit::Spade, dk::Value::Jack}, {dk::Suit::Heart, dk::Value::Jack}, {dk::Suit::Diamond, dk::Value::Jack},
		{dk::Suit::Diamond, dk::Value::Ace}, {dk::Suit::Diamond, dk::Value::Ten}, {dk::Suit::Diamond, dk::Value::King}, {dk::Suit::Diamond, dk::Value::Nine}
	};
	constexpr dk::Card offcolor[] = {
		{dk::Suit::Club, dk::Value::Ace}, {dk::Suit::Club, dk::Value::Ten}, {dk::Suit::Club, dk::Value::King}, {dk::Suit::Club, dk::Value::Nine},
		{dk::Suit::Spade, dk::Value::Ace}, {dk::Suit::Spade, dk::Value::Ten}, {dk::Suit::Spade, dk::Value::King}, {dk::Suit::Spade, dk::Value::Nine},
		{dk::Suit::Heart, dk::Value::Ace}, {dk::Suit::Heart, dk::Value::King}, {dk::Suit::Heart, dk::Value::Nine}
	};
	dk::gm::DiamondSolo gamemode;
	REQUIRE(std::all_of(std::begin(trumps), std::end(trumps), [&gamemode](const auto& card){ return gamemode.isTrump(card); }));
	REQUIRE(std::all_of(std::begin(offcolor), std::end(offcolor), [&gamemode](const auto& card){ return !gamemode.isTrump(card); }));
}