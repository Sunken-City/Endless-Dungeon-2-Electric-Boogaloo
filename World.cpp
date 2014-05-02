#include "World.h"

int World::viewRange = 5;

World::World()
{
    canChangeFloor = false;
	generateVisibility();
	difficultyLevel = 50;
	dummy = Cell();
	styleInit();
	Cell::world = this;
}

void World::initialize()
{
	actorInit();
	pickupInit();
	worldGen();
	initShop();
}

World::~World()
{
	for (int i = 0; i < Settings::worldx; i++)
	{
		for (int j = 0; j < Settings::worldy; j++)
		{
			delete map[i][j];
		}
	}
}


void World::serialize(Serializer write)
{
	write.IO<int>(this->difficultyLevel);
	write.IO<int>(this->viewRange);
	write.IO<TileSet>(*(this->style));
	
	iterateOverWorld([&](cint* pos){getCell(*pos)->serialize(write);});

	for (vector<vector<cint>>::iterator list = paths.begin(); list != paths.end(); list++)
	{
		for (vector<cint>::iterator itr = (*list).begin(); itr != (*list).end(); itr++)
		{
			itr->serialize(write);
		}
	}
	Utilities::serializeInventory(&inventory, write);
	this->plr->serialize(write);
}

void World::reconstruct(Serializer read)
{
	dummy = Cell();
	read.IO<int>(this->difficultyLevel);
	read.IO<int>(this->viewRange);
	read.IO<TileSet>(*(this->style));
	resizeMap();

	for (int i = 0; i < Settings::worldx; i++)
	{
		for (int j = 0; j < Settings::worldy; j++)
		{
			map[i][j] = Cell::reconstruct(read);
		}
	}

	for (vector<vector<cint>>::iterator list = paths.begin(); list != paths.end(); list++)
	{
		for (vector<cint>::iterator itr = (*list).begin(); itr != (*list).end(); itr++)
		{
			itr->reconstruct(read);
		}
	}
	int inventorySize;
	read.IO<int>(inventorySize);
	for (int i = 0; i < inventorySize; i++)
	{
		this->inventory.push_back(Pickup::reconstruct(read));
	}
	this->plr = Player::reconstruct(read);
	cint pos = this->plr->Pos();
	map[pos.X()][pos.Y()]->setActor(this->plr);
	this->plr->setCell(map[pos.X()][pos.Y()]);
	this->plr->world = this;

	findActors();
	this->Shop = findShop();	
}

void World::iterateOverWorld(function<void(cint*)> f)
{
	cint currPos;
	for (int i = 0; i < Settings::worldx; i++)
	{
		currPos.X(i);
		for (int j = 0; j < Settings::worldy; j++)
		{
			currPos.Y(j);
			f(&currPos);
		}
	}
}

cint World::iterateOverWorldForPosition(function<void(cint**, cint*)> f)
{
	cint currPos;
	cint* returnVal = 0;
	for (int i = 0; i < Settings::worldx; i++)
	{
		currPos.X(i);
		for (int j = 0; j < Settings::worldy; j++)
		{
			currPos.Y(j);
			f(&returnVal, &currPos);
			if(returnVal != 0)
				return *(returnVal);
		}
	}
}

void World::actorInit()
{
	srand(time(0));
	for(int i = 0; i < 10; i++)
	{
		actors.push_back(new ActorDef(difficultyLevel, 0));
	}
}

void World::pickupInit()
{
	srand(time(0));
	for (int i = 0; i < 5; i++)
	{
		pickups.push_back(new PickupDef(difficultyLevel, GP));
		pickups.push_back(new PickupDef(difficultyLevel, GP));
		pickups.push_back(new PickupDef(difficultyLevel, HEALTH));
		pickups.push_back(new PickupDef(difficultyLevel, WEAPON));
		pickups.push_back(new PickupDef(difficultyLevel, SHIELD));
		pickups.push_back(new PickupDef(difficultyLevel, ARMOR));
		pickups.push_back(new PickupDef(difficultyLevel, SCROLL));
	}
}

void World::styleInit()
{
	srand(time(0));
	//Floor, alt, Wall
	//Dungeon
	styles.push_back(new TileSet(0x128, 0x138, 0x129));
	//Grassland
	styles.push_back(new TileSet(0x100, 0x104, 0x113));
	//Regal
	styles.push_back(new TileSet(0x12B, 0x13B, 0x13C));
	//Cavern
	styles.push_back(new TileSet(0x11C, 0x10E, 0x10B));
	//House
	styles.push_back(new TileSet(0x16A, 0x16A, 0x120));
	//Crust
	styles.push_back(new TileSet(0x12E, 0x10F, 0x12D));
	//Ruins
	styles.push_back(new TileSet(0x13E, 0x13E, 0x14F));
	//Castle
	styles.push_back(new TileSet(0x125, 0x135, 0x13F));
	//Temple
	styles.push_back(new TileSet(0x14E, 0x14E, 0x14D));

	this->style = styles.at(rand() % (styles.size()));
}

void World::resizeMap()
{
	//Start off by resizing to the height of the game world
	map.resize(Settings::worldx); 
	//Now fill the rest of the grid in
	for (int i = 0; i < Settings::worldy; i++)
	{
		map[i].resize(Settings::worldx);
	}
}

void World::worldGen()
{
	resizeMap();
	//Initialize the world
	for (int i = 0; i < Settings::worldx; i++)
	{
		for (int j = 0; j < Settings::worldy; j++)
		{
			map[i][j] = new Cell(WALL,style->wall, i, j);
		}
	}

	//Seed the generator
	srand(time(0));

	//Generate the rooms and set neighbors
	int xsize, ysize, xpos, ypos, randomTile;
	for (int k = 0; k < 200; k++)
	{
		xsize = rand() % 6 + 1;
		ysize = rand() % 6 + 1;
		xpos = rand() % 52 + 6;
		ypos = rand() % 52 + 6;

		for (int i = xpos; i < xpos + xsize; i++)
		{
			for (int j = ypos; j < ypos + ysize; j++)
			{
				randomTile = rand() % 100 + 1;
				if (randomTile > 99)
					map[i][j]->setTile(style->altFloor, FLOOR);
				else
					map[i][j]->setTile(style->floor, FLOOR);
			}
		}
	}

	//Flood Fill all of the zones and find the biggest.
	vector<int> zones;
	int numFilled;
	currZone = 1;
	iterateOverWorld([&](cint* pos){
			Cell* c = getCell(*pos);
			numFilled = floodFill(c);
			if (numFilled != 0)
			{
				zones.push_back(numFilled);
				currZone++;
			}
		});
	//Determine the largest zone of the zones we have.
	int max = 0;
	int biggestZone;
	int iteration = 0;
	for_each (zones.begin(), zones.end(), [&](int &zone){
		if (zone > max)
		{
			max = zone;
			biggestZone = iteration + 1;
		}
		iteration++;
	});

	//Replace all other zones with wall tiles.
	iterateOverWorld([&](cint* pos){
		Cell* c = getCell(*pos);
		if(c->zone != biggestZone)
		{
			cint currPos = c->Pos();
			if (getCell(currPos.Next(0,-1))->zone == biggestZone ||
				getCell(currPos.Next(0,1))->zone == biggestZone ||
				getCell(currPos.Next(-1,0))->zone == biggestZone ||
				getCell(currPos.Next(1,0))->zone == biggestZone)
			{
				c->setTile(style->wall, WALL);
			}
			else
				c->setTile(0x000, UNUSED);
		}
	});

	setStairs();

	//Populate world with monsters
	iterateOverWorld([&](cint* pos){
		Cell* c = getCell(*pos);
		if(c->empty())
		{
			if((rand() % 30 + 1) == 15)
			{
				monsters.push_back(new Actor(actors.at(rand() % (actors.size())), cint(c->Pos()), c));
				c->setActor(monsters.at(monsters.size() - 1));
				if((rand() % 2 + 1) == 1)
					c->setPickup(new Pickup((*pickups.at(rand() % (pickups.size()))), cint(c->Pos()), c));
			}
		}
	});
}

int World::floodFill(Cell* tile)
{
	if((tile->zone == 0) && (tile->getType() == FLOOR))
	{
		tile->zone = currZone;
		cint currPos = tile->Pos();
		return (1 
			+ floodFill(getCell(currPos.Next(0,-1))) 
			+ floodFill(getCell(currPos.Next(1,0))) 
			+ floodFill(getCell(currPos.Next(0,1))) 
			+ floodFill(getCell(currPos.Next(-1,0))));
	}
	else
		return 0; //Not valid tile
}

Cell* World::getCell(cint location)
{
	if ((location.X() > Settings::worldx - 1 || location.X() < 0)||(location.Y() > Settings::worldy - 1 || location.Y() < 0))
		return &dummy;
	else
		return map[location.X()][location.Y()];
}

Cell* World::getCell(int x, int y)
{
	if ((x > Settings::worldx - 1 || x < 0)||(y > Settings::worldy - 1 || y < 0))
		return &dummy;
	else
		return map[x][y];
}

int World::getTile(int x, int y)
{
	if (x < 0 || x > Settings::worldx -1 || y < 0 || y > Settings::worldy -1)
		return dummy.getTile();
	else
		return map[x][y]->getTile();
}


cint World::getStart()
{
	cint start = iterateOverWorldForPosition([&](cint** returnValue, cint* pos){
		Cell* c = getCell(*pos);
		if(c->getType() == FLOOR && c->empty())
				*returnValue = pos;
	});
	return start;
}

void World::update(Player* plr)
{
	for (unsigned int i = 0; i < monsters.size(); i++)
	{
		if (monsters.at(i)->active)
		{
			monsters.at(i)->update();
			monsters.at(i)->AI(plr);
		}
	}
	
	updateVisibility(plr);
}

void World::nextFloor(Player* plr)
{
	difficultyLevel += 50;
	plr->clearCell();
	//The monsters array has the responsibility of deleting the monsters.
	for (unsigned int i = 0; i < monsters.size(); i++)
	{
		delete monsters[i];
	}
	monsters.clear();

	for (unsigned int i = 0; i < pickups.size(); i++)
	{
		delete pickups[i];
	}
	pickups.clear();

	for (unsigned int i = 0; i < actors.size(); i++)
	{
		delete actors[i];
	}
	actors.clear();

	for (unsigned int i = 0; i < inventory.size(); i++)
	{
		delete inventory[i];
	}
	inventory.clear();

	//The cells have the responsibility of deleting the pickups.
	for (int i = 0; i < Settings::worldx; i++)
	{
		for (int j = 0; j < Settings::worldy; j++)
		{
			delete map[i][j];
		}
	}

	this->style = styles.at(rand() % (styles.size()));
	initialize();
	cint newStart = this->getStart();
	plr->nextFloor(newStart, this->getCell(newStart.X(), newStart.Y()));
	updateVisibility(plr);
}

void World::setStairs()
{
	bool picked = false;
	while (!picked)
	{
		int i = rand() % 52 + 6;
		int j = rand() % 52 + 6;
		if(map[i][j]->getType() == FLOOR)
		{
			map[i][j]->makeStairs();
			picked = true;
		}
	} 
} 

Cell* World::findShop()
{
	for (int i = 0; i < Settings::worldx; i++)
	{
		for (int j = 0; j < Settings::worldy; j++)
		{
			if (map[i][j]->getType() == SHOP)
				return map[i][j];
		}
	}
	return 0; //This is a problem.
}

void World::initShop()
{
	Shop = 0;
	bool picked = false;
	while (!picked)
	{
		int i = rand() % 52 + 6;
		int j = rand() % 52 + 6;
		if(map[i][j]->getType() == FLOOR)
		{
			map[i][j]->makeShop();
			picked = true;
			this->Shop = map[i][j];
		}
	} 
	//I have to do the for loop multiple times so that the order is correct.
	for (int i = 0; i < 200; i+=50)
	{
		this->inventory.push_back(new Pickup(PickupDef(difficultyLevel + i, HEALTH)));
		this->inventory.push_back(new Pickup(PickupDef(difficultyLevel + i, WEAPON)));
		this->inventory.push_back(new Pickup(PickupDef(difficultyLevel + i, SHIELD)));
		this->inventory.push_back(new Pickup(PickupDef(difficultyLevel + i, ARMOR)));
		this->inventory.push_back(new Pickup(PickupDef(difficultyLevel + i, SCROLL)));
	}
	Utilities::sortInventory(&this->inventory);
}

void World::generateVisibility()
{
	const double PI = 4.0*atan(1.0);
	points.push_back(cint(viewRange, 0));
	int end = points.size() - 1;
	for(double i = 0.01; i < (2.0 * PI); i += 0.01)
	{
		cint point = cint(round(3.0 * sin(i)), round(3.0 * cos(i)));
		if (points.at(end) != point)
		{
			points.push_back(point);
			end++;
		}
	}
	paths.resize(points.size());
	for (vector<cint>::iterator itr = points.begin(); itr != points.end(); itr++)
	{
		findLinePath(viewRange, itr - points.begin(), cint(0,0), *itr);
	}
}

void World::updateVisibility(Player* plr)
{
	cint currPos = plr->Pos();
	getCell(currPos.X(), currPos.Y())->Visibility(0xFF);
	for (vector<vector<cint>>::iterator list = paths.begin(); list != paths.end(); list++)
	{
		cint newPos = currPos;
		int currVisibility = 0xFF;
		for (vector<cint>::iterator itr = (*list).begin(); itr != (*list).end(); itr++)
		{
			newPos = newPos + *itr;
			Cell* currCell = getCell(newPos.X(), newPos.Y());
			if (currCell->getType() == WALL)
			{
				currCell->Visibility(currVisibility);
				currVisibility = 0x00;
				break;
			}
			else
			{
				currCell->Visibility(currVisibility);
				currVisibility -= (0xFF / viewRange);
			}
		}
	}
}

int World::round(double roundMe)
{
	return (roundMe > 0.0) ? (roundMe + 0.5) : (roundMe - 0.5);
}

void World::findLinePath(int radius, int index, cint start, cint end)
{
	int x0 = start.X();
	int x1 = end.X();
	int y0 = start.Y();
	int y1 = end.Y();
	int deltaX = x1 - x0;
	int deltaY = y1 - y0;
	int bigger, smaller = 0;
	cint bigDirection, smallDirection;
	if (deltaY > deltaX) 
	{
		bigger = deltaY;
		smaller = deltaX;
		bigDirection = (y1 > y0) ? (cint(0,1)) : (cint(0,-1));
		smallDirection = (x1 > x0) ? (cint(1,0)) : (cint(-1,0));
	}
	else
	{
		bigger = deltaX;
		smaller = deltaY;
		bigDirection = (x1 > x0) ? (cint(1,0)) : (cint(-1,0));
		smallDirection = (y1 > y0) ? (cint(0,1)) : (cint(0,-1));
	}

	int counter = 0;
	cint netChange = cint(0,0);
	while (netChange != end)
	{
		if (abs(counter) < abs(bigger))
		{
			paths.at(index).push_back(bigDirection);
			counter += abs(smaller);
			netChange = netChange + bigDirection;
		}
		else
		{
			paths.at(index).push_back(smallDirection);
			counter -= abs(bigger);
			netChange = netChange + smallDirection;
		}
	}
}

void World::findActors()
{
	iterateOverWorld([&](cint* pos){
		Cell* c = getCell(*pos);
			if(c->hasActor() && !c->getActor()->isPlayer)
			{
				monsters.push_back(c->getActor());
			}
		});
}

Player* World::getPlayer()
{
	return plr;
}

void World::setPlayer(Player* plr)
{
	this->plr = plr;
}