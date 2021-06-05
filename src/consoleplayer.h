#pragma once

#include <doppelkopf/player.h>
#include <doppelkopf/interface/dki_stream.h>

#include <iostream>

class ConsolePlayer final : public dk::Player {
private:
	dki::dki_stream stream;
public:
	ConsolePlayer() : stream(std::cin, std::cout) {
		
	}
	
	bool hasReservations() override {
		bool reservations;
		stream << dki::HasReservationsCmd(std::tuple<>()) >> reservations;
		return reservations;
	}

	void notifyHasReservation(unsigned player) override {
		
	}
	
	void setState(int pos, std::vector<dk::Card> cards) override {
		stream << dki::SetStateCmd(std::tuple<std::vector<dk::Card>>(cards));
	}

	dk::Action getAction() override {
		
	}
};