#include "ActorDef.h"

ActorDef::ActorDef()
{

}

ActorDef::ActorDef(int difficulty, int bias)
{
	int offset = ((rand() % 50) - 25);
	difficulty += offset;
	this->tileNum = 0x07F + ((rand() % 112) + 1);
	this->name = "Monster";
	this->hitDice = Dice(2, (difficulty / 20));
	this->attack = Dice(2, (difficulty / 20));
	this->defence = Dice(1, 1);
	this->baseHP = hitDice.roll();
}

ActorDef::ActorDef(int tile, string monsterName, Dice hitDice, Dice attDice, Dice defDice)
{
	this->tileNum = tile;
	this->name = monsterName;
	this->hitDice = hitDice;
	this->attack = attDice;
	this->defence = defDice;
	this->baseHP = hitDice.roll();
}
ActorDef::ActorDef(int tile, string monsterName, int maxHP, Dice attDice, Dice defDice)
{
	this->tileNum = tile;
	this->name = monsterName;
	this->attack = attDice;
	this->defence = defDice;
	this->baseHP = maxHP;
}

ActorDef::~ActorDef()
{

}

void ActorDef::serialize(Serializer write)
{

}

void ActorDef::reconstruct(Serializer read)
{

}

int ActorDef::Tile()
{
	return this->tileNum;
}
int ActorDef::HP()
{
	return this->baseHP;
}
void ActorDef::HP(int increment)
{
	this->baseHP += increment;
}
int ActorDef::ATT()
{
	return this->attack.roll();
}
int ActorDef::DEF()
{
	return this->defence.roll();
}
string ActorDef::Name()
{
	return this->name;
}