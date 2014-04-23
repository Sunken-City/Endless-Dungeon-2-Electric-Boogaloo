#include "Utilities.h"
#include "Pickup.h"

bool Utilities::comparePickups(Pickup* first, Pickup* second)
{
	return (first->ItemType() < second->ItemType());
}

void Utilities::sortInventory(vector<Pickup*>* inventory)
{
	sort((*inventory).begin(), (*inventory).end(), &Utilities::comparePickups);
}

void Utilities::serializeInventory(vector<Pickup*>* inventory, Serializer write)
{
	int inventorySize = inventory->size();
	write.IO<int>(inventorySize);
	for (vector<Pickup*>::iterator itr = inventory->begin(); itr != inventory->end(); itr++)
	{
		(*itr)->serialize(write);
	}
}