#pragma once
#include "Coord.h"
#include "Actor.h"
#include "Pickup.h"
#include "Shop.h"

class World;

static enum tileType {DUMMY = -1, WALL = 0, FLOOR = 1, UNUSED = 2, STAIRS = 3, SHOP = 4};

class Cell : public Serializeable
{
public:
	void serialize(Serializer write);
	static Cell* reconstruct(Serializer read);

	static World* world;
	int zone;

	Cell();
	Cell(int x, int y);
	Cell(tileType typeIn, int tileNumIn, int x, int y);
	~Cell();
	cint Pos();
	int getTile();
	void setTile(int);
	void setTile(int, tileType);
	tileType getType();
	bool empty();
	int Visibility();
	void Visibility(int);

	bool hasActor();
	Actor* getActor();
	void setActor(Actor* entity);
	void setActor(Actor entity);

	bool hasPickup();
	Pickup* getPickup();
	void setPickup(Pickup* item);
	void setPickup(Pickup item);

	void makeStairs();
	void makeShop();

	Cell* operator+(cint& rhs);

private:
	int tileNum;
	cint position;
	tileType type;
	bool isOccupied;
	Actor* actor;
	Pickup* pickup;
	int visibility;
};

