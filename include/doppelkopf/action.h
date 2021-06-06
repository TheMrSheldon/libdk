#ifndef DOPPELKOPF_ACTION_H
#define DOPPELKOPF_ACTION_H

#include "card.h"

#include <variant>

/** @file
 * @brief Contains the dk::Action class.
 */

namespace dk {
	
	/**
	 * @brief This enum is used by dk::Action to identify the different action-types.
	 */
	enum class ActionType {
		PlaceCard,			/**< Denotes an action that places a card from the hand in the center. */
		AnnounceReContra	/**< Denotes the optional action of announceing "re" or "contra". */
	};

	/**
	 * @brief Players can perform actions on the current state.
	 */
	class Action final {
	private:
		struct PlacementData {
			Card card;
		};
		struct AnnouncementData {};
		std::variant<PlacementData, AnnouncementData> data;

		Action() noexcept;
		explicit Action(Card card) noexcept;
	public:
		
		/**
		 * @brief Retrieves the ActionType of this action to differentiate if a card was placed or an announcement was made.
		 * @return The type of action stored.
		 */
		[[nodiscard]] constexpr ActionType getType() const noexcept {
			return std::holds_alternative<PlacementData>(data)? ActionType::PlaceCard : ActionType::AnnounceReContra;
		}

		/**
		 * @brief Returns the card that was placed in this action.
		 * @details If this action is a placement action (if Action::getType() is equal to ActionType::PlaceCard) this function
		 *   returns the card that was placed. The behavior is undefined if the action is not a placement action and this method
		 *   should not be called then.
		 * @return The card that is placed in this action.
		 * @throws std::bad_variant_access if getType() is not ActionType::PlaceCard
		 */
		[[nodiscard]] constexpr Card getPlacedCard() const {
			return std::get<PlacementData>(data).card;
		}

		/**
		 * @brief Creates a new re / contra announcement action.
		 * @details An announcement action contains only the fact that something was announcement. The content of the announcement
		 *   (re or contra; no 90, no 60, no 30, black) is inferred from the previous announcements.
		 *   TODO: does that make sense? No "Sprungabsagen" allowed.
		 * @return Action The newly created announcement action.
		 */
		static Action NewAnnouncement() noexcept;

		/**
		 * @brief Creates a new placement action.
		 * 
		 * @param card The card that should be placed.
		 * @return Action The newly created placement action.
		 */
		static Action NewPlacementAction(Card card) noexcept;
	};
} // namespace dk

#endif