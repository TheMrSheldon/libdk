#pragma once

#include <doppelkopf/player.h>
#include <doppelkopf/interface/dki_stream.h>

#include <future>
#include <iostream>
#include <thread>

class ConsolePlayer final : public dk::Player {
private:
	dki::dki_stream stream;
	std::thread thread;
	bool isRunning = true;
	std::promise<dki::AnyCommand> response;

	void run() {
		//Listening for input
		while (isRunning) {
			dki::AnyCommand command;
			stream >> command;
			if (std::holds_alternative<dki::LogCmd>(command)) {
				std::cout << "logged" << std::endl;
			} else if (std::holds_alternative<dki::PlacementActionCmd>(command)) {
				response.set_value(command);
			} else if (std::holds_alternative<std::monostate>(command)) {
				std::cout << "error" << std::endl;
			}
		}
	}
public:
	ConsolePlayer() : stream(std::cin, std::cout) {
		thread = std::thread(&ConsolePlayer::run, this);
		thread.detach();
	}
	
	bool hasReservations() override {
		stream << dki::HasReservationsCmd(std::tuple<>());
		auto reservations = std::get<dki::ReservationCmd>(response.get_future().get());
		return std::get<0>(reservations.args);
	}

	void notifyHasReservation(unsigned player) override {
		//TODO: implement
	}

	void notifyPlaceCard(unsigned player, dk::Card card) {
		//TODO: implement
	}

	void notifyRoundEnd(int winner, std::array<dk::Card, 4> cards) {
		//TODO: implement
	}
	
	void setState(int pos, std::vector<dk::Card> cards) override {
		stream << dki::SetStateCmd(std::tuple<std::vector<dk::Card>>(cards));
	}

	dk::Action getAction() override {
		response = std::promise<dki::AnyCommand>();
		stream << dki::GetActionCmd(std::tuple<>());
		auto action = response.get_future().get();
		auto placementAction = std::get<dki::PlacementActionCmd>(action);
		//TODO: handle announcement actions
		return dk::Action::NewPlacementAction(std::get<0>(placementAction.args));
	}
};