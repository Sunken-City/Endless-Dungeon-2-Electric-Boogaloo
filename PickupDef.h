#pragma once
#include "Actor.h"
#include "Dice.h"
#include <string>
#include <functional>
#include <map>

class Pickup;
class Player;

using namespace std;

static enum behavior {GP = -1, HEALTH = 0, WEAPON = 1, SHIELD = 2, ARMOR = 3, SPELL = 4};

class PickupDef : public Serializeable
{
public:
	void serialize(Serializer write);
	void reconstruct(Serializer read);

	typedef map<behavior, function<void(Player*, Pickup*)>> funcMap;
	static funcMap behaviors;
	static funcMap initBehaviors();

	PickupDef();
	PickupDef(int difficulty, behavior itemType);
	PickupDef(int tileNumber, string name, int durability, int value, function<void(Player *, Pickup *)> u, behavior itemType);
	~PickupDef();
	int Tile();
	string Name();
	int Value();
	int Durability();
	int ItemType();
	int castingCost();
	function<void(Player *, Pickup *)> use;

private:
	int tileNum;
	int effectValue;
	int durability;
	string name;
	behavior type;
	int castCost;

};