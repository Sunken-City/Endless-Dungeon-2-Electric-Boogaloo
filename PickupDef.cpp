#include "PickupDef.h"
#include "Pickup.h"
#include "Player.h"


PickupDef::funcMap PickupDef::behaviors = initBehaviors();

PickupDef::funcMap PickupDef::initBehaviors()
{
	funcMap newMap;
	newMap[GP] = [&](Player* plr, Pickup* item){};
	newMap[HEALTH] = [&](Player* plr, Pickup* item){plr->heal(item->Type().Value(), item->Type().Value()/10);};
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
		this->effectValue = (difficulty / 20) + 19;
		this->name = "Potion";
	}
	else if (itemType == WEAPON)
	{
		this->tileNum = 0x260 + min((difficulty / 100), 0xD);
		this->durability = 40 - offset;
		this->effectValue = (difficulty / 20);
		this->name = itemNames[this->tileNum];
	}
	else if (itemType == SHIELD)
	{
		this->tileNum = 0x2C0 + min((difficulty / 100), 0xD);
		this->durability = 40 - offset;
		this->effectValue = (difficulty / 40) + 1;
		this->name = itemNames[this->tileNum];
	}
	else if (itemType == ARMOR)
	{
		this->tileNum = 0x2D0 + min((difficulty / 200), 0x6);
		this->durability = 40 - offset;
		this->effectValue = (difficulty / 30) + 1;
		this->name = itemNames[this->tileNum];
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
	write.IO<string>(this->name);
	write.IO<behavior>(this->type);
	write.IO<int>(this->castCost);
}

PickupDef* PickupDef::reconstruct(Serializer read)
{
	PickupDef* p = new PickupDef();
	read.IO<int>(p->tileNum);
	read.IO<int>(p->effectValue);
	read.IO<int>(p->durability);
	read.IO<string>(p->name);
	read.IO<behavior>(p->type);
	read.IO<int>(p->castCost);
	p->use = behaviors[p->type];
	return p;
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

PickupDef::nameMap PickupDef::itemNames = initItemNames();

PickupDef::nameMap PickupDef::initItemNames()
{
	nameMap newMap;
	newMap[0x260] = "Dinner Knife";
	newMap[0x261] = "Dagger";
	newMap[0x262] = "Curved Dagger";
	newMap[0x263] = "Survival Knife";
	newMap[0x264] = "Tanto";
	newMap[0x265] = "Steel Dagger";
	newMap[0x266] = "Dark Dagger";
	newMap[0x267] = "Katar";
	newMap[0x268] = "Long Katar";
	newMap[0x269] = "Triple Katar";
	newMap[0x26A] = "Menacing Katar";
	newMap[0x26B] = "Steel Katar";
	newMap[0x26C] = "Dark Katar";
	newMap[0x26D] = "Final Katar";
	
	newMap[0x2C0] = "Wood Shield";
	newMap[0x2C1] = "Studded Shield";
	newMap[0x2C2] = "Sturdy Shield";
	newMap[0x2C3] = "Tower Shield";
	newMap[0x2C4] = "Skull Shield";
	newMap[0x2C5] = "Bone Shield";
	newMap[0x2C6] = "Dark Shield";
	newMap[0x2C7] = "Iron Shield";
	newMap[0x2C8] = "Obsidian Shield";
	newMap[0x2C9] = "Menacing Shield";
	newMap[0x2CA] = "Steel Shield";
	newMap[0x2CB] = "Pie Shield";
	newMap[0x2CC] = "Mirror Shield";
	newMap[0x2CD] = "Holy Shield";

	newMap[0x2D0] = "Shirt";
	newMap[0x2D1] = "Tunic";
	newMap[0x2D2] = "Leather Armor";
	newMap[0x2D3] = "Breastplate";
	newMap[0x2D4] = "Iron Armor";
	newMap[0x2D5] = "Gold Armor";
	newMap[0x2D6] = "Demon Armor";
	return newMap;
}