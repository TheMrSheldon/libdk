
#include "consoleplayer.h"
#include "consoleui.h"

#include <doppelkopf/doppelkopf.h>

int main(int argc, char* argv[]) {
	ConsoleUI ui;
	ConsolePlayer player1, player2, player3, player4;
	dk::Doppelkopf instance(player1, player2, player3, player4);
	instance.addObserver(ui);
	instance.runGame();

	return 0;
}