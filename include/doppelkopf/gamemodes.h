#ifndef DOPPELKOPF_GAMEMODES_H
#define DOPPELKOPF_GAMEMODES_H

#include "card.h"
#include "gamemode.h"

/**
 * @defgroup gamemodes Gamemodes
 * @brief Gamemodes are defined here
 * @details All standard gamemodes (the default gamemode where no solo is played, the suit-solos and fleshless) are already predefined.
 */

/** @file
 * @brief 
 * @ingroup gamemodes
 */

/** @namespace dk::gm
 * @brief Contains the default gamemodes and auxiliary definitions.
 * @ingroup gamemodes
 */
namespace dk::gm {
	/**
	 * @addtogroup gamemodes
	 * @{
	 */
	/**
	 * @brief A gamemode with compile time constant trump patterns.
	 * 
	 * @tparam trumps Patterns describing the trump cards.
	 */
	template<Pattern... trumps>
	class TemplateGamemode final : public Gamemode {
	public:
		/**
		 * @brief Constructs a new dk::Gamemode instance using the patterns passed via the template argument.
		 */
		TemplateGamemode() noexcept : Gamemode({trumps...}) {}
	};

	/**
	 * @brief In the default gamemode the ten of hearts, all queens, all jacks and all diamonds are trump.
	 */
	using Default = TemplateGamemode<Pattern{Suit::Heart, Value::Ten}, Pattern{Value::Queen}, Pattern{Value::Jack}, Pattern{Suit::Diamond}>;
	/**
	 * @brief In the club solo is the same as the Default gamemode but clubs are tump instead of diamonds.
	 */
	using ClubSolo = TemplateGamemode<Pattern{Suit::Heart, Value::Ten}, Pattern{Value::Queen}, Pattern{Value::Jack}, Pattern{Suit::Club}>;
	/**
	 * @brief In the spade solo is the same as the Default gamemode but spades are tump instead of diamonds.
	 */
	using SpadeSolo = TemplateGamemode<Pattern{Suit::Heart, Value::Ten}, Pattern{Value::Queen}, Pattern{Value::Jack}, Pattern{Suit::Spade}>;
	/**
	 * @brief In the heart solo is the same as the Default gamemode but hearts are tump instead of diamonds.
	 */
	using HeartSolo = TemplateGamemode<Pattern{Suit::Heart, Value::Ten}, Pattern{Value::Queen}, Pattern{Value::Jack}, Pattern{Suit::Heart}>;
	/**
	 * @brief A solo with the same trumps as the Default gamemode.
	 */
	using DiamondSolo = Default;
	/**
	 * @brief A solo where there are no trumps.
	 */
	using Fleshless = TemplateGamemode<>;
	
	/** @} */
} // namespace dk::gm

#endif