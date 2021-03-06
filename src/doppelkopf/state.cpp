#include <doppelkopf/state.h>

#include <doppelkopf/doppelkopf.h>
#include <doppelkopf/gamemodes.h>

#include <algorithm>
#include <numeric>

using dk::State, dk::Card;

template<typename C, typename T>
static inline bool contains(const C& vec, const T& element) noexcept {
	return std::find(std::begin(vec), std::end(vec), element) != std::end(vec);
}

template<typename C, typename T>
static inline bool remove_first(C& vec, const T& element) noexcept {
	auto it = std::find(std::begin(vec), std::end(vec), element);
	if (it != std::end(vec)) vec.erase(it);
	return it != std::end(vec);
}

State::State() noexcept : gamemode(std::nullopt) {}

State::PlayerState& State::getPlayerState(uint index) noexcept(false) {
	return playerStates.at(index);
}
const State::PlayerState& State::getPlayerState(uint index) const {
	return playerStates.at(index);
}
bool State::isRoundOver() const noexcept {
	return std::all_of(playerStates.begin(), playerStates.end(), [](const PlayerState& state){ return state.placed.has_value(); });
}
bool State::isGameOver() const noexcept {
	return std::all_of(playerStates.begin(), playerStates.end(), [](const PlayerState& state){ return state.hand.empty(); });
}
int State::getRoundWinner() const noexcept {
	if (!isRoundOver())
		return -1;
	auto winner = firstPlayer;
	for (int turn = 1; turn < Doppelkopf::NumPlayers; turn++) {
		auto player = (firstPlayer + turn) % Doppelkopf::NumPlayers;
		if (gamemode->get()->beats(getPlacedCard(winner).value(), getPlacedCard(player).value()))
			winner = player;
	}
	return winner;
}
void State::nextRound() noexcept {
	auto winner = getRoundWinner();
	if (winner >= 0) {
		const auto pips = std::accumulate(playerStates.begin(), playerStates.end(), 0, [](int prev, const PlayerState& state){ return prev+state.placed->pips(); });
		getPlayerState(winner).score += pips;
		for (auto&& p : playerStates)
			p.placed = std::nullopt;
		firstPlayer = playersTurn = winner;
	}
}

bool State::isLegal(Card card) const noexcept {
	try {
		const bool playerHasCard = contains(getHand(playersTurn), card);
		const bool isFirstPlayer = firstPlayer == playersTurn;
		const bool playerMustServe = !isFirstPlayer && canServe();
		const bool cardServes = isFirstPlayer || gamemode->get()->serves(getPlacedCard(firstPlayer).value(), card);
		return playerHasCard && !(playerMustServe && !cardServes);
	} catch(std::exception const& e) {
		//TODO: log error
		return false;
	}
}

bool State::placeCard(Card card) noexcept {
	bool legal = !isRoundOver() && isLegal(card);
	if (legal) {
		remove_first(getPlayerState(playersTurn).hand, card);
		getPlayerState(playersTurn).placed = card;
		playersTurn = (playersTurn+1) % static_cast<int>(playerStates.size());
	}
	return legal;
}

bool State::canServe() const noexcept {
	try {
		if (firstPlayer == playersTurn)
			return true;
		const auto& gm = **gamemode;
		const auto& hand = getHand(playersTurn);
		const auto& toserve = getPlacedCard(firstPlayer).value();
		return std::any_of(hand.begin(), hand.end(), [gm, toserve](const auto& card){ return gm.serves(toserve, card); });
	} catch (std::exception const& e) {
		return false;
	}
}

int State::getTurn() const noexcept {
	return playersTurn;
}

const std::optional<Card>& State::getPlacedCard(unsigned player) const {
	return getPlayerState(player).placed;
}

const std::vector<Card>& State::getHand(unsigned player) const {
	return getPlayerState(player).hand;
}