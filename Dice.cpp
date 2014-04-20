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

int Dice::roll()
{
	int total = 0;
	for (int i = 0; i < rolls; i++)
	{
		total += (rand() % sides + 1);
	}
	return total + modifier;
}