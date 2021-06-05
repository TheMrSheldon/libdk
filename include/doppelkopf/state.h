#pragma once

#include "card.h"
#include "gamemode.h"

#include <array>
#include <memory>
#include <optional>
#include <vector>

/** @file
 * @brief 
 */

namespace dk {
	
	/**
	 * @brief The state stores information about the player whose turn it is, the placed cards, the selected gamemode (solos, ...) and the re and contra announcements.
	 * 
	 * The State class implements the core logic of the game with respect to the provided gamemode. Players are represented as indexes (0 to 3). The state then stores
	 * the player whose turn it is currently, the hand for each player, the placed center cards, and the score of each player. It additionally stores the gamemode to
	 * check the legality of performed moves.
	 */
	class State final {
		friend class Doppelkopf;
	private:
		std::optional<std::unique_ptr<Gamemode>> gamemode;

		int firstPlayer = 0;
		int playersTurn = 0;
		struct PlayerState {
			std::vector<Card> hand;
			std::optional<Card> placed;
			int score;
		};
		std::array<PlayerState, 4> playerStates;
		
		/**
		 * @brief Sets the gamemode for this gemestate.
		 * 
		 * This method should be called **exactly once** per game to set the gamemode after the players' announcements after the cards were dealt.
		 * 
		 * @tparam Mode the type of the gamemode to be instantiated for this gamestate.
		 */
		template<class Mode>
		void setGameMode() noexcept {
			//assert gamemode is empty
			std::unique_ptr<Gamemode> ptr = std::make_unique<Mode>();
			gamemode = std::make_optional<std::unique_ptr<Gamemode>>(std::move(ptr));
		}
		
		PlayerState& getPlayerState(uint index);
		const PlayerState& getPlayerState(uint index) const;
		bool isRoundOver() const noexcept;
		bool isGameOver() const noexcept;
		int getRoundWinner() const noexcept;
		void nextRound() noexcept;

		State() noexcept;
	public:
		/**
		 * @brief Checks if placing the provided card is legal for the player whose turn it is.
		 * 
		 * Checks if placing the provided card is legal given this gamestate and gamemode. The placement is legal iff all the following requirements are met:
		 *  - the player whose turn it is holds the card in his hand
		 *  - the player can serve and the card is serving
		 *  - the player can't serve
		 * 
		 * @param card the card that should be played.
		 * @return true iff placing the specified card is legal in the this gamestate under the rules specified by the current gamemode.
		 */

		bool isLegal(Card card) const noexcept;
		/**
		 * @brief Places the provided card as the player whose turn it is.
		 * 
		 * Checks if the current card can be placed by the current player according to the gamemode. If it is possible the card will be placed
		 * and this state is updated accordingly. If the round is over after the turn, the next round is started and the first player to play
		 * a card is picked to be the winner of the current round. If the round is not over the player whose turn it is is advanced by one.
		 * 
		 * @param card the card that should be placed by the current player.
		 * @return true iff the provided card could be placed.
		 * @see isLegal
		 * @see getTurn
		 */
		bool placeCard(Card card) noexcept;

		/**
		 * @brief Checks if the current player can serve the center.
		 * 
		 * @return true iff the current player can serve the center. 
		 * @see Gamemode::serves
		 */
		bool canServe() const noexcept;

		/**
		 * @brief Retrieves the index of the player whose turn it is.
		 * 
		 * @return int the index of the player whose turn it is (ranged from 0 to 3).
		 */
		int getTurn() const noexcept;
		/**
		 * @brief Retrieves the card the player has placed in the middle in the current round.
		 * 
		 * @param player 
		 * @return std::optional<Card> 
		 */
		std::optional<Card> getPlacedCard(unsigned player) const;
		/**
		 * @brief Retrieves the cards the player currently holds.
		 * 
		 * @param player 
		 * @return const std::vector<Card>& 
		 */
		const std::vector<Card>& getHand(unsigned player) const;
	};

}