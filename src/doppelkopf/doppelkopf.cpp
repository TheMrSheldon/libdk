#include <doppelkopf/doppelkopf.h>

#include <doppelkopf/gamemodes.h>

#include <iostream>
#include <algorithm>

using namespace dk;

const Card Doppelkopf::AllCards[] = {
	{Suit::Club, Value::Ace}, {Suit::Club, Value::King}, {Suit::Club, Value::Queen}, {Suit::Club, Value::Jack}, {Suit::Club, Value::Ten}, {Suit::Club, Value::Nine},
	{Suit::Club, Value::Ace}, {Suit::Club, Value::King}, {Suit::Club, Value::Queen}, {Suit::Club, Value::Jack}, {Suit::Club, Value::Ten}, {Suit::Club, Value::Nine},
	{Suit::Spade, Value::Ace}, {Suit::Spade, Value::King}, {Suit::Spade, Value::Queen}, {Suit::Spade, Value::Jack}, {Suit::Spade, Value::Ten}, {Suit::Spade, Value::Nine},
	{Suit::Spade, Value::Ace}, {Suit::Spade, Value::King}, {Suit::Spade, Value::Queen}, {Suit::Spade, Value::Jack}, {Suit::Spade, Value::Ten}, {Suit::Spade, Value::Nine},
	{Suit::Heart, Value::Ace}, {Suit::Heart, Value::King}, {Suit::Heart, Value::Queen}, {Suit::Heart, Value::Jack}, {Suit::Heart, Value::Ten}, {Suit::Heart, Value::Nine},
	{Suit::Heart, Value::Ace}, {Suit::Heart, Value::King}, {Suit::Heart, Value::Queen}, {Suit::Heart, Value::Jack}, {Suit::Heart, Value::Ten}, {Suit::Heart, Value::Nine},
	{Suit::Diamond, Value::Ace}, {Suit::Diamond, Value::King}, {Suit::Diamond, Value::Queen}, {Suit::Diamond, Value::Jack}, {Suit::Diamond, Value::Ten}, {Suit::Diamond, Value::Nine},
	{Suit::Diamond, Value::Ace}, {Suit::Diamond, Value::King}, {Suit::Diamond, Value::Queen}, {Suit::Diamond, Value::Jack}, {Suit::Diamond, Value::Ten}, {Suit::Diamond, Value::Nine}
};

void Doppelkopf::setupGame() noexcept {
	constexpr size_t CardsPerHand = DeckSize / NumPlayers;
	auto deck = std::vector<Card>(AllCards, AllCards+DeckSize);
	std::random_shuffle(deck.begin(), deck.end());
	for (int i = 0; i < players.size(); ++i) {
		auto& playerState = state.getPlayerState(i);
		playerState.hand = std::vector<Card>(deck.begin()+i*CardsPerHand, deck.begin()+(i+1)*CardsPerHand);
		players[i].get().setState(i, playerState.hand);
	}
}

void Doppelkopf::handleReservations() noexcept {
	std::vector<std::reference_wrapper<Player>> reservations;
	for (int i = 0; i < players.size(); ++i) {
		auto& player = players[i];
		if (player.get().hasReservations()) {
			reservations.push_back(player);
			for (auto p : players)
				p.get().notifyHasReservation(i);
		}
	}
	//TODO: check the reservations
	state.setGameMode<gm::Default>(); //ignoring the reservations for now
}

Doppelkopf::Doppelkopf(Player& player1, Player& player2, Player& player3, Player& player4) noexcept : players({player1, player2, player3, player4}) {}

void Doppelkopf::addObserver(Observer& observer) noexcept {
	observers.push_back(observer);
}

void Doppelkopf::runGame() noexcept {
	std::cout << "[Starting Game]" << std::endl;
	setupGame();
	handleReservations();
	while (!state.isGameOver()) {
		while (!state.isRoundOver()) {
			auto& player = players[state.getTurn()].get();
			const auto action = player.getAction();
			switch (action.getType()) {
			case ActionType::AnnounceReContra:
				//TODO: implement
				break;
			case ActionType::PlaceCard:
				state.placeCard(action.getPlacedCard());
				break;
			}
		}
	}
}