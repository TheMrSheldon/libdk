#ifndef DOPPELKOPF_OBSERVER_H
#define DOPPELKOPF_OBSERVER_H

#include "state.h"

/** @file
 * @brief 
 */

namespace dk {

	/**
	 * @brief The observer can optionally be implemented to get notified about events and the current state of a doppelkopf game.
	 * 
	 * @see dk::Doppelkopf
	 */
	class Observer {
	public:
		/**
		 * @brief Notifies the observer about an updated state.
		 * 
		 * @param state 
		 */
		virtual void notifyStateUpdate(const State& state) noexcept = 0;

		/**
		 * @brief Notifies the observer that a card was played.
		 * 
		 * @param player the player who placed the card.
		 * @param card the card that was placed.
		 */
		virtual void notifyPlaceCard(int player, Card card) noexcept = 0;

		/**
		 * @brief Notifies the observer that a player made an announcement.
		 * 
		 * @param player the player who made an announcement.
		 */
		virtual void notifyAnnouncement(int player) noexcept = 0;
	};
} // namespace dk

#endif