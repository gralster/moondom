#include "main.hpp"
// set colours for game, eventually to have health bar - very rarely appears, dont know why. 

Gui::Gui(int width,int height)
{
	con = new TCODConsole(width,height);
}

void Gui::render(Actor *player) 
{
	con->setDefaultBackground(TCODColor::black);
	con->clear();
	renderBar(0,0,10,"HP",player->hp, player->maxhp, TCODColor::lightRed, TCODColor::darkerRed);
	TCODConsole::blit(con,0,0,width,height,TCODConsole::root,40,0);
}

void Gui::renderBar(int x, int y, int width, const char *name, float value, float maxValue, const TCODColor barColor, const TCODColor backColor)  //not really working?
{
	con->setDefaultBackground(backColor);
	con->rect(x,y,width,1,false,TCOD_BKGND_SET);
	int barWidth = (int)(value / maxValue *width);
	if (barWidth > 0 ) 
	{
		con->setDefaultBackground(barColor);
		con->rect(x,y,barWidth,1,false,TCOD_BKGND_SET);
	}
}
