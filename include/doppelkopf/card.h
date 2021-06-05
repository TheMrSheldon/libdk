#pragma once

/** @file
 * @brief 
 */

namespace dk {

	/**
	 * @brief Defines all possible card values in a doppelkopf deck.
	 */
	enum class Value : int {
		Ace = 0, King, Queen, Jack, Ten, Nine
	};
	/**
	 * @brief Defines all possible card suits in a doppelkopf deck.
	 */
	enum class Suit : int {
		Club = 0, Spade, Heart, Diamond
	};
	
	/**
	 * @brief A card is uniquely identified by its suit and value.
	 */
	struct Card final {
		/**
		 * @brief The suit of this card.
		 */
		Suit suit;
		/**
		 * @brief The value of this card.
		 */
		Value value;
		
		/**
		 * @brief Checks if the other card is equal to this one.
		 * 
		 * @param other The card for which to check for equality.
		 * @return true iff the other card matches this in value and suit.
		 * @see Card::operator<=>(const Card& other)
		 */
		constexpr bool operator==(const Card& other) const noexcept = default;

		/**
		 * @brief Checks if the other card is not equal to this one.
		 * 
		 * @param other The card for which to check for inequality.
		 * @return true iff the other card does not match this in value or suit.
		 * @see Card::operator<=>(const Card& other)
		 */
		constexpr bool operator!=(const Card& other) const noexcept = default;

		/**
		 * @brief Checks if the other card is beaten by this one. This comparison ignores trumps. To consider trumps additionally check with Gamemode::isTrump(const Card& card).
		 * 
		 * @param other The card for which to check if it is beaten.
		 * @return true iff the other card is beaten by this one.
		 */
		constexpr bool operator>=(const Card& other) const noexcept = default;

		/**
		 * @brief Checks if the other card beats this one. This comparison ignores trumps. To consider trumps additionally check with Gamemode::isTrump(const Card& card).
		 * 
		 * @param other The card for which to check if it beats this one.
		 * @return true iff the other card beats this one.
		 */
		constexpr bool operator<=(const Card& other) const noexcept = default;
		
		/**
		 * @brief Returns the difference of values of this card and the other's.
		 * 
		 * @return 0 if the cards are equal. >0 if this card beats the other one (ignoring trump). <0 if this card is beaten by the other one (ignoring trump).
		 * @see Card::numericValue()
		 */
		constexpr int operator<=>(const Card& other) const noexcept {
			return numericValue() - other.numericValue();
		}
		
		/**
		 * @brief Returns the pips of this card. The pips denote the score of the card that is tallied up to determine the winner.
		 * 
		 * @return The pips of this card.
		 */
		constexpr int pips() const noexcept {
			constexpr int pips[] = {
				[static_cast<int>(Value::Ace)] = 11,
				[static_cast<int>(Value::King)] = 4,
				[static_cast<int>(Value::Queen)] = 3,
				[static_cast<int>(Value::Jack)] = 2,
				[static_cast<int>(Value::Ten)] = 10,
				[static_cast<int>(Value::Nine)] = 0
			};
			return pips[static_cast<int>(value)];
		}

	private:
		constexpr int numericValue() const noexcept {
			return (int)value * 4 + (int)suit;
		}
	};
	
}