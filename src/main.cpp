#include "main.hpp"
// main gameloop that runs round and round while the player is playing

int main()
{
	
	Engine *moondom = new Engine(50,50); //create game
	//Map *map1 = moondom->areas[moondom->currentArea];


	//
	while (!TCODConsole::isWindowClosed() )
	{
		while (strcmp(moondom->gameMode,"normal")==0) // during normal play the player can 
		{
			moondom->getNormalInput(); // walk and change the game mode
			moondom->aiturn(); //ais move
			moondom->render(); // the game updates
			TCODConsole::flush(); 
			moondom->turn+=1;
			
		}
		while (strcmp(moondom->gameMode,"look")==0)  // press l to go into look mode
		{
			moondom->getLookInput(); // move the looking "x"
			moondom->render();    // update the movement of the x
			moondom->selector->render(); //make sure the x is on top
			TCODConsole::flush();
		}
		while (strcmp(moondom->gameMode,"menu")==0)  // esc for menu mode
		{
			moondom->getMenuInput(); // wait for selection
			//moondom->renderMenu();
			//TCODConsole::flush();
		}
		while (strcmp(moondom->gameMode,"talking")==0)  // interaction mode - this kinda sucks atm
		{
			moondom->convo(moondom->selected); // talk to an AI
			//moondom->listen4Esc("bleh");
			//moondom->renderMenu();
			//TCODConsole::flush();
		}
	}
	printf("\nDONE\n");

	return 0;
};

