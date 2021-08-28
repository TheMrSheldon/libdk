#include <doppelkopf/doppelkopf.h>

#include <doppelkopf/gamemodes.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <sstream>

using dk::Doppelkopf, dk::Card, dk::Suit, dk::Player, dk::Observer, dk::ActionType;

const std::array<Card, Doppelkopf::DeckSize> Doppelkopf::AllCards = {
	Card{Suit::Club, Value::Ace}, Card{Suit::Club, Value::King}, Card{Suit::Club, Value::Queen}, Card{Suit::Club, Value::Jack}, Card{Suit::Club, Value::Ten}, Card{Suit::Club, Value::Nine},
	Card{Suit::Club, Value::Ace}, Card{Suit::Club, Value::King}, Card{Suit::Club, Value::Queen}, Card{Suit::Club, Value::Jack}, Card{Suit::Club, Value::Ten}, Card{Suit::Club, Value::Nine},
	Card{Suit::Spade, Value::Ace}, Card{Suit::Spade, Value::King}, Card{Suit::Spade, Value::Queen}, Card{Suit::Spade, Value::Jack}, Card{Suit::Spade, Value::Ten}, Card{Suit::Spade, Value::Nine},
	Card{Suit::Spade, Value::Ace}, Card{Suit::Spade, Value::King}, Card{Suit::Spade, Value::Queen}, Card{Suit::Spade, Value::Jack}, Card{Suit::Spade, Value::Ten}, Card{Suit::Spade, Value::Nine},
	Card{Suit::Heart, Value::Ace}, Card{Suit::Heart, Value::King}, Card{Suit::Heart, Value::Queen}, Card{Suit::Heart, Value::Jack}, Card{Suit::Heart, Value::Ten}, Card{Suit::Heart, Value::Nine},
	Card{Suit::Heart, Value::Ace}, Card{Suit::Heart, Value::King}, Card{Suit::Heart, Value::Queen}, Card{Suit::Heart, Value::Jack}, Card{Suit::Heart, Value::Ten}, Card{Suit::Heart, Value::Nine},
	Card{Suit::Diamond, Value::Ace}, Card{Suit::Diamond, Value::King}, Card{Suit::Diamond, Value::Queen}, Card{Suit::Diamond, Value::Jack}, Card{Suit::Diamond, Value::Ten}, Card{Suit::Diamond, Value::Nine},
	Card{Suit::Diamond, Value::Ace}, Card{Suit::Diamond, Value::King}, Card{Suit::Diamond, Value::Queen}, Card{Suit::Diamond, Value::Jack}, Card{Suit::Diamond, Value::Ten}, Card{Suit::Diamond, Value::Nine}
};

void Doppelkopf::setupGame() noexcept {
	constexpr size_t CardsPerHand = DeckSize / NumPlayers;
	auto deck = std::vector<Card>(std::begin(AllCards), std::end(AllCards));
	std::random_device rd;
    std::mt19937 g(rd());
	std::shuffle(deck.begin(), deck.end(), g);
	for (int i = 0; i < players.size(); ++i) {
		auto& playerState = state.getPlayerState(i);
		playerState.hand = std::vector<Card>(deck.begin()+i*CardsPerHand, deck.begin()+(i+1)*CardsPerHand);
		players[i].get().setState(i, playerState.hand);
	}
}

void Doppelkopf::handleReservations() noexcept {
	//Following is temporarily disabled
	/*std::vector<std::reference_wrapper<Player>> reservations;
	for (int i = 0; i < players.size(); ++i) {
		auto& player = players[i];
		if (player.get().hasReservations()) {
			reservations.push_back(player);
			for (auto p : players)
				p.get().notifyHasReservation(i);
		}
	}*/
	//TODO: check the reservations
	state.setGameMode<dk::gm::Default>(); //ignoring the reservations for now
}

Doppelkopf::Doppelkopf(Player& player1, Player& player2, Player& player3, Player& player4) noexcept : players({player1, player2, player3, player4}) {}

void Doppelkopf::addObserver(Observer& observer) noexcept {
	observers.emplace_back(observer);
	//Tell the new observer about the current state
	observer.notifyStateUpdate(state);
}

void Doppelkopf::runGame() noexcept {
	std::cout << "[Starting Game]" << std::endl;
	setupGame();
	handleReservations();
	while (!state.isGameOver()) {
		std::cout << "[New Round]" << std::endl;
		while (!state.isRoundOver()) {
			std::cout << "turn: " << state.getTurn() << std::endl;
			auto turn = state.getTurn();
			auto& player = players[turn].get();
			const auto action = player.getAction();
			switch (action.getType()) {
			case ActionType::AnnounceReContra:
				//TODO: implement
				break;
			case ActionType::PlaceCard:
				if (state.placeCard(action.getPlacedCard())) {
					//Inform players and observers
					player.setState(turn, state.getPlayerState(turn).hand);
					std::for_each(std::begin(players), std::end(players), [turn, action](auto& p){p.get().notifyPlaceCard(turn, action.getPlacedCard());});
					std::for_each(std::begin(observers), std::end(observers), [turn, action](auto& o){o.get().notifyPlaceCard(turn, action.getPlacedCard());});
				} else {
					//Illegal Turn
				}
				break;
			}
		}
		auto winner = state.getRoundWinner();
		std::array<Card, 4> center = {state.getPlacedCard(0).value(), state.getPlacedCard(1).value(), state.getPlacedCard(2).value(), state.getPlacedCard(3).value()};
		std::for_each(std::begin(players), std::end(players), [winner, center](auto& p){p.get().notifyRoundEnd(winner, center);});
		std::for_each(std::begin(observers), std::end(observers), [winner, center](auto& o){o.get().notifyRoundEnd(winner, center);});
		state.nextRound();
	}
	std::cout << "[Game Over]" << std::endl;
}

std::string dk::to_string(const dk::Card& card) noexcept {
	std::string str = "";
	switch (card.suit){
		case dk::Suit::Club: str += 'C'; break;
		case dk::Suit::Spade: str += 'S'; break;
		case dk::Suit::Heart: str += 'H'; break;
		case dk::Suit::Diamond: str += 'D'; break;
		default: str += '-'; break;
	}
	switch(card.value) {
		case dk::Value::Ace: str += 'A'; break;
		case dk::Value::King: str += 'K'; break;
		case dk::Value::Queen: str += 'Q'; break;
		case dk::Value::Jack: str += 'J'; break;
		case dk::Value::Ten: str += "10"; break;
		case dk::Value::Nine: str += '9'; break;
		default: str += '-'; break;
	}
	return str;
}

dk::Card dk::str_to_card(const std::string& string) noexcept {
	dk::Card card;
	std::stringstream stream(string);
	switch (stream.get()) {
	case 'C': card.suit = dk::Suit::Club; break;
	case 'S': card.suit = dk::Suit::Spade; break;
	case 'H': card.suit = dk::Suit::Heart; break;
	case 'D': card.suit = dk::Suit::Diamond; break;
	default: break;
	}
	switch (stream.get()) {
	case 'A': card.value = dk::Value::Ace; break;
	case 'K': card.value = dk::Value::King; break;
	case 'Q': card.value = dk::Value::Queen; break;
	case 'J': card.value = dk::Value::Jack; break;
	case '1':
		/*assert(stream.get() == '0')*/
		card.value = dk::Value::Ten;
		break;
	case '9': card.value = dk::Value::Nine;
	default: break;
	}
	return card;
}