#pragma once
#include "Actor.h"
#include "Dice.h"
#include "Cell.h"

class Pickup;
class World;

class Player : public Actor
{
public:
	void serialize(Serializer write);
	static Player* reconstruct(Serializer read);

	World* world;

	vector<Pickup*> inventory;
	int gold;
	Pickup* sword;
	Pickup* shield;
	Pickup* armor;

	Player();
	Player(cint startingPosition, World* gameWorld);
	~Player();
	void die();
	void update();
	void printInventory(cint mousePos);
	void useItem(cint mousePos);
	void sellItem(cint mousePos, vector<Pickup*>* shopInventory);
	void equip(Pickup* item);
	int ATT();
	int DEF();
	int getDefModifier(bool checkForDegrade);
	int getAttModifier(bool checkForDegrade);
	void clearCell();
	void nextFloor(cint pos, Cell* newCell);
	void hollaHollaGetDolla(int dolla);
	bool purchase(int price);
	static void gainExperience(int exp);
	int expToNext();
	void recalculatePositions();

	void breakItem(Pickup*); //For consumables
	void breakItem(Pickup*, Pickup**); //For equipment

private:
	int level;
	static int experience;
};

