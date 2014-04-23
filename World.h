#pragma once

#include "Cell.h"
#include "Coord.h"
#include "Settings.h"
#include "Dice.h"
#include "Player.h"
#include "TileSet.h"
#include "Sound.h"
#include "Utilities.h"
#include <cstdlib>
#include <vector>
#include <ctime>
#include <cmath>
#include <algorithm>


using namespace std;

class World : public Serializeable
{
public:
	void serialize(Serializer write);
	void reconstruct(Serializer read);

	//WHY DOES C++ STILL NOT HAVE A FREAKING ROUND FUNCTION????? WHAT IS THIS WORLD COMING TO?????
	int round(double);

	Cell* Shop;
	int currZone;
	vector<ActorDef*> actors;
	vector<PickupDef*> pickups;
	vector<Actor*> monsters;
	vector<TileSet*> styles;
	vector<Pickup*> inventory;

	World();
	~World();
	void actorInit();
	void pickupInit();
	void styleInit();
	void worldGen();
	void resizeMap();
	void initialize();

	void iterateOverWorld(function<void(cint*)> f);

	int getTile(int x, int y);
	Cell* getCell(cint location);
	Cell* getCell(int x, int y);

	int floodFill(Cell* tile);
	cint getStart();
	void setStairs();
	void nextFloor(Player* plr);
	void initShop();

	Cell* findShop();
	void findActors();
	void setPlayer(Player* plr);
	Player* getPlayer();

	void update(Player* plr);
	void updateVisibility(Player* plr);
	void generateVisibility();
	void findLinePath(int radius, int index, cint start, cint end);

	static Cell** getSurrounding(cint start);

private:
	vector<vector<Cell*>> map;
	Cell dummy;
	Player* plr;
	TileSet* style;
	int difficultyLevel;
	static int viewRange;

	vector<cint> points;
	vector<vector<cint>> paths;
};