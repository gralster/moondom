#include "main.hpp"
// functions relating to the game engine, player input and actions etc

Engine::Engine(int width, int height) //initialise game
{	
	TCODConsole::initRoot(width,height,"MOONDOM",false);
	nMaps=0;
	turn=0;
	gameMode="normal";
	currentArea = 0;
	Map *start = new Map(width,height);
	addMap(start);
	gui = new Gui(width-10,height);
	Actor *player = new Actor("Grace",'@',TCODColor::white,"you!", 10, 3);
	player->setPlace(25,25);
	start->placePlayer(player);
	players[0]=player;
	
}

void Engine::aiturn() // simulate input for ais each turn // WORK ON THIS NEXT
{
	for (int index = 0; index < this->areas[currentArea]->nais; index++)
	{this->areas[currentArea]->ais[index]->aiturn();}
}

void Engine::addMap(Map *area) // create a new map
{
	this->areas[nMaps]=area;
	this->nMaps += 1;
}

void Engine::moveArea() // eventually let player move to new map
{
	;
}


void Engine::transferObject(Map *area, Actor *actor) //pick up an item 
{
	if (area->nObjects > 0)
	{
		for (int index = 0; index < area->nObjects; index++)
		{
			if (area->objects[index]->i == actor->i && area->objects[index]->j == actor->j)
			{
				actor->pickUp(area->objects[index]);
				printf("You pick up the %s.\n",area->objects[index]->name);
				area->removeObject(area->objects[index]);
			}
			else
			{
				printf("There is nothing here to pick up!\n");
			}
		}
	}
	else
	{
		printf("There is nothing here to pick up!\n");
	}
}

void Engine::transferObject(Actor *actor, Map *area) // drop an item
{
	printf("What do you want to drop?\n");	
	actor->displayInventory();
	int index = this->getIntInput();
	if (actor->checkInventory(index) == 0)
	{
		Physical *object = actor->inventory[index];
		actor->drop(index);
		object->setPlace(actor->i,actor->j);
		area->placeObject(object);
	}
}


void Engine::look() //press l to find something to look at
{
	this->selector = new Physical("selector",'X', TCODColor::white,"");
	selector->setPlace(this->players[0]->i,this->players[0]->j);
	printf("What do you want to look at?\n");
	this->gameMode = "look";
}


void Engine::backToNormal() // leave menu mode
{
	 this->gameMode = "normal";
}

void Engine::describe(Physical *object) // print out the results of looking at something
{
	printf("You see a %s. ",object->name);
	printf("It is %s\n", object->description);
}

void Engine::select(int i, int j) //handle player pressing enter while looking // describe object //interact ai // you cant see
{
	if (this->players[0]->isLookingAt(i,j))
	{
	for (int index=0; index<this->areas[currentArea]->nais; index++)
		{
			if (i==this->areas[currentArea]->ais[index]->i && j==this->areas[currentArea]->ais[index]->j) 
				{this->interactAi(this->areas[currentArea]->ais[index]);break;}
			//else{this->backToNormal();}
		}
	for (int index=0; index<this->areas[currentArea]->nObjects; index++)
		{
			if (i==this->areas[currentArea]->objects[index]->i && j==this->areas[currentArea]->objects[index]->j) 
				{this->describe(this->areas[currentArea]->objects[index]);break;}
			//else{this->backToNormal();}
		}
	//printf("there is nothing to see there.\n");
	//this->backToNormal();
	}
	else{printf("you can't see over there.\n"); this->backToNormal();}
}

void Engine::interactAi(Ai *ai) // move to talking mode
{
	this->describe(ai);
	this->gameMode = "talking";
	this->selected = ai;
}



void Engine::convo(Ai *selected) // AI says hello // needs work
{
	const char *prompt = "";
	printf("%s: ",this->selected->name);
	printf("%s\n",this->selected->saySomething(prompt));
	printf("%s: ",this->players[0]->name);
	const char *input = this->getStringInput();
	if(strcmp(input, "bye") ==0) {this->backToNormal();} // player can leave convo by saying "bye"
}

char const* Engine::getStringInput() // handle player writing words into command line
{
	std::string strin ;
	std::getline(std::cin,strin);
    for (const auto c : strin)
    {
        if (c == 27)
        	{  	this->backToNormal(); return "esc"; }
    }
	const char *str = strin.c_str();
	return str;
}

int Engine::getIntInput() // handle player writing numbers into command line
{
	bool waiting = true;
	TCODConsole::flush();
	TCOD_key_t key;
	while (waiting)
	{
		TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
		if (key.c ==127)
		{
			break;
		}
		else if (key.c == 0 || key.c-48 < 0 || key.c -48 > 9)
		{
			waiting = true;
		}
		else
		{
			waiting = false;
		}
	}
	return key.c-48;
}


void Engine::getNormalInput() // handle walking around key inputs
{
	TCOD_key_t key;
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
	int oldi = players[0]->i;
	int oldj= players[0]->j;
	int moved ;
	switch(key.vk) 
	{
		case TCODK_UP : this->players[0]->moveForward(); moved = 1; break;
		case TCODK_DOWN : this->players[0]->moveBackward(); moved =1 ; break;
		case TCODK_LEFT : this->players[0]->turnl(); break;
		case TCODK_RIGHT : this->players[0]->turnr(); break;
		case TCODK_ESCAPE : this->menu(); break;
		default : break;
	}
	if (!(areas[currentArea]->canWalk(players[0]->i,players[0]->j))) {players[0]->i=oldi;players[0]->j=oldj;moved =0;}
	if (moved ==1 and areas[currentArea]->playerIsNearEdge()) {players[0]->i=oldi;players[0]->j=oldj;this->areas[currentArea]->moveMap();}
	
	
	switch(key.c)
	{
		case 46 : this->transferObject(this->areas[currentArea],this->players[0]); break; //.
		case 105 : this->players[0]->displayInventory(); break; // i
		case 100 : this->transferObject(this->players[0],this->areas[currentArea]); break;	//d
		case 108 : this->look(); break; //l
		default : break;
	}
	//}
};

void Engine::getLookInput() // handle moving look x when player is selecting what to examine
{
	TCOD_key_t key;
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);

	switch(key.vk) 
	{
		case TCODK_UP : this->selector->j -=1; break;
		case TCODK_DOWN : this->selector->j +=1; break;
		case TCODK_LEFT : this->selector->i -=1; break;
		case TCODK_RIGHT : this->selector->i +=1; break;
		case TCODK_ENTER : this->select(this->selector->i,this->selector->j); break;
		case TCODK_ESCAPE : this->backToNormal(); break;
		default : break;
	}
}

void Engine::menu() // print menu output and enter menu mode
{
	printf("Menu:\n");
	printf("Enter		: quit\n");
	printf("Esc		: back to game\n");
	
	this->gameMode = "menu";
}

void Engine::getMenuInput() // handle menu input
{
	TCOD_key_t key;
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);

	switch(key.vk) 
	{
		case TCODK_ENTER : exit(0); break;
		case TCODK_ESCAPE : this->backToNormal(); break;
		default : break;
	}
}


void Engine::render() // update screen output of game
{
	areas[currentArea]->render();
	gui->render(players[0]);
}
