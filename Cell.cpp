#include "Cell.h"
#include "World.h"

World* Cell::world;

Cell::Cell()
{
	this->position = cint();
	this->type = DUMMY;
	this->tileNum = 0x000;
	this->zone = 0;
	this->actor = 0;
	this->pickup = 0;
	this->isOccupied = empty();
	this->visibility = 0x00;
}

Cell::Cell(int x, int y)
{
	this->position = cint(x, y);
	this->type = DUMMY;
	this->tileNum = 0x100;
	this->zone = 0;
	this->actor = 0;
	this->pickup = 0;
	this->isOccupied = empty();
	this->visibility = 0x00;
}

Cell::Cell(tileType typeIn, int tileNumIn, int x, int y)
{
	this->position = cint(x, y);
	this->type = typeIn;
	this->tileNum = tileNumIn;
	this->zone = 0;
	this->actor = 0;
	this->pickup = 0;
	this->isOccupied = empty();
	this->visibility = 0x00;
}

Cell::~Cell()
{
	delete this->pickup;
}


void Cell::serialize(Serializer write)
{
	write.IO<int>(this->tileNum);
	this->position.serialize(write);
	write.IO<tileType>(this->type);
	write.IO<bool>(this->isOccupied);
	if (this->actor == 0)
		write.IO<int>(null);
	else
	{
		if(this->actor->isPlayer)
		{
			write.IO<int>(null);
		}
		else
		{
			write.IO<int>(notNull);
			this->actor->serialize(write);
		}
	}
	if (this->pickup == 0)
		write.IO<int>(null);
	else
	{
		write.IO<int>(notNull);
		this->pickup->serialize(write);
	}
	write.IO<int>(this->visibility);
}

Cell* Cell::reconstruct(Serializer read)
{
	Cell* c = new Cell();
	int isNull;
	read.IO<int>(c->tileNum);
	c->position.reconstruct(read);
	read.IO<tileType>(c->type);
	read.IO<bool>(c->isOccupied);
	read.IO<int>(isNull);
	if (isNull == null)
		c->actor = 0;
	else
	{
		c->actor = Actor::reconstruct(read);
		c->actor->setCell(c);
		c->actor->homeTile = c->position;
	}
	read.IO<int>(isNull);
	if (isNull == null)
		c->pickup = 0;
	else
	{
		c->pickup = Pickup::reconstruct(read);
		c->pickup->currCell = c;
	}
	read.IO<int>(c->visibility);
	return c;
}

cint Cell::Pos()
{
	return this->position;
}

int Cell::getTile()
{
	return this->tileNum;
}

void Cell::setTile(int input)
{
	this->tileNum = input;
}

void Cell::setTile(int tileNumber, tileType tileType)
{
	this->tileNum = tileNumber;
	this->type = tileType;
}

tileType Cell::getType()
{
	return this->type;
}

bool Cell::empty()
{
	if (this->type == WALL || this->type == UNUSED || this->actor != 0)
		return false;
	else
		return true;
}

bool Cell::hasActor()
{
	if (this->actor == 0)
		return false;
	else
		return true;
}

Actor* Cell::getActor()
{
	return this->actor;
}

void Cell::setActor(Actor* entity)
{
	this->actor = entity;
}

void Cell::setActor(Actor entity)
{
	(*(this->actor)) = entity;
}

bool Cell::hasPickup()
{
	if (this->pickup == 0)
		return false;
	else
		return true;
}

Pickup* Cell::getPickup()
{
	return this->pickup;
}

void Cell::setPickup(Pickup* item)
{
	this->pickup = item;
}

void Cell::setPickup(Pickup item)
{
	(*(this->pickup)) = item;
}

Cell* Cell::operator+(cint& rhs)
{
	cint newCoord = this->Pos() + rhs;
	return this->world->getCell(newCoord);
}

void Cell::makeStairs()
{
	this->type = STAIRS;
	this->tileNum = 0x12C;
}

void Cell::makeShop()
{
	this->type = SHOP;
	this->tileNum = 0x178;
}

int Cell::Visibility()
{
	return this->visibility;
}

void Cell::Visibility(int value)
{
	if(this->visibility < value)
		this->visibility = value;
}