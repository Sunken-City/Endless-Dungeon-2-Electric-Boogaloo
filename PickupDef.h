#pragma once
#include "Actor.h"
#include "Dice.h"
#include <string>
#include <functional>
#include <map>

class Pickup;
class Player;

using namespace std;

static enum behavior {GP = -1, HEALTH = 0, WEAPON = 1, SHIELD = 2, ARMOR = 3, SCROLL = 4,
	//Spell Behaviors go down here. Not used for generation of weapons, only scroll uses.
	HEAL = 10, HALF_HEAL, FULL_HEAL, FIX_SWORD, BREAK_SWORD, FIX_SHIELD, BREAK_SHIELD, FIX_ARMOR, BREAK_ARMOR, DOWN_FLOOR, END_OF_LIST};

class PickupDef : public Serializeable
{
public:
	void serialize(Serializer write);
	static PickupDef* reconstruct(Serializer read);

	typedef map<behavior, function<void(Player*, Pickup*)>> funcMap;
	typedef map<int, string> nameMap;
	static nameMap itemNames;
	static nameMap initItemNames();
	static funcMap behaviors;
	static funcMap initBehaviors();

	PickupDef();
	PickupDef(int difficulty, behavior itemType);
	PickupDef(int tileNumber, char* name, int durability, int value, function<void(Player *, Pickup *)> u, behavior itemType);
	~PickupDef();
	int Tile();
	string Name();
	int Value();
	int Durability();
	int ItemType();
	function<void(Player *, Pickup *)> use;

private:
	int tileNum;
	int effectValue;
	int durability;
	string name;
	behavior type;
	behavior type2;
	int castCost;

};