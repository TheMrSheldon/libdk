#ifndef DOPPELKOPF_GAMEMODE_H
#define DOPPELKOPF_GAMEMODE_H

#include "card.h"
#include "pattern.h"

#include <vector>

/** @file
 * @brief 
 * @ingroup gamemodes
 */

namespace dk {

	/**
	 * @brief A gamemode dictates what cards are trump.
	 */
	class Gamemode {
	private:
		/**
		 * @brief The patterns that match all trumps in this gamemode.
		 */
		const std::vector<Pattern> trumpPatterns;

	public:
		/**
		 * @brief Construct a new gamemode that treats cards matching the provided patterns as trump.
		 * 
		 * @param trumpPatterns 
		 */
		explicit Gamemode(std::vector<Pattern> trumpPatterns) noexcept;
		
		/**
		 * @brief Checks if the provided card is considered trump in this gamemode.
		 * 
		 * @param card the card to be checked for if it is trump.
		 * @return true if the card matches any of the previously declared trump patterns for this gamemode.
		 */
		[[nodiscard]] bool isTrump(const Card& card) const noexcept;

		/**
		 * @brief Checks if the provided card serves the other.
		 * 
		 * @param served 
		 * @param other 
		 * @return true 
		 */
		[[nodiscard]] bool serves(const Card& served, const Card& other) const noexcept;
		
		/**
		 * @brief Checks if the provided card beats the other.
		 * 
		 * @param tobeat 
		 * @param other 
		 * @return true 
		 */
		[[nodiscard]] bool beats(const Card& tobeat, const Card& other) const noexcept;
	};

} // namespace dk

#endif