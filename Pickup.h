#pragma once
#include "Coord.h"
#include "Console.h"
#include "PickupDef.h"
#include <sstream>
#include <functional>

class Cell;

using namespace std;

static enum action {BUY = 0, SELL = 1};

class Pickup : public Serializeable
{
public:
	void serialize(Serializer write);
	static Pickup* reconstruct(Serializer read);

	function<void(Player *, Pickup *)> use;
	bool equipped;
	int inventoryPosition;
	Cell* currCell;

	Pickup();
	Pickup(PickupDef pickupType, cint position = cint(0,0), Cell* currentCell = 0);
	~Pickup();

	cint Pos();
	void Pos(cint newPosition);
	int Tile();
	PickupDef Type();
	string Name();
	Cell* getCell();
	void pickedUp();
	bool degrade();
	int Durability();
	int ItemType();
	void describe();
	void describe(action);
	int Price();
	int Price(action);

protected:
	int tileNum;
	Coord<int> position;
	PickupDef type;
	string name;
	int durability;

};