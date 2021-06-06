#ifndef DOPPELKOPF_DOPPELKOPF_H
#define DOPPELKOPF_DOPPELKOPF_H

#include "card.h"
#include "observer.h"
#include "player.h"

#include <array>
#include <cstdint>
#include <functional>
#include <vector>

/** @file
 * @brief 
 */

namespace dk {

	/**
	 * @brief Contains and combines the logic needed for running an entire game of doppelkopf via callbacks to provided player implementations.
	 */
	class Doppelkopf final {
	public:
		/**
		 * @brief The size of a deck of doppelkopf cards: 4 suits &times; 6 values &times; each card twice.
		 */
		static constexpr size_t DeckSize = 4*6*2;
		/**
		 * @brief The number of players in a game of doppelkopf.
		 */
		static constexpr size_t NumPlayers = 4;
	private:
		static const std::array<Card, DeckSize> AllCards;
		
		State state;
		std::array<std::reference_wrapper<Player>, NumPlayers> players;
		std::vector<std::reference_wrapper<Observer>> observers;

		void setupGame() noexcept;
		void handleReservations() noexcept;
	public:
		/**
		 * @brief Constructs a new doppelkopf instance played by the provided dk::Player implementations.
		 * 
		 * @param player1 The first player.
		 * @param player2 The second player.
		 * @param player3 The third player.
		 * @param player4 The fourth player.
		 */
		Doppelkopf(Player& player1, Player& player2, Player& player3, Player& player4) noexcept;

		/**
		 * @brief Adds a new observer to the game.
		 * @details Opposite to a dk::Player an dk::Observer gets notified with all information about the currently
		 * 		played game. They may as such be used to implement user interfaces that output the current state of
		 * 		the game.
		 * @param observer The observer that should be added.
		 */
		void addObserver(Observer& observer) noexcept;

		/**
		 * @brief Launches and plays an entire game of doppelkopf.
		 * @details The game is played by repeatedly asking the players for their moves. When observerers were added
		 * 		they are notified about all changes to the game's current state.
		 */
		void runGame() noexcept;
	};
} // namespace dk

#endif