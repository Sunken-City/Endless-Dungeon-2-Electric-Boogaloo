#include "PickupDef.h"
#include "Pickup.h"
#include "Player.h"


PickupDef::PickupDef()
{

}


PickupDef::PickupDef(int difficulty, function<void(Player*, Pickup*)> u, behavior itemType)
{
	int offset = ((rand() % 50) - 25);
	difficulty += offset;
	this->use = u;
	this->type = itemType;

	if (itemType == GP)
	{
		this->tileNum = 0x2FA;
		this->effectValue = ((rand() % (difficulty)) + (difficulty / 10));
		this->name = "GP";
		this->durability = 1;
	}
	else if (itemType == HEALTH)
	{
		this->tileNum = 0x22D - min((difficulty / 100), 0xD);
		this->durability = 1;
		this->effectValue = min((difficulty / 30) + 9, 75);
		this->name = "Potion";
	}
	else if (itemType == WEAPON)
	{
		this->tileNum = 0x260 + min((difficulty / 100), 0xD);
		this->durability = 30 - offset;
		this->effectValue = (difficulty / 20);
		this->name = "Dagger";
	}
	else if (itemType == SHIELD)
	{
		this->tileNum = 0x2C0 + min((difficulty / 100), 0xD);
		this->durability = 30 - offset;
		this->effectValue = (difficulty / 20);
		this->name = "Shield";
	}
	else if (itemType == ARMOR)
	{
		this->tileNum = 0x2D0 + min((difficulty / 200), 0x6);
		this->durability = 30 - offset;
		this->effectValue = (difficulty / 20);
		this->name = "Armor";
	}
	
}

PickupDef::PickupDef(int tileNumber, string name, int durability, int value, function<void(Player*, Pickup*)> u, behavior itemType)
{
	this->tileNum = tileNumber;
	this->name = name;
	this->effectValue = value;
	this->use = u;
	this->durability = durability;
	this->type = itemType;
	this->castCost = 5;
}

PickupDef::~PickupDef()
{

}

void PickupDef::serialize(Serializer write)
{

}

void PickupDef::reconstruct(Serializer read)
{

}


int PickupDef::Tile()
{
	return this->tileNum;
}

string PickupDef::Name()
{
	return this->name;
}

int PickupDef::Value()
{
	return this->effectValue;
}

int PickupDef::Durability()
{
	return this->durability;
}

int PickupDef::ItemType()
{
	return this->type;
}

int PickupDef::castingCost()
{
	return this->castCost;
}