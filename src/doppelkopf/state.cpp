#include <doppelkopf/state.h>

#include <doppelkopf/doppelkopf.h>
#include <doppelkopf/gamemodes.h>

#include <algorithm>
#include <numeric>

using namespace dk;

template<typename C, typename T>
static inline bool contains(const C& vec, const T& element) noexcept {
	return std::find(std::begin(vec), std::end(vec), element) != std::end(vec);
}

State::State() noexcept : gamemode(std::nullopt) {}

State::PlayerState& State::getPlayerState(uint index) {
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
		playersTurn = winner;
	}
}

bool State::isLegal(Card card) const noexcept {
	const bool playerHasCard = contains(getHand(playersTurn), card);
	const bool isFirstPlayer = firstPlayer == playersTurn;
	const bool playerMustServe = !isFirstPlayer && canServe();
	const bool cardServes = isFirstPlayer || gamemode->get()->serves(getPlacedCard(firstPlayer).value(), card);
	return playerHasCard && !(playerMustServe && !cardServes);
}

bool State::placeCard(Card card) noexcept {
	bool legal = isLegal(card);
	if (legal) {
		if (isRoundOver())
			nextRound();
		else
			playersTurn = (playersTurn+1) % playerStates.size();
	}
	return legal;
}

bool State::canServe() const noexcept {
	if (firstPlayer == playersTurn)
		return true;
	const auto& gm = *gamemode->get();
	const auto& hand = getHand(playersTurn);
	const auto& toserve = getPlacedCard(firstPlayer).value();
	return std::any_of(hand.begin(), hand.end(), [gm, toserve](const auto& card){ return gm.serves(toserve, card); });
}

int State::getTurn() const noexcept {
	return playersTurn;
}

std::optional<Card> State::getPlacedCard(unsigned player) const {
	return getPlayerState(player).placed;
}

const std::vector<Card>& State::getHand(unsigned player) const {
	return getPlayerState(player).hand;
}