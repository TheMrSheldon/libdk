//#pragma once

#include <doppelkopf/interface/dki_stream.h>

#include <vector>
#include <algorithm>
#include <cctype>

namespace aux {
	template<std::size_t...>
	struct seq {};
	
	template<std::size_t N, std::size_t... Is>
	struct gen_seq : gen_seq<N-1, N-1, Is...> {};
	
	template<std::size_t... Is>
	struct gen_seq<0, Is...> : seq<Is...> {};
	
	template<class Tuple, std::size_t... Is>
	static inline void printTuple(const dki::dki_stream& dkstream, std::ostream& ostream, const Tuple& t, aux::seq<Is...>) {
		using bin=int[];
		(void)bin{0, ((void)((ostream << (Is == 0? "" : " ")), (dkstream << std::get<Is>(t))), 0)...};
	}

	template<class Tuple, std::size_t... Is>
	static inline void readTuple(dki::dki_stream& dkstream, Tuple& t, aux::seq<Is...>) {
		using bin=int[];
		(void)bin{0, ((void)(dkstream >> std::get<Is>(t)), 0)...};
	}

	template<dki::CommandID id>
	static std::string getName() noexcept {
		switch (id) {
		case dki::CommandID::Log: return "log";
		case dki::CommandID::SetState: return "setstate";
		case dki::CommandID::HasReservations: return "reservations?";
		default: return "";
		}
	}
}

namespace dki {
	const dki_stream& dki_stream::operator<<(const std::string& str) const noexcept {
		bool containsSpaces = std::any_of(str.begin(), str.end(), [](char c) {
			return std::isspace(static_cast<unsigned char>(c));
		});
		if (containsSpaces)
			ostream << '\"';
		ostream << str;
		if (containsSpaces)
			ostream << '\"';
		return *this;
	}
	const dki_stream& dki_stream::operator<<(const dk::Card& card) const noexcept {
		static const char suits[] = {
			[(int)dk::Suit::Club] = 'C',
			[(int)dk::Suit::Spade] = 'S',
			[(int)dk::Suit::Heart] = 'H',
			[(int)dk::Suit::Diamond] = 'D'
		};
		static const char* const values[] = {
			[(int)dk::Value::Ace] = "A",
			[(int)dk::Value::King] = "K",
			[(int)dk::Value::Queen] = "Q",
			[(int)dk::Value::Jack] = "J",
			[(int)dk::Value::Ten] = "10",
			[(int)dk::Value::Nine] = "9"
		};
		ostream << suits[(int)card.suit] << values[(int)card.value];
		return *this;
	}
	const dki_stream& dki_stream::operator<<(const bool b) const noexcept {
		ostream << b? '1':'0';
		return *this;
	}

	template<typename T>
	const dki_stream& dki_stream::operator<<(const std::vector<T>& vec) const noexcept {
		for (const auto& e : vec)
			*this << e;
		return *this;
	}

	dki_stream::dki_stream(std::istream& istream, std::ostream& ostream) noexcept : istream(istream), ostream(ostream) {}

	template<CommandID cmd, typename... Args>
	const dki_stream& dki_stream::operator<<(const Command<cmd, Args...>& command) const noexcept {
		ostream << aux::getName<cmd>() << ' ';
		aux::printTuple(*this, ostream, command.args, aux::gen_seq<sizeof...(Args)>());
		ostream << std::endl;
		return *this;
	}

	template<CommandID cmd, typename... Args>
	const dki_stream& dki_stream::operator>>(Command<cmd, Args...>& command) const noexcept {
		readTuple(command.args, aux::gen_seq<sizeof...(Args)>());
		return *this;
	}

	const dki_stream& dki_stream::operator>>(std::string& str) const noexcept {
		istream >> std::ws;
		if (istream.peek() == '"') {
			istream.ignore();
			str = "";
			while (istream.peek() != '"' && istream.peek() != EOF)
				str += istream.get();
			istream.ignore();
		} else
			istream >> str;
		return *this;
	}

	const dki_stream& dki_stream::operator>>(bool& b) const noexcept {
		istream >> std::noboolalpha >> b;
		return *this;
	}

	template const dki_stream& dki_stream::operator<<(const LogCmd& command) const noexcept;
	template const dki_stream& dki_stream::operator<<(const SetStateCmd& command) const noexcept;
	template const dki_stream& dki_stream::operator<<(const HasReservationsCmd& command) const noexcept;
}