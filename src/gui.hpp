class Gui
{
public:
	int width;
	int height;
	Gui(int width,int height);
	void render(Actor *actor);
	TCODConsole *con;
	void renderBar(int x, int y, int width, const char *name, float value, float maxValue, const TCODColor barColor, const TCODColor backColor);
};
