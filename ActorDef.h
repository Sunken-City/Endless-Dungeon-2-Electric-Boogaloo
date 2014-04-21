#pragma once
#include <string>
#include "Dice.h"
#include "Serializeable.h"

using namespace std;

class ActorDef : public Serializeable
{
public:
	void serialize(Serializer write);
	static ActorDef* reconstruct(Serializer read);

	ActorDef();
	ActorDef(int difficulty, int bias);
	ActorDef(int tile, string monsterName, Dice hitDice, Dice attDice, Dice defDice);
	ActorDef(int tile, string monsterName, int maxHP, Dice attDice, Dice defDice);
	~ActorDef();
	int Tile();
	int HP();
	void HP(int increment);
	int ATT();
	int DEF();
	string Name();

private:
	int tileNum;
	int baseHP;
	Dice hitDice;
	Dice attack;
	Dice defence;
	string name;
};