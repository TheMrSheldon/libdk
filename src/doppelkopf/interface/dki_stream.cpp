#include <doppelkopf/interface/dki_stream.h>

#include <doppelkopf/doppelkopf.h>

#include <algorithm>
#include <cctype>
#include <vector>

using namespace std::literals::string_literals;

//Taken from here: https://stackoverflow.com/a/6245777
namespace aux {
	template<std::size_t...>
	struct seq {};
	
	template<std::size_t N, std::size_t... Is>
	struct gen_seq : gen_seq<N-1, N-1, Is...> {};
	
	template<std::size_t... Is>
	struct gen_seq<0, Is...> : seq<Is...> {};
	
	template<class Tuple, std::size_t... Is>
	static inline void printTuple(const dki::dki_stream& dkstream, std::ostream& ostream, const Tuple& t, aux::seq<Is...>) { // NOLINT(readability-named-parameter)
		using bin=int[]; // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,-warnings-as-errors,modernize-avoid-c-arrays,-warnings-as-errors)
		(void)bin{0, ((void)((ostream << (Is == 0? "" : " ")), (dkstream << std::get<Is>(t))), 0)...};
	}

	template<class Tuple, std::size_t... Is>
	static inline void readTuple(dki::dki_stream& dkstream, Tuple& t, aux::seq<Is...>) { // NOLINT(readability-named-parameter)
		using bin=int[]; // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,-warnings-as-errors,modernize-avoid-c-arrays,-warnings-as-errors)
		(void)bin{0, ((void)(dkstream >> std::get<Is>(t)), 0)...};
	}

	template<dki::CommandID id>
	static std::string getName() noexcept {
		switch (id) {
		case dki::CommandID::Log: return "log";
		case dki::CommandID::SetState: return "setstate";
		case dki::CommandID::HasReservations: return "reservations?";
		case dki::CommandID::GetAction: return "action?";
		default: return "";
		}
	}
} // namespace aux

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
		ostream << dk::to_string(card);
		return *this;
	}
	const dki_stream& dki_stream::operator<<(const bool& b) const noexcept {
		ostream << (b? '1':'0');
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
	const dki_stream& dki_stream::operator>>(Command<cmd, Args...>& command) noexcept {
		aux::readTuple(*this, command.args, aux::gen_seq<sizeof...(Args)>());
		return *this;
	}

	const dki_stream& dki_stream::operator>>(AnyCommand& command) noexcept {
		std::string name;
		*this >> name;
		if ("log"s.compare(name) == 0) {
			LogCmd cmd;
			*this >> cmd;
			command = cmd;
		} else if ("place"s.compare(name) == 0) {
			PlacementActionCmd cmd;
			*this >> cmd;
			command = cmd;
		} else {
			//TODO: throw exception
		}
		return *this;
	}

	const dki_stream& dki_stream::operator>>(std::string& str) const noexcept {
		istream >> std::ws;
		if (istream.peek() == '"') {
			istream.ignore();
			str = "";
			while (istream.peek() != '"' && istream.peek() != EOF)
				str += static_cast<char>(istream.get());
			istream.ignore();
		} else
			istream >> str;
		return *this;
	}

	const dki_stream& dki_stream::operator>>(bool& b) const noexcept {
		istream >> std::noboolalpha >> b;
		return *this;
	}

	const dki_stream& dki_stream::operator>>(dk::Card& c) const noexcept {
		char ch;
		istream >> ch;
		switch (ch) {
		case 'C': c.suit = dk::Suit::Club; break;
		case 'S': c.suit = dk::Suit::Spade; break;
		case 'H': c.suit = dk::Suit::Heart; break;
		case 'D': c.suit = dk::Suit::Diamond; break;
		default: //TODO: throw exception
			break;
		}
		istream >> ch;
		switch (ch) {
		case 'K': c.value = dk::Value::King; break;
		case 'Q': c.value = dk::Value::Queen; break;
		case 'J': c.value = dk::Value::Jack; break;
		case '1':
			char tmp;
			istream >> tmp;
			//assert(tmp == '0');
			c.value = dk::Value::Ten; break;
		case '9': c.value = dk::Value::Nine; break;
		default: //TODO: throw exception
			break;
		}
		return *this;
	}

	template const dki_stream& dki_stream::operator<<(const LogCmd& command) const noexcept;
	template const dki_stream& dki_stream::operator<<(const SetStateCmd& command) const noexcept;
	template const dki_stream& dki_stream::operator<<(const HasReservationsCmd& command) const noexcept;
	template const dki_stream& dki_stream::operator<<(const GetActionCmd& command) const noexcept;
} // namespace dki