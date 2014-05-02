#include "Player.h"
#include "tilelib.h"
#include "Pickup.h"
#include "World.h"

int Player::experience = 0;

Player::Player()
{
	this->isPlayer = true;
	this->mute = true;
	this->active = true;
	this->hurtCounter = 0;
	this->world = 0;
}

Player::Player(cint startingPosition, World* gameWorld)
{
	this->isPlayer = true;
	this->type = new ActorDef(0x0AC, "Pico", 100, Dice(2,3), Dice(2,2));
	this->position = startingPosition;
	this->hP = type->HP();
	this->position = position;
	this->tileNum = type->Tile();
	this->currCell = 0;
	this->homeTile = cint(0,0);
	this->sword = 0;
	this->shield = 0;
	this->armor = 0;
	this->gold = 0;
	this->mute = true;
	this->level = 1;
	experience = 0;
	this->active = true;
	this->world = gameWorld;
}

Player::~Player()
{

}

void Player::serialize(Serializer write)
{
	Actor::serialize(write);
	write.IO<int>(this->level);
	write.IO<int>(this->gold);
	write.IO<int>(experience);
	Utilities::serializeInventory(&inventory, write);
}

Player* Player::reconstruct(Serializer read)
{
	Player* p = new Player();
	read.IO<bool>(p->isPlayer);
	p->position.reconstruct(read);
	p->type = ActorDef::reconstruct(read);
	p->tileNum = p->type->Tile();
	read.IO<int>(p->stamina);
	read.IO<int>(p->maxStamina);
	read.IO<int>(p->hP);
	read.IO<status>(p->Status);
	read.IO<int>(p->level);
	read.IO<int>(p->gold);
	read.IO<int>(experience);
	int inventorySize;
	read.IO<int>(inventorySize);
	p->armor = 0;
	p->shield = 0;
	p->sword = 0;
	for (int i = 0; i < inventorySize; i++)
	{
		Pickup* item = Pickup::reconstruct(read);
		p->inventory.push_back(item);
		if(item->equipped)
		{
			p->equip(item);
		}
	}
	return p;
}

void Player::die()
{
	Console::log("Oh Dear! You have Died!", 0xFFFF00FF);
	Sound::play("deathSound.sfs");
	this->active = false;
}

int Player::expToNext()
{
	return (int)(100 * (pow(2.0, this->level - 1)));
}

void Player::update()
{	
	if (experience > expToNext())
	{
		this->level += 1;
		Console::log("Level Up!", 0xFF9900FF);
		Sound::play("levelUp.sfs");
		this->Type()->HP(5 * this->level);
		this->hP += (5 * this->level);
		this->Type()->attackDice.levelUp(this->level / 2);
		this->Type()->defenceDice.levelUp(this->level / 2);
	}
	if (this->HP() < 1)
		this->die();
	if (this->currCell->hasPickup())
	{
		Pickup* item = this->currCell->getPickup();
		if (item->Type().ItemType() == GP)
			this->hollaHollaGetDolla(item->Type().Value());
		else if (inventory.size() >= 48)
		{
			Console::log("Your Inventory is full!", 0xFFFFFFFF);
			return;
		}
		else
		{
			this->inventory.push_back(item);
			Utilities::sortInventory(&this->inventory);
		}
		this->currCell->setPickup(0);
		item->pickedUp();
	}
}

void Player::printInventory(cint mousePos)
{
	cint currentLocation;
	ostringstream health, attack, defence, gold, level, xp;
	int goldSpot = (tl_xres() - 6) * 2;
	int hp = this->HP();
	int maxHP = this->Type()->HP();
	level << "LVL: " << this->level;
	xp << "XP: " << experience << "/" << expToNext();
	health << "HP: " << hp << "/" << maxHP;
	attack << "ATT:" << this->Type()->attackDice.print() << "+" << getAttModifier(false);
	defence << "DEF:" << this->Type()->defenceDice.print() << "+" << getDefModifier(false);
	gold << " X" << this->gold;
	/*This statement shifts the color from Green to Red slowly depending on the ratio of the player's HP
	* In Short:
	* Green = 0xFF * (HP/MaxHP)
	* Red = 0xFF * (1 - (HP/MaxHP))
	*/
	int hpColor = 0xFF + (0x1000000 * (int)(0xFF * (1.0 - ((double)hp/(double)maxHP))) + (0x10000 * (int)(0xFF * ((double)hp/(double)maxHP))));
	Console::print(level.str().c_str(), (tl_xres() - 6) * 2, 0);
	Console::print(xp.str().c_str(), (tl_xres() - 6) * 2, 1);
	Console::print(health.str().c_str(), (tl_xres() - 6) * 2, 2, hpColor);
	Console::print(attack.str().c_str(), (tl_xres() - 6) * 2, 3);
	Console::print(defence.str().c_str(), (tl_xres() - 6) * 2, 4);
	Console::print(gold.str().c_str(), (tl_xres() - 6) * 2, 5);
	tl_scale(2);
	tl_rendertile(0x2FA, goldSpot, 5); //Put in the gold piece
	tl_scale(1);
	Console::print("Inventory:", (tl_xres() - 6) * 2, 7);
	int length = this->inventory.size();
	for (int i = 0; i < length; i++)
	{
		this->inventory.at(i)->inventoryPosition = i;
		currentLocation = cint(tl_xres() - 6 + (i % 6), 4 + (i / 6));
		if (mousePos == currentLocation)
		{
			tl_color(0xFF0000FF);
			tl_rendertile(0x13E, currentLocation.X(), currentLocation.Y());
			if (Shop::inStore)
				this->inventory.at(i)->describe(SELL);
			else
				this->inventory.at(i)->describe();
		}
		if (this->inventory.at(i)->equipped)
		{
			tl_color(0x84E900FF);
			tl_rendertile(0x13E, currentLocation.X(), currentLocation.Y());
		}
		tl_color(0xFFFFFFFF);
		tl_rendertile(this->inventory.at(i)->Tile(), currentLocation.X(), currentLocation.Y());
	}
}

void Player::useItem(cint mousePos)
{
	Pickup* item = 0;
	if ((mousePos.X() > tl_xres() - 7) && (mousePos.Y() > 3))
	{
		int inventoryPos = (((mousePos.Y() - 4) * 6) + abs((tl_xres() - mousePos.X() - 6)));
		if (this->inventory.size() > inventoryPos)
		{
			item = this->inventory.at(inventoryPos);
			item->use(this, item);
			//If it breaks, then delete it.
			if (((item->ItemType() == HEALTH) || (item->ItemType() == SCROLL)) && item->degrade())
			{
				breakItem(item);
			}
		}
	}
}

void Player::sellItem(cint mousePos, vector<Pickup*>* shopInventory)
{
	Pickup* item = 0;
	int mx = mousePos.X();
	int my = mousePos.Y();
	if ((mx > tl_xres() - 7) && (my > 3))
	{
		int inventoryPos = (((my - 4) * 6) + abs((tl_xres() - mx - 6)));
		if (this->inventory.size() > inventoryPos)
		{
			item = this->inventory.at(inventoryPos);
			if(item->equipped)
				this->equip(item); //This will actually unequip it, despite what the method call looks like.
			shopInventory->push_back(item);
			this->hollaHollaGetDolla(item->Price(SELL));
			breakItem(item); //Sneaky way to remove from inventory. Literally the same call.
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
	Sound::play("break.sfs");

	item->equipped = false;
	this->inventory.erase(inventory.begin() + item->inventoryPosition);
	recalculatePositions();
}

void Player::recalculatePositions()
{
	int i = 0;
	for (vector<Pickup*>::iterator itr = this->inventory.begin(); itr != this->inventory.end(); itr++)
	{
		(*itr)->inventoryPosition = i;
		i++;
	}
}

void Player::breakItem(Pickup* item)
{
	this->inventory.erase(this->inventory.begin() + item->inventoryPosition);
	recalculatePositions();
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

void Player::gainExperience(int exp)
{
	experience += exp;
}
