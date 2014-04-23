#pragma once
#include <algorithm>
#include <vector>
#include "Serializer.h"

using namespace std;

class Pickup;

class Utilities
{
public:

	static bool comparePickups(Pickup* first, Pickup* second);
	static void sortInventory(vector<Pickup*>* inventory);
	static void serializeInventory(vector<Pickup*>* inventory, Serializer write);
};