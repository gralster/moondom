#include "main.hpp"

int main()
{
	
	Engine *moondom = new Engine(50,50);
	//Map *map1 = moondom->areas[moondom->currentArea];


	//
	while (!TCODConsole::isWindowClosed() )
	{
		while (strcmp(moondom->gameMode,"normal")==0) 
		{
			moondom->getNormalInput();
			moondom->aiturn();
			moondom->render();
			TCODConsole::flush();
			moondom->turn+=1;
			
		}
		while (strcmp(moondom->gameMode,"look")==0) 
		{
			moondom->getLookInput();
			moondom->render();
			moondom->selector->render();
			TCODConsole::flush();
		}
		while (strcmp(moondom->gameMode,"menu")==0) 
		{
			moondom->getMenuInput();
			//moondom->renderMenu();
			//TCODConsole::flush();
		}
		while (strcmp(moondom->gameMode,"talking")==0) 
		{
			moondom->convo(moondom->selected);
			//moondom->listen4Esc("bleh");
			//moondom->renderMenu();
			//TCODConsole::flush();
		}
	}
	printf("\nDONE\n");

	return 0;
};

