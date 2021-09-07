
#include "consoleplayer.h"
#include "randomplayer.h"
#include "consoleui.h"

#include "interfaces/websocket/wsobservermanager.h"
#include "interfaces/websocket/wsplayer.h"

#include "interfaces/websocket/wsmanager.h"

#include <doppelkopf/doppelkopf.h>

int main(int argc, char* argv[]) {
	dki::WSManager wsmanager(1235);
	dki::WSPlayer player1 = wsmanager.newPlayer("^/player/player0/?$");
	wsmanager.start();

	std::cout << "waiting for the players to connect" << std::endl;
	player1.waitForConnection();

	RandomPlayer player2, player3, player4;
	dk::Doppelkopf instance(player1, player2, player3, player4);

	dki::WSObserverMgr wsobservers(1234, instance);
	instance.runGame();
	wsmanager.stop();

	return 0;
}