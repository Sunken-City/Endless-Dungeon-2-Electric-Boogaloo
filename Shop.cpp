#include "Shop.h"
#include "tilelib.h"

bool Shop::inStore = false;

void Shop::render(cint mousePos, vector<Pickup*>* inventory)
{
	Shop::inStore = true;
	cint currentLocation;
	Console::print("Shop:", (tl_xres() - 6) * 2, (tl_yres() - 6) * 2 + 1);
	int length = inventory->size();
	for (int i = 0; i < length; i++)
	{
		inventory->at(i)->inventoryPosition = i;
		tl_scale(1);
		currentLocation = cint(tl_xres() - 6 + (i % 6), tl_yres() - 5 + (i / 6));
		if (mousePos == currentLocation)
		{
			tl_color(0xFF0000FF);
			tl_rendertile(0x125, currentLocation.X(), currentLocation.Y());
			tl_color(0xFFFFFFFF);
			inventory->at(i)->describe(BUY);
		}
		else
		{
			tl_rendertile(0x116, currentLocation.X(), currentLocation.Y());
		}
		tl_rendertile(inventory->at(i)->Tile(), currentLocation.X(), currentLocation.Y());
	}
}

void Shop::buyItem(cint mousePos, vector<Pickup*>* shopInventory, Player* plr)
{
	Pickup* item = 0;
	if ((mousePos.X() > tl_xres() - 7) && (mousePos.Y() > tl_yres() - 6))
	{
		int inventoryPos = ((abs((tl_yres() - mousePos.Y() - 5)) * 6) + abs((tl_xres() - mousePos.X() - 6)));
		if (shopInventory->size() > inventoryPos)
		{
			item = shopInventory->at(inventoryPos);
			if (plr->inventory.size() >= 48)
			{
				Console::log("Your Inventory is full!", 0xFFFFFFFF);
				return;
			}
			if (plr->purchase(item->Price(BUY)))
			{
				plr->inventory.push_back(item);
				shopInventory->erase(shopInventory->begin() + item->inventoryPosition);
				ostringstream message;
				message << "You bought the " << item->Name() << " for " << item->Price(BUY) << "GP";
				Console::log(message.str().c_str(),0xFFFF00FF);
				Sound::play("gold.sfs");
			}
			else
				Console::log("You can't afford that!",0xFFFF00FF);
		}
	}
}