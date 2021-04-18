#include "main.hpp"
// functions for the background and objectss
//split into individual Tiles (1) , and the map as a whole (2)

//(1)//////////////////////////////////////////////////////////////////////////////////////////////////////////
//constructor
Tile::Tile( bool passable, bool translucent, int height, TCODColor colour, const char* type) : passable(passable), translucent(translucent), height(height),colour(colour),type(type)
{
};

//TCODColor Tile::render()
//{
	//TCODConsole::root->setCharBackground(this->colour);
//};

bool Tile::canWalk() // can the player walk here?
{
	return (this->passable);
	
};

bool Tile::isSeeThrough() // can the player see through this?
{
	//printf("%d",this->translucent);
	return this->translucent;
	//return false;
};

bool Tile::isWall() //is this a wall?
{
	if (strcmp(this->type, "wall") == 0) {return true;}
	else{return false;}
}

Wall::Wall() : Tile(false, false, 10,TCODColor::desaturatedRed,"wall")
{
};

Ground::Ground() : Tile(true, true, 0,TCODColor::desaturatedGreen,"ground")
{
};


//(2)///////////////////////////////////////////////////////////

//constructor
Map::Map(int width, int height): width(width), height(height) // creates initial map, creates  and defines all ais and objects
{
	nObjects = 0;
	nais =0;
	origini = 0;
	originj = 0;
	for (int i = 0; i <width; i++)
	{
		for (int j = 0; j <height; j++)
		{
			this->tiles[i][j] = new Ground();
		}
	}
	Ai *bird = new Ai("bird",'v',TCODColor::red, "small and red", 3,1);
	bird->setPlace(25,25);
	this->placeAI(bird);
	Ai *alien = new Ai("Alien",'!',TCODColor::green,"slimy and generally weird looking", 3,1);
	alien->setPlace(30,30);
	this->placeAI(alien);
	
	Ai *whale = new Ai("whale",'w',TCODColor::blue, "extremely majestic",3,1);
	whale->setPlace(25,25);
	this->placeAI(whale);
	//make map function to generate features:
	Item *pot = new Item("pot",'U',TCODColor::black,"quite rusted",10,2);
	pot->setPlace(20,20);
	this->placeObject(pot);
	Item *lute = new Item("lute",'P',TCODColor::yellow,"probably out of tune",10,2);
	lute->setPlace(30,10);
	this->placeObject(lute);
	this->setWall(19,19);
	this->setWall(19,20);
	this->setWall(5,5);
};


//find tile properties
bool Map::isSeeThrough(int i, int j) 
{
	return this->tiles[i][j]->isSeeThrough();
};
bool Map::canWalk(int i, int j)
{
	return this->tiles[i][j]->canWalk();
}
void Map::setWall(int i, int j)
{
	this->tiles[i][j] = NULL;
	this->tiles[i][j] = new Wall();
}
// check tile properties for viewing
bool Map::isOffMap(int i, int j)
{
	if ((i < 0) || (i > this-> width-1) || (j < 0) || (j > this->height-1)) {return true;}
	else {return false;}
}
bool Map::tooFarAway(double distance)
{
	if (distance > 30) {return true;}
	else {return false;}
}
bool Map::isInFOV(Actor *object, int i, int j)
{
	int di = i-object->i;// -i ;
	int dj = j-object->j;// -j ;
	double distance = sqrt(pow(di,2) + pow(dj,2));
	if  (di == 0 && dj == 0)
	{
		return true;
	}
	else if (tooFarAway(distance))
	{
		return false;
	}
	else if (object->isLookingAt(i,j))
	{
		for (float lambda = 0; lambda<1; lambda+=distance/1000)
		{
			int tilei = floor(object->i +lambda*di+0.5);
			int tilej = floor(object->j +lambda*dj+0.5);
			if (!(this->isSeeThrough(tilei,tilej))&&!((tilei==i)&&(tilej==j)))
			{
				return false;
			}
		}
		return true;
	}
	else{return false;}
	//return true;
}

//object placement
void Map::placePlayer(Actor *player)
{
	this->players[0] = player;
}
void Map::placeAI(Ai *ai)
{
	this->ais[this->nais] = ai;
	this->nais += 1;
}
void Map::placeObject(Physical *object)
{ 
	this->objects[this->nObjects] = object;
	this->nObjects += 1;
	//printf("%s,%s",this->objects[0]->name,this->objects[1]->name);
};
//object unplacement (for when you pick something up)
void Map::removeObject(Physical *object)
{
	for (int item = 0; item < this->nObjects; item++)
	{
		if (object == this->objects[item])
		{
			this->objects[item] = NULL;
			if (item != this->nObjects) 
			{
				for (int index = item+1; index < this->nObjects; index++)
				{
					this->objects[index-1] = this->objects[index];
				}
			}
			this->nObjects -= 1;
		}
	}
};

// initially you could wonder forever - needs to updated to delete old map as it caused a memory problem crash
//no longer in use
bool Map::playerIsNearEdge()
{
	if (((width - players[0]->i) < 5) or ((height - players[0]->j) < 5) or ((players[0]->i) < 5) or ((players[0]->j) < 5))
		{ return true;}
	else {return false;}
}
void Map::moveMap()
{
	//this->origini += 1;
	//this->originj += 1;
	int j;
	int i;
	int facing = this->players[0]->facing;
	if (facing == 0 or facing == 1 or facing ==2 or facing == 7)
	{
	for (j=height; j > 0; j--)
	{
		for (i=width; i >0; i--)
		{
			if (facing == 0)
				{
				if (i == origini or j == originj) 
					{
					this->tiles[i][j] = new Ground();
					}
				else{this->tiles[i][j] = this->tiles[i-1][j-1];}
				}
			if (facing == 1)
				{
				if (j == originj) 
					{
					this->tiles[i][j] = new Ground();
					}
				else{this->tiles[i][j] = this->tiles[i][j-1];}
				}
			if (facing == 2)
				{
				if (j == originj or i == width) 
					{
					this->tiles[i][j] = new Ground();
					}
				else{this->tiles[i][j] = this->tiles[i+1][j-1];}
				}
			
			if (facing == 7)
				{
				if (i == origini) 
					{
					this->tiles[i][j] = new Ground();
					}
				else{this->tiles[i][j] = this->tiles[i-1][j];}
				}
			}
		}
	}
	else
	{
	for (j=0; j <height; j++)
	{
		for (i=0; i < width; i++)
		{
			if (facing == 3)
				{
				if (i == width) 
					{
					this->tiles[i][j] = new Ground();
					}
				else{this->tiles[i][j] = this->tiles[i+1][j];}
				}
			if (facing == 4)
				{
				if (j == height or i == height) 
					{
					this->tiles[i][j] = new Ground();
					}
				else{this->tiles[i][j] = this->tiles[i+1][j+1];}
				}
			if (facing == 5)
				{
				if (j == height) 
					{
					this->tiles[i][j] = new Ground();
					}
				else{this->tiles[i][j] = this->tiles[i][j+1];}
				}
			if (facing == 6)
				{
				if (j == height or i == origini) 
					{
					this->tiles[i][j] = new Ground();
					}
				else{this->tiles[i][j] = this->tiles[i-1][j+1];}
				}
			
		}
	}
	}
	///FIX THIS BIT
	for (int index=0; index<this->nObjects; index++)
	{
		this->objects[index]->i += 1;
		this->objects[index]->j += 1;
	}
	for (int index=0; index<this->nais; index++)
	{
		this->ais[index]->i +=1;
		this->ais[index]->j +=1;
	}
}

//print the map if the player can see it, leave black if not
void Map::render()
{
	TCODConsole::root->clear();
	int j;
	int i;
	for (j=originj; j < originj+height; j++)
	{
		for (i=origini; i < origini+width; i++)
		{

			if (this->isInFOV(players[0],i,j))
			{
			TCODConsole::root->setCharBackground(i,j,this->tiles[i][j]->colour);
			for (int index=0; index<this->nObjects; index++)
			{
				if (i==this->objects[index]->i && j==this->objects[index]->j) {this->objects[index]->render();}
			}
			for (int index=0; index<this->nais; index++)
			{
				if (i==this->ais[index]->i && j==this->ais[index]->j) {this->ais[index]->render();}
			}
			}
		
		}
	}
	this->players[0]->render();
};
