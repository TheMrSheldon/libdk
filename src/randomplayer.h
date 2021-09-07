#pragma once

#include <doppelkopf/player.h>

#include <algorithm>
#include <random>

class RandomPlayer final : public dk::Player {
private:
	unsigned myindex;
	std::vector<dk::Card> cards;
	std::mt19937 prng = std::mt19937{std::random_device{}()};

public:
	virtual bool hasReservations() override {
		return false;
	}

	virtual void notifyHasReservation(unsigned player) override {/*Ignored*/}

	virtual void notifyPlaceCard(unsigned player, dk::Card card) {/*Ignored*/}

	virtual void notifyRoundEnd(int winner, std::array<dk::Card, 4> cards) override {/*Ignored*/}
	
	virtual void setState(int pos, std::vector<dk::Card> cards) override {
		myindex = pos;
		this->cards = cards;
	}

	virtual dk::Action getAction() override {
		std::vector<dk::Card> card;
		std::ranges::sample(cards, std::back_inserter(card), 1, prng);
		//TODO: assert(card.size() == 1)
		return dk::Action::NewPlacementAction(card[0]);
	}
};