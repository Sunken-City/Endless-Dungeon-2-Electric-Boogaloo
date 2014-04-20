#pragma once
#include "Actor.h"
#include "Dice.h"
#include "Cell.h"

class Pickup;

class Player : public Actor, public Serializeable
{
public:
	void serialize(Serializer write);
	void reconstruct(Serializer read);

	vector<Pickup*> inventory;
	int gold;

	Player();
	Player(cint startingPosition);
	~Player();
	void die();
	void update();
	void printInventory(cint mousePos);
	void useItem(cint mousePos);
	void sellItem(cint mousePos, vector<Pickup*>* shopInventory);
	void equip(Pickup* item);
	void heal(int hp, int mana);
	int ATT();
	int DEF();
	int getDefModifier(bool checkForDegrade);
	int getAttModifier(bool checkForDegrade);
	void clearCell();
	void nextFloor(cint pos, Cell* newCell);
	void hollaHollaGetDolla(int dolla);
	bool purchase(int price);
	bool cast(int cost);
	static void gainExperience(int exp);

private:
	Pickup* sword;
	Pickup* shield;
	Pickup* armor;
	int mana;
	int maxMana;
	int level;
	static int experience;
	void breakItem(Pickup*); //For consumables
	void breakItem(Pickup*, Pickup**); //For equipment
};

