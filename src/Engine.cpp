#include "main.hpp"

Engine::Engine(int width, int height) 
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

void Engine::aiturn() //WORK ON THIS NEXT
{
	for (int index = 0; index < this->areas[currentArea]->nais; index++)
	{this->areas[currentArea]->ais[index]->aiturn();}
}

void Engine::addMap(Map *area)
{
	this->areas[nMaps]=area;
	this->nMaps += 1;
}

void Engine::moveArea()
{
	;
}

//pick up an item passed
void Engine::transferObject(Map *area, Actor *actor)
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

void Engine::transferObject(Actor *actor, Map *area)
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


void Engine::look()
{
	this->selector = new Physical("selector",'X', TCODColor::white,"");
	selector->setPlace(this->players[0]->i,this->players[0]->j);
	printf("What do you want to look at?\n");
	this->gameMode = "look";
}


void Engine::backToNormal()
{
	 this->gameMode = "normal";
}

void Engine::describe(Physical *object)
{
	printf("You see a %s. ",object->name);
	printf("It is %s\n", object->description);
}

void Engine::select(int i, int j)
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

void Engine::interactAi(Ai *ai)
{
	this->describe(ai);
	this->gameMode = "talking";
	this->selected = ai;
}



void Engine::convo(Ai *selected)
{
	const char *prompt = "";
	printf("%s: ",this->selected->name);
	printf("%s\n",this->selected->saySomething(prompt));
	printf("%s: ",this->players[0]->name);
	const char *input = this->getStringInput();
	if(strcmp(input, "bye") ==0) {this->backToNormal();}
}

char const* Engine::getStringInput()
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

int Engine::getIntInput()
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


void Engine::getNormalInput()
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
		case 46 : this->transferObject(this->areas[currentArea],this->players[0]); break;
		case 105 : this->players[0]->displayInventory(); break;
		case 100 : this->transferObject(this->players[0],this->areas[currentArea]); break;	//DROPPING DOES NOT WORK
		case 108 : this->look(); break;
		default : break;
	}
	//}
};

void Engine::getLookInput()
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

void Engine::menu()
{
	printf("Menu:\n");
	printf("Enter		: quit\n");
	printf("Esc		: back to game\n");
	printf("s		: see controls\n");
	
	this->gameMode = "menu";
}

void Engine::getMenuInput()
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

void Engine::renderMenu()
{
	printf("Menu:\n");
	printf("Enter		: quit\n");
	printf("Esc		: back to game\n");
	printf("s		: see controls\n");
}

void Engine::render()
{
	areas[currentArea]->render();
	gui->render(players[0]);
}
