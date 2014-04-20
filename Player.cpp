#include "Player.h"
#include "tilelib.h"
#include "Pickup.h"

int Player::experience = 0;

Player::Player()
{

}

Player::Player(cint startingPosition)
{
	this->type = new ActorDef(0x0AC, "Pico", 100, Dice(1,3), Dice(1,2));
	this->position = startingPosition;
	this->hP = type->HP();
	this->position = position;
	this->tileNum = type->Tile();
	this->currCell = 0;
	this->homeTile = new Cell(0,0);
	this->sword = 0;
	this->shield = 0;
	this->armor = 0;
	this->gold = 0;
	this->mute = true;
	this->mana = 20;
	this->maxMana = 20;
	this->level = 1;
	experience = 0;
}

Player::~Player()
{

}

void Player::serialize(Serializer write)
{

}

void Player::reconstruct(Serializer read)
{

}

void Player::die()
{
	Console::log("Oh Dear! You have Died!", 0xFFFF00FF);
	Sound::play("deathSound.sfs");
	tl_shutdown();
}

void Player::update()
{	
	if (experience > 100 * (pow(2.0, this->level - 1)))
	{
		this->level += 1;
		Console::log("Level Up!", 0xFF9900FF);
		this->Type()->HP(10 * this->level);
		this->hP += (10 * this->level);
		this->mana += (10 * this->level);
		this->maxMana += (10 * this->level);
	}
	if (this->HP() < 1)
		this->die();
	if (this->currCell->hasPickup())
	{
		Pickup* item = this->currCell->getPickup();
		if (item->Type().ItemType() == GP)
			this->hollaHollaGetDolla(item->Type().Value());
		else if (inventory.size() >= 54)
		{
			Console::log("Your Inventory is full!", 0xFFFFFFFF);
			return;
		}
		else
			this->inventory.push_back(item);
		this->currCell->setPickup(0);
		item->pickedUp();
	}
}

void Player::printInventory(cint mousePos)
{
	cint currentLocation;
	ostringstream health, mana, attack, defence, gold;
	int goldSpot = (tl_xres() - 6) * 2;
	health << "HP: " << this->HP() << "/" << this->Type()->HP();
	mana << "MP: " << this->mana << "/" << this->maxMana;
	attack << "ATT: " << getAttModifier(false);
	defence << "DEF: " << getDefModifier(false);
	gold << " X" << this->gold;
	Console::print(health.str().c_str(), (tl_xres() - 6) * 2, 0);
	Console::print(mana.str().c_str(), (tl_xres() - 6) * 2, 1);
	Console::print(attack.str().c_str(), (tl_xres() - 6) * 2, 2);
	Console::print(defence.str().c_str(), (tl_xres() - 6) * 2, 3);
	Console::print(gold.str().c_str(), (tl_xres() - 6) * 2, 4);
	tl_scale(2);
	tl_rendertile(0x2FA, goldSpot, 4); //Put in the gold piece
	tl_scale(1);
	Console::print("Inventory:", (tl_xres() - 6) * 2, 5);
	int length = this->inventory.size();
	for (int i = 0; i < length; i++)
	{
		this->inventory.at(i)->inventoryPosition = i;
		currentLocation = cint(tl_xres() - 6 + (i % 6), 3 + (i / 6));
		if (mousePos == currentLocation)
		{
			tl_color(0xFF0000FF);
			tl_rendertile(0x125, currentLocation.X(), currentLocation.Y());
			tl_color(0xFFFFFFFF);
			if (Shop::inStore)
				this->inventory.at(i)->describe(SELL);
			else
				this->inventory.at(i)->describe();
		}
		if (this->inventory.at(i)->equipped)
		{
			tl_color(0x00FF00FF);
			tl_rendertile(0x125, currentLocation.X(), currentLocation.Y());
			tl_color(0xFFFFFFFF);
		}
		tl_rendertile(this->inventory.at(i)->Tile(), currentLocation.X(), currentLocation.Y());
	}
}

void Player::useItem(cint mousePos)
{
	Pickup* item = 0;
	if ((mousePos.X() > tl_xres() - 7) && (mousePos.Y() > 2))
	{
		int inventoryPos = (((mousePos.Y() - 3) * 6) + abs((tl_xres() - mousePos.X() - 6)));
		if (this->inventory.size() > inventoryPos)
		{
			item = this->inventory.at(inventoryPos);
			if (item->ItemType() == SPELL)
			{
				if (cast(item->Type().castingCost()))
				{
					item->use(this, item);
					if (item->degrade())
						breakItem(item);
				}
			}
			else
			{
				item->use(this, item);
			}
			//If it breaks, then delete it.
			if (item->ItemType() == HEALTH && item->degrade())
			{
				breakItem(item);
			}
		}
	}
}

void Player::sellItem(cint mousePos, vector<Pickup*>* shopInventory)
{
	Pickup* item = 0;
	if ((mousePos.X() > tl_xres() - 7) && (mousePos.Y() > 2))
	{
		int inventoryPos = (((mousePos.Y() - 3) * 6) + abs((tl_xres() - mousePos.X() - 6)));
		if (this->inventory.size() > inventoryPos)
		{
			item = this->inventory.at(inventoryPos);
			if(item->equipped)
				this->equip(item); //This will actually unequip it, despite what the method call looks like.
			shopInventory->push_back(item);
			this->hollaHollaGetDolla(item->Price(SELL));
			breakItem(item);
			ostringstream message;
			message << "You sold your " << item->Name() << " for " << item->Price(SELL) << "GP";
			Sound::play("gold.sfs");
			Console::log(message.str().c_str(),0xFFFF00FF);
		}
	}
}

void Player::equip(Pickup* item)
{
	ostringstream message;
	Pickup** slot;
	if (item->ItemType() == WEAPON)
		slot = &this->sword;
	else if (item->ItemType() == SHIELD)
		slot = &this->shield;
	else if (item->ItemType() == ARMOR)
		slot = &this->armor;
	if (*slot == 0) //Equip
	{
		*slot = item;
		item->equipped = true;
		message << "Equipped " << item->Name();
	}
	else if (*slot == item) //Unequip
	{
		*slot = 0;
		item->equipped = false;
		message << "Unequipped " << item->Name();
	}
	else //Swap equips
	{
		message << "Swapped " << (*slot)->Name() << " for " << item->Name();
		(*slot)->equipped = false;
		*slot = item;
		item->equipped = true;
		
	}
	Console::log(message.str().c_str(), 0x35C0CDFF);
	Sound::play("equip.sfs");
}

int Player::ATT()
{
	return this->type->ATT() + getAttModifier(true);
}

int Player::DEF()
{
	return this->type->DEF() + getDefModifier(true);
}

int Player::getDefModifier(bool checkForDegrade)
{
	int modifier = 0;
	if (this->shield != 0)
	{
		modifier += this->shield->Type().Value();
		if (checkForDegrade && this->shield->degrade())
			breakItem(this->shield, &this->shield);
	}
	if (this->armor != 0)
	{
		modifier += this->armor->Type().Value();
		if (checkForDegrade && this->armor->degrade())
			breakItem(this->armor, &this->armor);
	}
	return modifier;
}

int Player::getAttModifier(bool checkForDegrade)
{
	int modifier = 0;
	if (this->sword != 0)
	{
		modifier += this->sword->Type().Value();
		if (checkForDegrade && this->sword->degrade())
			breakItem(this->sword, &this->sword);
	}
	return modifier;
}

void Player::breakItem(Pickup* item, Pickup** slot)
{
	*slot = 0;

	ostringstream message;
	message << "Your " << item->Name() << " broke!";
	Console::log(message.str().c_str(), 0x35C0CDFF);

	item->equipped = false;
	this->inventory.erase(inventory.begin() + item->inventoryPosition);
}

void Player::breakItem(Pickup* item)
{
	this->inventory.erase(inventory.begin() + item->inventoryPosition);
}

void Player::clearCell()
{
	this->currCell = 0;
}

void Player::nextFloor(cint newPosition, Cell* newCell)
{
	this->currCell = newCell;
	this->currCell->setActor(this);
	this->position = newPosition;
}

//I couldn't resist. I had to name it this.
void Player::hollaHollaGetDolla(int dolla)
{
	this->gold += dolla;
}

bool Player::purchase(int price)
{
	if((this->gold - price) > -1)
	{
		this->gold -= price;
		return true;
	}
	else
		return false;
}

bool Player::cast(int cost)
{
	if (this->mana - cost >= 0)
	{
		this->mana -= cost;
		return true;
	}
	else
	{
		Console::log("Not enough mana!", 0x0000FFFF);
		return false;
	}
}

void Player::gainExperience(int exp)
{
	experience += exp;
}

void Player::heal(int hp, int mana)
	{
		ostringstream message;
		ostringstream manaMessage;
		this->hP += hp;
		if (this->hP > type->HP())
			this->hP = type->HP();
		message << this->Name() << " healed " << hp << " HP!";
		Console::log(message.str().c_str(), 0x00FF00FF);
		this->mana += mana;
		if (this->mana > maxMana)
			this->mana = maxMana;
		manaMessage << this->Name() << " restored " << mana << " Mana!";
		Console::log(manaMessage.str().c_str(), 0x0099FFFF);
	}