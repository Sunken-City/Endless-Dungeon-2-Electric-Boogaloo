#include "PickupDef.h"
#include "Pickup.h"
#include "Player.h"


PickupDef::funcMap PickupDef::behaviors = initBehaviors();

PickupDef::funcMap PickupDef::initBehaviors()
{
	funcMap newMap;
	newMap[GP] = [&](Player* plr, Pickup* item){};
	newMap[HEALTH] = [&](Player* plr, Pickup* item){plr->heal(item->Type().Value(), item->Type().Value()/10); Sound::play("grandHeal.sfs");};
	newMap[WEAPON] = [&](Player* plr, Pickup* item){plr->equip(item);};
	newMap[SHIELD] = newMap[WEAPON];
	newMap[ARMOR] = newMap[WEAPON];
	newMap[SPELL] = [&](Player* plr, Pickup* item){plr->heal(item->Type().Value(), 0); Sound::play("grandHeal.sfs");};
	return newMap;
}

PickupDef::PickupDef()
{

}

PickupDef::PickupDef(int difficulty, behavior itemType)
{
	int offset = ((rand() % 50) - 25);
	difficulty += offset;
	this->use = behaviors[itemType];
	this->type = itemType;
	this->castCost = 5;

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
	else if (itemType == SPELL)
	{
		this->tileNum = 0x367 + min((difficulty / 200), 0x6);
		this->durability = 1;
		this->effectValue = (difficulty / 20);
		this->name = "Healing Scroll";
	}
	
}

PickupDef::PickupDef(int tileNumber, char* name, int durability, int value, function<void(Player*, Pickup*)> u, behavior itemType)
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
	write.IO<int>(this->tileNum);
	write.IO<int>(this->effectValue);
	write.IO<int>(this->durability);
	write.IO<char*>(this->name);
	write.IO<behavior>(this->type);
	write.IO<int>(this->castCost);
}

PickupDef PickupDef::reconstruct(Serializer read)
{
	PickupDef p = PickupDef();
	read.IO<int>(p.tileNum);
	read.IO<int>(p.effectValue);
	read.IO<int>(p.durability);
	read.IO<char*>(p.name);
	read.IO<behavior>(p.type);
	read.IO<int>(p.castCost);
	return p;
}


int PickupDef::Tile()
{
	return this->tileNum;
}

char* PickupDef::Name()
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