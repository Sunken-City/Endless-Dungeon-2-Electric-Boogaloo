#pragma once
#include "Serializeable.h"
#include <cstdlib>
#include <ctime>

class Dice : public Serializeable
{
public:
	void serialize(Serializer write);
	void reconstruct(Serializer read);

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