#include "Actor.h"
#include "Cell.h"



Actor::Actor()
{

}

Actor::Actor(ActorDef* actorType, cint position, Cell* currentCell)
{
	type = actorType;
	this->hP = type->HP();
	this->position = position;
	this->tileNum = type->Tile();
	this->currCell = currentCell;
	this->homeTile = new Cell(currentCell->Pos().X(), currentCell->Pos().Y());
	this->active = true;
	this->mute = false;
	this->maxStamina = 10;
	this->stamina = this->maxStamina;
	this->Status = NORMAL;
}

Actor::~Actor()
{

}

void Actor::serialize(Serializer write)
{

}

void Actor::reconstruct(Serializer read)
{

}

int Actor::HP()
{
	return this->hP;
}

void Actor::HP(int hp)
{
	this->hP = hp;
}

void Actor::heal(int heals)
{
	ostringstream message;
	this->hP += heals;
	if (this->hP > type->HP())
		this->hP = type->HP();
	message << this->Name() << " healed " << heals << " HP!";
	Console::log(message.str().c_str(), 0x00FF00FF);
}

void Actor::takesDamage(int enemyAttack)
{
	ostringstream message;
	int defRoll = this->DEF();

	if((enemyAttack - defRoll) > 0)
	{
		this->hP -= (enemyAttack - defRoll);
		message << this->Name() << " takes " << (enemyAttack - defRoll) << " damage!";
		//if (!mute)
			Sound::play("damage.sfs");
	}
	else
	{
		message << this->Name() << " blocks the attack!";
		//if (!mute)
			Sound::play("block.sfs");
	}

	Console::log(message.str().c_str(), 0xFF0000FF);
}

cint Actor::Pos()
{
	return this->position;
}

void Actor::Pos(cint newPosition)
{
	this->position = newPosition;
}

int Actor::Tile()
{
	return tileNum;
}

ActorDef* Actor::Type()
{
	return this->type;
}

int Actor::ATT()
{
	return this->type->ATT();
}

int Actor::DEF()
{
	return this->type->DEF();
}

string Actor::Name()
{
	return this->type->Name();
}

bool Actor::move(cint newPosition, Cell* newCell)
{
	if (((newCell->getType() == FLOOR) || (newCell->getType() == STAIRS) || (newCell->getType() == SHOP)) && (!newCell->hasActor()))
	{
		if (this->currCell != 0)
			this->currCell->setActor(0);
		this->currCell = newCell;
		this->currCell->setActor(this);
		this->position = newPosition;
		return true; //We successfully moved.
	}
	else
		return false; //We bumped into something.
}

Cell* Actor::getCell()
{
	return this->currCell;
}

void Actor::die()
{
	if (this->currCell != 0)
	{
		Player::gainExperience(this->type->HP() * 10);
		string output = "You have slain the " + this->Name();
		Sound::play("murder.sfs");
		Console::log(output.c_str(), 0xFFFFFFFF);
		this->currCell->setActor(0);
		this->currCell = 0;	
	}
}

void Actor::update()
{
	if (this->HP() < 1)
	{
		this->die();
		this->active = false;
	}
}

void Actor::AI(Actor* player)
{
	if (this->currCell != 0)
	{
		if (cint::manhattan(player->Pos(), this->position) < 4 && this->stamina > 0)
		{
			cint direction = cint::getDirection(player->Pos(), this->position);
			//The enemies can move on diagonals. This makes them a little more challenging, and makes the code nice looking <3
			this->move(this->Pos() + direction, *(this->currCell) + direction);
			this->stamina--;
		}
		else
		{
			//If we've strayed from our home tile, go back towards it
			if (cint::manhattan(this->homeTile->Pos(), this->position) > 4)
			{
				cint direction = cint::getDirection(this->homeTile->Pos(), this->position);
				this->move(this->Pos() + direction, *(this->currCell) + direction);
			}
			else
			{
				Cell* next = *(this->currCell) + cint::randNext();
				this->move(next->Pos(), next);
			}
			if (this->stamina < this->maxStamina)
				this->stamina++;
		}
		if (this->stamina == 0)
		{
			delete(this->homeTile);
			this->homeTile = new Cell(this->currCell->Pos().X(), this->currCell->Pos().Y());
		}
		if (cint::manhattan(player->Pos(), this->position) == 1)
		{
			player->takesDamage(this->ATT());
		}
	}
}

void Actor::describe()
{
	ostringstream description;
	description << this->Name() << " HP: " << this->HP() << "/" << this->Type()->HP();
	Console::quickLog(description.str().c_str());
}