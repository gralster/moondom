class Engine
{
public:
	int width;
	int height;
	//TCOD_key_t lastKey;
	int nMaps;
	int currentArea;
	int currentPlayer;
	const char *gameMode;
	int turn;
	Gui *gui;
	Map *areas[9];
	Actor *players[1];
	Physical *selector;
	Ai *selected;
	//Actor *ais[10]
	//
	Engine(int width, int height);
	void addMap(Map *area);
	void moveArea();
	void getNormalInput();
	void getLookInput();
	void getMenuInput();
	void look();
	//void listen4Esc(const char* input);
	void convo(Ai *selected);
	void interactAi(Ai *ai);
	char const* getStringInput();
	void backToNormal();
	void describe(Physical *object);
	void select(int i, int j);
	void aiturn();
	int getIntInput();
	void transferObject(Map *area, Actor *actor);
	void transferObject(Actor *actor, Map *area);
	void render();
	void menu();
	void renderMenu();
};
