class Physical
{
public:
	const char* name;
	const char* description;
	int i;
	int j;
	int symbol;
	TCODColor colour;
	Physical(const char* name, int symbol, TCODColor colour, const char* description);
	void setPlace(int i, int j);
	void render();
};

class Item : public Physical
{
public:
	int durability;
	int wear;
	//
	Item(const char* name, int symbol, TCODColor colour,const char* description, int durability, int wear);

	
};

class Weapon : public Item
{
public:
	int ap;
	int range;
	Weapon(const char* name, int symbol, TCODColor colour,const char* description, int durability, int wear, int ap, int range);
};

class Actor : public Physical
{
public:
	//attributes
	int hp;
	int ap;
	int xp;
	int maxhp;
	int level;
	int facing;
	//Map *currentArea;
	int nCarry;
	int maxHold =10;
	Physical *inventory[10];

	//functions
	Actor(const char* name, int symbol, TCODColor colour, const char* description, int hp, int ap);
	//void pickUp(int i, int j, Map *area);
	void pickUp(Physical *object);
	void displayInventory();
	void drop(int index);
	void drop(const char* objectname);
	void turnl();
	void turnr();
	void moveForward();
	void moveBackward();
	bool isLookingAt(int i, int j);
	int checkInventory(int index);
	//void setPlace(int i, int j);
	//void move(int di, int dj);
};

class Ai : public Actor
{
public:
	//attributes
	int hp;
	int ap;
	int xp;
	int maxhp;
	int level;
	int facing;
	//Map *currentArea;
	int nCarry;
	int maxHold =10;
	Physical *inventory[10];

	//functions
	Ai(const char* name, int symbol, TCODColor colour,const char* description, int hp, int ap);
	char const* saySomething(const char* input);
	void aiturn();
};
