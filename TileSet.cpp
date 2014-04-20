#include "TileSet.h"

TileSet::TileSet()
{

}

TileSet::TileSet(int floorTile, int altFloorTile, int wallTile)
{
	this->floor = floorTile;
	this->altFloor = altFloorTile;
	this->wall = wallTile;
}