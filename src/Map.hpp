class Tile
{
public:
	bool passable;
	bool translucent;
	int height;
	const char* type;
	TCODColor colour;
	Tile(bool passable,bool translucent,int height,TCODColor colour,const char* type);
	void render();
	bool canWalk();
	bool isSeeThrough();
	bool isWall();
	 //not only actors cus of lights
};

class Wall : public Tile
{
public:
	Wall();
};

class Ground : public Tile
{
public:
	Ground();
};


class Map
{
public:
	int origini;
	int originj;
	int width;
	int height;
	//int area;
	int nObjects;
	int nais;
	Tile *tiles[50][50];
	Physical *objects[50];
	Physical *selector;
	Actor *players[1];
	Ai *ais[10];
	//
	Map(int width, int height);
	void moveMap();
	void placePlayer(Actor *player);
	void placeAI(Ai *ai);
	void placeObject(Physical *object);
	void render();
	void removeObject(Physical *object);
	void setWall(int i, int j);
	bool isSeeThrough(int i, int j);
	bool canWalk(int i, int j);
	bool playerIsNearEdge();
	bool isInFOV(Actor *object, int i, int j);
	bool isOffMap(int i, int j);
	bool tooFarAway(double distance);
};

