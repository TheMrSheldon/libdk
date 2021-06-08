#include <doppelkopf/interface/dki_stream.h>

#include <algorithm>
#include <cctype>
#include <vector>

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
		switch (card.suit){
			case dk::Suit::Club: ostream << 'C'; break;
			case dk::Suit::Spade: ostream << 'S'; break;
			case dk::Suit::Heart: ostream << 'H'; break;
			case dk::Suit::Diamond: ostream << 'D'; break;
			default: ostream <<  '-'; break;
		}
		switch(card.value) {
			case dk::Value::Ace: ostream << 'A'; break;
			case dk::Value::King: ostream << 'K'; break;
			case dk::Value::Queen: ostream << 'Q'; break;
			case dk::Value::Jack: ostream << 'J'; break;
			case dk::Value::Ten: ostream << "10"; break;
			case dk::Value::Nine: ostream << '9'; break;
			default: ostream << '-'; break;
		}
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

	template const dki_stream& dki_stream::operator<<(const LogCmd& command) const noexcept;
	template const dki_stream& dki_stream::operator<<(const SetStateCmd& command) const noexcept;
	template const dki_stream& dki_stream::operator<<(const HasReservationsCmd& command) const noexcept;
} // namespace dki