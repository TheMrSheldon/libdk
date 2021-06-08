#ifndef DOPPELKOPF_PLAYER_H
#define DOPPELKOPF_PLAYER_H

#include "action.h"
#include "card.h"

#include <vector>

/** @file
 * @brief 
 */

namespace dk {

	/**
	 * @brief The player partially observes (it may not see the other players' cards) a game of doppelkopf and can act in it through callbacks.
	 */
	class Player {
	public:
		/**
		 * @brief Checks if the player has any reservations.
		 * 
		 * @return true 
		 * @return false 
		 */
		virtual bool hasReservations() = 0;

		/**
		 * @brief Notifies the player that another player has a reservation.
		 * 
		 * @param player 
		 */
		virtual void notifyHasReservation(unsigned player) = 0;

		/**
		 * @brief Notifies the player about the new game state.
		 * 
		 * @param pos 
		 * @param cards 
		 */
		virtual void setState(int pos, std::vector<Card> cards) = 0;

		/**
		 * @brief Asks the player to choose his action.
		 * 
		 * @return Action 
		 */
		virtual Action getAction() = 0;
	};

} // namespace dk

#endif