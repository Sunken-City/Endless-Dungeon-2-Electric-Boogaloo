#pragma once

class TileSet
{
public:
	int floor;
	int altFloor;
	int wall;

	TileSet();
	TileSet(int floorTile, int altFloorTile, int wallTile);
	
};