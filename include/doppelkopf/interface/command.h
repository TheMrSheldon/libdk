#ifndef DOPPELKOPF_INTERFACE_COMMAND_H
#define DOPPELKOPF_INTERFACE_COMMAND_H

#include <doppelkopf/card.h>

#include <string>
#include <tuple>
#include <variant>
#include <vector>

/**
 * @defgroup interface Doppelkopf Interface
 * @brief 
 * @details 
 */

/** @file
 * @brief 
 * @ingroup interface
 */

/** @namespace dki
 * @brief Contains interface definitions
 * @ingroup interface
 */
namespace dki {
	/**
	 * @ingroup interface
	 * @{
	 */
	/**
	 * @brief Used to uniquely identify commands-types.
	 */
	enum class CommandID {
		Log, SetState, HasReservations, Reservation, GetAction, PlacementAction, AnnouncementAction
	};

	/**
	 * @brief 
	 * 
	 * @tparam id 
	 * @tparam Args 
	 */
	template<CommandID id, typename... Args>
	class Command final {
	public:
		/**
		 * @brief The command's arguments;
		 */
		std::tuple<Args...> args; // NOLINT(misc-non-private-member-variables-in-classes)
		
		/**
		 * @brief Constructs a new command instance with default arguments.
		 */
		Command() = default;

		/**
		 * @brief Construct a new command instance with the provided arguments.
		 * 
		 * @param args 
		 */
		explicit Command(std::tuple<Args...> args) noexcept : args(args) {}
	};
	
	/**
	 * @brief TODO: document the log command
	 */
	using LogCmd = Command<CommandID::Log, std::string, std::string>;

	/**
	 * @brief TODO: document the setstate command
	 */
	using SetStateCmd = Command<CommandID::SetState, std::vector<dk::Card>>;

	/**
	 * @brief TODO: document the hasreservations command
	 */
	using HasReservationsCmd = Command<CommandID::HasReservations>;
	
	/**
	 * @brief TODO: documentation
	 */
	using ReservationCmd = Command<CommandID::Reservation, bool>;

	/**
	 * @brief TODO: document the getaction command
	 */
	using GetActionCmd = Command<CommandID::GetAction>;

	/**
	 * @brief TODO: documentation
	 */
	using PlacementActionCmd = Command<CommandID::PlacementAction, dk::Card>;

	/**
	 * @brief TODO: documentation
	 */
	using AnyCommand = std::variant<std::monostate, LogCmd, SetStateCmd, HasReservationsCmd, ReservationCmd, GetActionCmd, PlacementActionCmd>;
	///@}
} // namespace dki

#endif