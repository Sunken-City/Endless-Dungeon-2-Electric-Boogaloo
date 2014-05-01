#include "ActorDef.h"

ActorDef::ActorDef()
{

}

ActorDef::ActorDef(int difficulty, int bias)
{
	int offset = ((rand() % 50) - 25);
	difficulty += offset;
	if (difficulty < 150)
		this->tileNum = tier1[(rand() % 9)];
	else if (difficulty < 350)
		this->tileNum = tier2[(rand() % 9)];
	else if (difficulty < 550)
		this->tileNum = tier3[(rand() % 9)];
	else if (difficulty >= 550)
		this->tileNum = tier4[(rand() % 9)];
	this->name = monsterNames[this->tileNum];
	this->hitDice = Dice(2, (difficulty / 10), 5);
	this->attackDice = Dice(1, (difficulty / 10), (difficulty / 100) + 2);
	this->defenceDice = Dice(1, 1);
	this->baseHP = hitDice.roll();
}

ActorDef::ActorDef(int tile, string monsterName, int maxHP, Dice attDice, Dice defDice)
{
	this->tileNum = tile;
	this->name = monsterName;
	this->attackDice = attDice;
	this->defenceDice = defDice;
	this->baseHP = maxHP;
}

ActorDef::~ActorDef()
{

}

void ActorDef::serialize(Serializer write)
{
	write.IO<int>(this->tileNum);
	write.IO<int>(this->baseHP);
	this->hitDice.serialize(write);
	this->attackDice.serialize(write);
	this->defenceDice.serialize(write);
	write.IO<string>(this->name);
}

ActorDef* ActorDef::reconstruct(Serializer read)
{
	ActorDef* a = new ActorDef();
	read.IO<int>(a->tileNum);
	read.IO<int>(a->baseHP);
	a->hitDice.reconstruct(read);
	a->attackDice.reconstruct(read);
	a->defenceDice.reconstruct(read);
	read.IO<string>(a->name);
	return a;
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
	return this->attackDice.roll();
}
int ActorDef::DEF()
{
	return this->defenceDice.roll();
}
string ActorDef::Name()
{
	return this->name;
}


ActorDef::tierMap ActorDef::tier1 = initMonsterTier(1);
ActorDef::tierMap ActorDef::tier2 = initMonsterTier(2);
ActorDef::tierMap ActorDef::tier3 = initMonsterTier(3);
ActorDef::tierMap ActorDef::tier4 = initMonsterTier(4);

ActorDef::tierMap ActorDef::initMonsterTier(int tierNumber)
{
	tierMap newMap;
	if (tierNumber == 1)
	{
		newMap[0] = 0x90; //Dog
		newMap[1] = 0x87; //Blue Slime -> White Beast
		newMap[2] = 0xD9; //Rat
		newMap[3] = 0x86; //Slime
		newMap[4] = 0xBC; //Kobolds
		newMap[5] = 0xC9; //Goblins
		newMap[6] = 0x80; //Ants -> Beetle
		newMap[7] = 0xDF; //Spiders -> Bees
		newMap[8] = 0X9F; //Fire -> Hellhound
	}
	else if (tierNumber == 2)
	{
		newMap[0] = 0x91; //Dog
		newMap[1] = 0xC1; //Blue Slime -> White Beast
		newMap[2] = 0xDA; //Rat
		newMap[3] = 0x88; //Slime
		newMap[4] = 0xBD; //Kobolds
		newMap[5] = 0xCA; //Goblins
		newMap[6] = 0x82; //Ants -> Beetle
		newMap[7] = 0xE0; //Spiders -> Bees
		newMap[8] = 0X99; //Fire -> Hellhound
	}
	else if (tierNumber == 3)
	{
		newMap[0] = 0x92; //Dog
		newMap[1] = 0xC2; //Blue Slime -> White Beast
		newMap[2] = 0xDB; //Rat
		newMap[3] = 0xBA; //Slime
		newMap[4] = 0xBE; //Kobolds
		newMap[5] = 0xCB; //Goblins
		newMap[6] = 0x83; //Ants -> Beetle
		newMap[7] = 0x81; //Spiders -> Bees
		newMap[8] = 0X9A; //Fire -> Hellhound
	}
	else if (tierNumber == 4)
	{
		newMap[0] = 0x96; //Dog
		newMap[1] = 0xC3; //Blue Slime -> White Beast
		newMap[2] = 0xDC; //Rat
		newMap[3] = 0xEC; //Slime
		newMap[4] = 0xBF; //Kobolds
		newMap[5] = 0xCE; //Goblins
		newMap[6] = 0x84; //Ants -> Beetle
		newMap[7] = 0x85; //Spiders -> Bees
		newMap[8] = 0X9B; //Fire -> Hellhound
	}
	return newMap;
}


ActorDef::nameMap ActorDef::monsterNames = initMonsterNames();
ActorDef::nameMap ActorDef::initMonsterNames()
{
	nameMap newMap;
	newMap[0x90] = "Puppy"; //Dog
	newMap[0x87] = "Blue Slime"; //Blue Slime -> White Beast
	newMap[0xD9] = "Rat"; //Rat
	newMap[0x86] = "Green Slime"; //Slime
	newMap[0xBC] = "Young Kobold"; //Kobolds
	newMap[0xC9] = "Goblin Scout"; //Goblins
	newMap[0x80] = "Worker Ant"; //Ants -> Beetle
	newMap[0xDF] = "Giant Spider"; //Spiders -> Bees
	newMap[0X9F] = "Fire Elemental"; //Fire -> Hellhound
	
	newMap[0x91] = "Dog"; //Dog
	newMap[0xC1] = "Grep"; //Blue Slime -> White Beast
	newMap[0xDA] = "Giant Rat"; //Rat
	newMap[0x88] = "Jelly"; //Slime
	newMap[0xBD] = "Kobold"; //Kobolds
	newMap[0xCA] = "Goblin"; //Goblins
	newMap[0x82] = "Warrior Ant"; //Ants -> Beetle
	newMap[0xE0] = "Tarantula"; //Spiders -> Bees
	newMap[0X99] = "Hellpup"; //Fire -> Hellhound
	
	newMap[0x92] = "Large Dog"; //Dog
	newMap[0xC2] = "Unarj"; //Blue Slime -> White Beast
	newMap[0xDB] = "Fury Rat"; //Rat
	newMap[0xBA] = "Dark Slime"; //Slime
	newMap[0xBE] = "Kobold Warrior"; //Kobolds
	newMap[0xCB] = "Goblin Warrior"; //Goblins
	newMap[0x83] = "Fire Ant"; //Ants -> Beetle
	newMap[0x81] = "Bee"; //Spiders -> Bees
	newMap[0X9A] = "Hellhound"; //Fire -> Hellhound
	
	newMap[0x96] = "Wolf"; //Dog
	newMap[0xC3] = "Chmod"; //Blue Slime -> White Beast
	newMap[0xDC] = "Sentient Rat"; //Rat
	newMap[0xEC] = "Dark Jelly"; //Slime
	newMap[0xBF] = "Kobold General"; //Kobolds
	newMap[0xCE] = "Goblin General"; //Goblins
	newMap[0x84] = "Stag Beetle"; //Ants -> Beetle
	newMap[0x85] = "Queen Bee"; //Spiders -> Bees
	newMap[0X9B] = "Cerberus"; //Fire -> Hellhound
	
	return newMap;
}
