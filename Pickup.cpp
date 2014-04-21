
#include "Pickup.h"
#include "Cell.h"


Pickup::Pickup()
{

}

Pickup::Pickup(PickupDef pickupType)
{
	this->type = pickupType;
	this->position = cint(0,0);
	this->tileNum = type.Tile();
	this->currCell = 0;
	this->name = type.Name();
	this->use = type.use;
	this->durability = type.Durability();
	this->equipped = false;
	this->inventoryPosition = 0;
}

Pickup::Pickup(PickupDef pickupType, cint position, Cell* currentCell)
{
	this->type = pickupType;
	this->position = position;
	this->tileNum = type.Tile();
	this->currCell = currentCell;
	this->name = type.Name();
	this->use = type.use;
	this->durability = type.Durability();
	this->equipped = false;
	this->inventoryPosition = 0;
}

void Pickup::serialize(Serializer write)
{
	write.IO<int>(this->tileNum);
	this->position.serialize(write);
	this->type.serialize(write);
	write.IO<int>(this->durability);
	write.IO<int>(this->inventoryPosition);
	write.IO<bool>(this->equipped);
}

Pickup* Pickup::reconstruct(Serializer read)
{
	Pickup* p = new Pickup();
	read.IO<int>(p->tileNum);
	p->position.reconstruct(read);
	p->type = *PickupDef::reconstruct(read);
	read.IO<int>(p->durability);
	read.IO<int>(p->inventoryPosition);
	read.IO<bool>(p->equipped);
	p->name = p->type.Name();
	p->use = p->type.use;
	return p;
}

Pickup::~Pickup()
{
	this->currCell = 0;
}

cint Pickup::Pos()
{
	return this->position;
}

void Pickup::Pos(cint newPosition)
{
	this->position = newPosition;
}

int Pickup::Tile()
{
	return this->tileNum;
}

int Pickup::ItemType()
{
	return this->type.ItemType();
}

PickupDef Pickup::Type()
{
	return this->type;
}

string Pickup::Name()
{
	return this->name;
}

Cell* Pickup::getCell()
{
	return this->currCell;
}

void Pickup::pickedUp()
{
	ostringstream message;
	
	if (this->type.ItemType() == GP)
	{
		message << "You found " << this->type.Value() << " Gold Pieces";
		Sound::play("gold.sfs");
	}
	else
	{
		message << "You picked up the " << this->name;
		Sound::play("pickup.sfs");
	}
	Console::log(message.str().c_str(), 0xFFFFFFFF);
	this->currCell = 0; //Unset the pointer, it's no longer associated with a cell.
}

bool Pickup::degrade()
{
	this->durability -= 1;
	if (this->durability <= 0)
		return true; //It's broken!
	else
		return false;
}

int Pickup::Durability()
{
	return this->durability;
}

void Pickup::describe()
{
	ostringstream description;
	//description << this->Name() << " Price: " << this->Price();
	if (this->type.ItemType() == GP)
		description << this->Type().Value() << " Gold Pieces";
	else
		description << this->Name() << "(+" << this->Type().Value() << ") Use:" << this->Durability() << "/" << this->Type().Durability() << " GP:" << this->Price(SELL);
	Console::quickLog(description.str().c_str());
}

void Pickup::describe(action Action)
{
	ostringstream description;
	//description << this->Name() << " Price: " << this->Price();
	if (Action == SELL)
		description << "SELL:" << this->Name() << "(+" << this->Type().Value() << ") Use:" << this->Durability() << "/" << this->Type().Durability() << " GP:" << this->Price(SELL);
	else
		description << "BUY:" << this->Name() << "(+" << this->Type().Value() << ") Use:" << this->Durability() << "/" << this->Type().Durability() << " GP:" << this->Price(BUY);
	Console::quickLog(description.str().c_str());
}

int Pickup::Price()
{
	if (this->Type().ItemType() == HEALTH) //Because they lack durability, pots end up being too cheap and need to be inflated for $$$$
		return 10 * (int)((double)this->Type().Value() * (((double)this->Durability() * (double)this->Durability()) / (double)this->Type().Durability()));
	else
		return (int)((double)this->Type().Value() * (((double)this->Durability() * (double)this->Durability()) / (double)this->Type().Durability()));
}

int Pickup::Price(action Action)
{
	if (Action == BUY)
		return (this->Price() * 2);
	else
		return (this->Price() / 2);
}


void Pickup::attackSurrounding (Player* plr, int damage, status Status)
{
	plr->Pos();
}