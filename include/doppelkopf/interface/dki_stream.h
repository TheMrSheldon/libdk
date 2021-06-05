#pragma once

#include "command.h"

#include <doppelkopf/card.h>

#include <istream>
#include <ostream>
#include <string>
#include <vector>

/** @file
 * @brief 
 * @ingroup interface
 */

namespace dki {
	/**
	 * @brief TODO: document
	 * @ingroup interface
	 */
	class dki_stream final {
	private:
		std::istream& istream;
		std::ostream& ostream;

	public:
		/**
		 * @brief TODO: document
		 * 
		 * @param istream 
		 * @param ostream 
		 */
		dki_stream(std::istream& istream, std::ostream& ostream) noexcept;

		/**
		 * @brief TODO: document
		 * 
		 * @tparam cmd 
		 * @tparam Args 
		 * @param command 
		 * @return const dki_stream& 
		 */
		template<CommandID cmd, typename... Args>
		const dki_stream& operator<<(const Command<cmd, Args...>& command) const noexcept;

		/**
		 * @brief TODO: document
		 * 
		 * @tparam cmd 
		 * @tparam Args 
		 * @param command 
		 * @return const dki_stream& 
		 */
		template<CommandID cmd, typename... Args>
		const dki_stream& operator>>(Command<cmd, Args...>& command) const noexcept;
		
		/**
		 * @brief TODO: document
		 * 
		 * @param str 
		 * @return const dki_stream& 
		 */
		const dki_stream& operator<<(const std::string& str) const noexcept;

		/**
		 * @brief TODO: document
		 * 
		 * @param card 
		 * @return const dki_stream& 
		 */
		const dki_stream& operator<<(const dk::Card& card) const noexcept;

		/**
		 * @brief TODO: document
		 * 
		 * @param b 
		 * @return const dki_stream& 
		 */
		const dki_stream& operator<<(const bool b) const noexcept;

		/**
		 * @brief TODO: document
		 * 
		 * @tparam T 
		 * @param vec 
		 * @return const dki_stream& 
		 */
		template<typename T>
		const dki_stream& operator<<(const std::vector<T>& vec) const noexcept;

		/**
		 * @brief TODO: document
		 * 
		 * @param str 
		 * @return const dki_stream& 
		 */
		const dki_stream& operator>>(std::string& str) const noexcept;

		/**
		 * @brief TODO: document
		 * 
		 * @param b 
		 * @return const dki_stream& 
		 */
		const dki_stream& operator>>(bool& b) const noexcept;
	};
}