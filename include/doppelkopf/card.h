#ifndef DOPPELKOPF_CARD_H
#define DOPPELKOPF_CARD_H

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
		Suit suit; // NOLINT(misc-non-private-member-variables-in-classes)
		/**
		 * @brief The value of this card.
		 */
		Value value; // NOLINT(misc-non-private-member-variables-in-classes)
		
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
		 * @return The pips of this card or -1 if the Card::value is invalid.
		 */
		[[nodiscard]] constexpr int pips() const noexcept {
			switch (value){
				case Value::Ace: return AcePips;
				case Value::King: return KingPips;
				case Value::Queen: return QueenPips;
				case Value::Jack: return JackPips;
				case Value::Ten: return TenPips;
				case Value::Nine: return NinePips;
				default: return -1;
			}
		}

	private:
		static constexpr int AcePips = 11;
		static constexpr int KingPips = 4;
		static constexpr int QueenPips = 3;
		static constexpr int JackPips = 2;
		static constexpr int TenPips = 10;
		static constexpr int NinePips = 0;

		[[nodiscard]] constexpr int numericValue() const noexcept {
			return static_cast<int>(value) * 4 + static_cast<int>(suit);
		}
	};
}  // namespace dk

#endif