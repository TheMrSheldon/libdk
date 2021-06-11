
#include "consoleplayer.h"
#include "consoleui.h"

#include "wsobserver.h"

#include <doppelkopf/doppelkopf.h>

int main(int argc, char* argv[]) {
	//ConsoleUI ui;
	dki::WSObserver observer(1234);
	ConsolePlayer player1, player2, player3, player4;
	dk::Doppelkopf instance(player1, player2, player3, player4);
	instance.addObserver(observer);
	instance.runGame();

	return 0;
}