#include "main.hpp"

/////////////////////////////////////////////////////////////
//constructor
Physical::Physical(const char* name, int symbol, TCODColor colour, const char* description): name(name), symbol(symbol), colour(colour), description(description)
{
};

void Physical::setPlace(int i, int j)
{
	this->i = i;
	this->j = j;
};

void Physical::render()
{
	TCODConsole::root->setChar(this->i,this->j,this->symbol);
	TCODConsole::root->setCharForeground(i,j,this->colour);
};


/////////////////////////////////////////////////////////////
//constructor
Item::Item(const char* name, int symbol, TCODColor colour,const char* description, int durability, int wear): Physical(name, symbol, colour, description), durability(durability), wear(wear)
{
};

/////////////////////////////////////////////////////////////
//constructor
Weapon::Weapon(const char* name, int symbol, TCODColor colour,const char* description, int durability, int wear, int ap, int range): Item(name, symbol, colour, description, durability, wear), ap(ap), range(range) 
{
};

/////////////////////////////////////////////////////////////
//constructor
Actor::Actor(const char* name, int symbol, TCODColor colour,const char* description, int hp, int ap) : Physical(name, symbol, colour,description), hp(hp), ap(ap)
{
	xp = 0;
	level = 0;
	nCarry = 0;
	facing = 1;
	maxhp = 10;
};


void Actor::pickUp(Physical *object)
{	
	
	if (object->i == this->i && object->j == this->j)
	{
		if (this->nCarry <= this->maxHold) 
		{
			this->inventory[this->nCarry] = object;
			this->nCarry += 1;
		}
		else
		{
			printf("You cannot carry this much stuff!");
		}
	}
	else
	{
		printf("You must be on top of an item to pick it up!");	
	}
};

//drop an item by inventory index
void Actor::drop(int index)
{	
	if (this->nCarry == 0)
	{
		printf("You are not carrying anything!\n");
	}
	else if (index == 127-48)
	{
		printf("You don't drop anything\n");
	}
	else if (index > this->nCarry || index < 0 )
	{
		printf("That is not a valid selection");
	}
	else
	{
		printf("You drop the %s\n",this->inventory[index]->name);
		this->inventory[index] = NULL;
		if (index != this->nCarry) 
		{
			for (int item = index+1; item < this->nCarry; item++)
			{
				this->inventory[item-1] = this->inventory[item];
			}
			this->nCarry -= 1;
		}
	}
}



int Actor::checkInventory(int index)
{
	if (this->nCarry == 0)
	{
		printf("You are not carrying anything!\n");
		return 1;
	}
	else if (index >= this->nCarry || index < 0 )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//drop an item by name
void Actor::drop(const char* object)
{
	for (int item = 0; item < this->nCarry; item++)
	{
		if (object == this->inventory[item]->name)
		{
			this->inventory[item] = NULL;
			if (item != this->nCarry) 
			{
				for (int index = item+1; index < this->nCarry; index++)
				{
					this->inventory[index-1] = this->inventory[index];
				}
			}
			this->nCarry -= 1;
		}
	}
}

//print the contents of an inventory
void Actor::displayInventory()
{
	printf("%s's inventory:\n",this->name);
	if(this->checkInventory(0) ==1)
	{
		;
	}
	else
	{
		for (int item = 0; item < this->nCarry; item++)
		{
			printf("%d: ",item);
			printf("%s\n",this->inventory[item]->name);
		}
	}
};

void Actor::turnr()
{
	this->facing = ( this->facing +1 )%8;
}

void Actor::turnl()
{
	this->facing = (this->facing-1)%8;
	if (this->facing < 0) {this->facing = (8+this->facing)%8;}
}

void Actor::moveForward()
{
	if (this->facing == 0) {this->i-=1; this->j-=1;}
	else if (this->facing == 1) {this->j-=1;}
	else if (this->facing == 2) {this->i+=1; this->j-=1;}
	else if (this->facing == 3) {this->i+=1;}
	else if (this->facing == 4) {this->i+=1; this->j+=1;}
	else if (this->facing == 5) {this->j+=1;}
	else if (this->facing == 6) {this->i-=1; this->j+=1;}
	else if (this->facing == 7) {this->i-=1;}
	else {printf("%i\n",this->facing);}
}

void Actor::moveBackward()
{
	if (this->facing == 0) {this->i+=1; this->j+=1;}
	else if (this->facing == 1) {this->j+=1;}
	else if (this->facing == 2) {this->i-=1; this->j+=1;}
	else if (this->facing == 3) {this->i-=1;}
	else if (this->facing == 4) {this->i-=1; this->j-=1;}
	else if (this->facing == 5) {this->j-=1;}
	else if (this->facing == 6) {this->i+=1; this->j-=1;}
	else if (this->facing == 7) {this->i+=1;}
	else {printf("%i\n",this->facing);}
}

bool Actor::isLookingAt(int i, int j)
{
	for (int l = -22 ; l<22;l+=1)
	{
		if (this->facing == 0 && (this->i-l<i && this->j+l<j)) {return false;}
		else if (this->facing == 2 && (this->i+l>i &&this->j+l<j)) {return false;}
		else if (this->facing == 4 && (this->i+l>i &&this->j-l>j)) {return false;}
		else if (this->facing == 6 && (this->i+l<i &&this->j+l>j)) {return false;}
	}

	if (this->facing == 1 && this->j < j) {return false;}
	else if (this->facing == 3 && this->i > i) {return false;}
	else if (this->facing == 5 && this->j > j) {return false;}
	else if (this->facing == 7 && this->i < i) {return false;}

	else {return true;}
}

Ai::Ai(const char* name, int symbol, TCODColor colour, const char* description, int hp, int ap) : Actor(name, symbol, colour, description, hp, ap)
{;};

char const* Ai::saySomething(const char* prompt)
{
	return "Hello";
}

void Ai::aiturn() //WORK ON THIS NEXT
{
	int split1 = 1;
	int split2 = 2;
	int split3 = 4;
	int split4 = 5;
	int random = rand() % 10;
	if (random < split1)
	{this->moveForward();}
	if (random > split1 and random < split2)
	{this->moveBackward();}
	if (random > split2 and random < split3)
	{this->turnl();}
	if (random > split3 and random < split4)
	{this->turnr();}
	if (random > split4)
	{;}
	
}

