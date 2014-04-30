#include "Dice.h"

Dice::Dice()
{
	//srand(time(0));
	rolls = 0;
	sides = 0;
}

Dice::Dice(int numRolls, int numSides)
{
	//srand(time(0));
	rolls = numRolls;
	sides = numSides;
	modifier = 0;
}

Dice::Dice(int numRolls, int numSides, int rollModifier)
{
	//srand(time(0));
	rolls = numRolls;
	sides = numSides;
	modifier = rollModifier;
}

Dice::~Dice()
{

}

void Dice::serialize(Serializer write)
{
	write.IO<int>(this->rolls);
	write.IO<int>(this->sides);
	write.IO<int>(this->modifier);
}

void Dice::reconstruct(Serializer read)
{
	read.IO<int>(this->rolls);
	read.IO<int>(this->sides);
	read.IO<int>(this->modifier);
}

int Dice::roll()
{
	int total = 0;
	for (int i = 0; i < rolls; i++)
	{
		total += (rand() % sides + 1);
	}
	return total + modifier;
}

string Dice::print()
{
	ostringstream str;
	str << this->rolls << "d" << this->sides;	
	return str.str();
}

void Dice::levelUp(int playerLevel)
{
	this->sides += playerLevel;
}