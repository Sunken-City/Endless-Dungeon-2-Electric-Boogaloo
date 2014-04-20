#pragma once
#include "Pickup.h"
#include "Player.h"
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Shop
{
public:
	static bool inStore;
	static void render(cint mousePos, vector<Pickup*>* shopInventory);
	static void buyItem(cint mousePos, vector<Pickup*>* shopInventory, Player* plr);

private:
	
};