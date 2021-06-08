#ifndef DOPPELKOPF_PATTERN_H
#define DOPPELKOPF_PATTERN_H

#include "card.h"

/** @file
 * @brief 
 */

namespace dk {

	/**
	 * @brief A pattern describes a card or cards that have a specified suit and / or value.
	 * @details Generally a pattern may be understood as a card that has additional wildcard values for its suit or value.
	 *  cards match a pattern iff they have the same suit or the pattern's suit is a wildcard and if they have the same 
	 *  value or the pattern's value is a wildcard.
	 */
	struct Pattern final {
		/**
		 * @brief The suit this pattern matches against or -1 for wildcard.
		 */
		const int suit;
		/**
		 * @brief The value this pattern matches against or -1 for wildcard.
		 */
		const int value;
		
	private:
		/**
		 * @brief Constructs a new Pattern for the desired suit and value.
		 */
		constexpr Pattern(int suit, int value) noexcept : suit(suit), value(value) {}
		
	public:
		/**
		 * @brief Checks if this pattern matches the provided card.
		 * 
		 * @param card 
		 * @return true 
		 * @return false 
		 */
		[[nodiscard]] bool matches(const Card& card) const noexcept;

		/**
		 * @brief Constructs a new Pattern with the fixed suit and value.
		 */
		constexpr Pattern(Suit suit, Value value) noexcept : Pattern(static_cast<int>(suit), static_cast<int>(value)) {}
		/**
		 * @brief Constructs a new Pattern with the fixed suit and arbitrary value.
		 */
		explicit constexpr Pattern(Suit suit) noexcept : Pattern(static_cast<int>(suit), -1) {}
		/**
		 * @brief Constructs a new Pattern with the fixed value and arbitrary suit.
		 */
		explicit constexpr Pattern(Value value) noexcept : Pattern(-1, static_cast<int>(value)) {}
	};
} // namespace dk

#endif