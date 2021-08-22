
#include "consoleplayer.h"
#include "randomplayer.h"
#include "consoleui.h"

#include "wsobservermanager.h"

#include <doppelkopf/doppelkopf.h>

int main(int argc, char* argv[]) {
	ConsolePlayer player1;
	RandomPlayer player2, player3, player4;
	dk::Doppelkopf instance(player1, player2, player3, player4);
	dki::WSObserverMgr wsmanager(1234, instance);
	instance.runGame();

	return 0;
}