#pragma once
#include <cstdlib>
#include <ctime>

class Dice
{
public:
	Dice();
	Dice(int numRolls, int numSides);
	Dice(int numRolls, int numSides, int rollModifier);
	~Dice();
	int roll();

private:
	int rolls;
	int sides;
	int modifier;
};