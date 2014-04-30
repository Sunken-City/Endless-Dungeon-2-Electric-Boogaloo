#pragma once
#include "Serializeable.h"
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;

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
	void levelUp(int playerLevel);
	string print();

private:
	int rolls;
	int sides;
	int modifier;
};