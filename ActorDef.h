#pragma once
#include <string>
#include <map>
#include "Dice.h"
#include "Serializeable.h"

using namespace std;

class ActorDef : public Serializeable
{
public:
	void serialize(Serializer write);
	static ActorDef* reconstruct(Serializer read);

	typedef map<int, int> tierMap;
	typedef map<int, string> nameMap;

	static tierMap tier1;
	static tierMap tier2;
	static tierMap tier3;
	static tierMap tier4;
	static nameMap monsterNames;

	static tierMap initMonsterTier(int tierNumber);
	static nameMap initMonsterNames();

	Dice attackDice;
	Dice defenceDice;

	ActorDef();
	ActorDef(int difficulty, int bias);
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
	string name;
};