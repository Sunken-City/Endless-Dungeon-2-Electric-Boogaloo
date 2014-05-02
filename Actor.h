#pragma once
#include "Coord.h"
#include "ActorDef.h"
#include "Console.h"
#include "Sound.h"
#include "Utilities.h"
#include <string>
#include <sstream>

class Cell;

using namespace std;

static enum status {NORMAL = 0, PARALYZED = 1};

class Actor : public Serializeable
{
public:
	void serialize(Serializer write);
	static Actor* reconstruct(Serializer read);

	bool active;
	bool mute;
	bool isPlayer;
	cint homeTile;
	status Status;

	Actor();
	Actor(ActorDef* actorType, Coord<int> position, Cell* currentCell);
	~Actor();
	int HP();
	void HP(int);
	void heal(int);
	void takesDamage(int);
	cint Pos();
	void Pos(cint newPosition);
	int Tile();
	ActorDef* Type();
	virtual int ATT();
	virtual int DEF();
	string Name();
	bool move(cint newPosition, Cell* newCell);
	Cell* getCell();
	void setCell(Cell* newCell);
	virtual void die();
	virtual void update();
	void AI(Actor* player);
	void describe();

	int getColor();



protected:
	int tileNum;
	cint position;
	int hP;
	ActorDef* type;
	Cell* currCell;

	int hurtCounter;
	int stamina;
	int maxStamina;
	
};