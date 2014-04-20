#include "windows.h"
#include "tilelib.h"
#include "Coord.h"
#include "Shop.h"
#include "Cell.h"
#include "World.h"
#include "Player.h"
#include "Console.h"
#include "Settings.h"
#include "Serializeable.h"
#include <cstdlib>
#include <string>
#include <vector>


using namespace std;

Player plr;
World gameWorld;

bool canChangeFloor = false;
bool newGameHover = false;
bool loadGameHover = false;
bool onMenu = true;
bool loaded = false;

cint RIGHT = cint(1,0);
cint LEFT = cint(-1,0);
cint DOWN = cint(0,1);
cint UP = cint(0,-1);

int Settings::worldx = 64;
int Settings::worldy = 64;

cint mousePosition;

void save()
{
	Serializer write = Serializer(false);

	int headerCode = 9001;
	write.IO<int>(headerCode);

	double versionNumber = 1.0;
	write.IO<double>(versionNumber);

	//WorldSize
	write.IO<int>(Settings::worldx);
	write.IO<int>(Settings::worldy);

	gameWorld.serialize(write);

	write.close();
}

void load()
{
	Serializer read = Serializer(true);

	int headerCode;
	read.IO<int>(headerCode);
	if (headerCode != 9001)
	{
		read.close();
		throw InvalidHeaderException();
		return;
	}

	double versionNumber;
	read.IO<double>(versionNumber);
	if (versionNumber != 1.0)
	{
		read.close();
		throw InvalidVersionException();
		return;
	}

	//WorldSize
	read.IO<int>(Settings::worldx);
	read.IO<int>(Settings::worldy);

	gameWorld.reconstruct(read);

	read.close();
	loaded = true;
}

void takeTurn(cint coord)
{
	Actor* monster;
	if((gameWorld.getCell(plr.Pos() + coord)->hasActor()))
	{
		monster = gameWorld.getCell(plr.Pos() + coord)->getActor();
		monster->takesDamage(plr.ATT());
	}
	else if(gameWorld.getCell(plr.Pos() + coord)->empty())
		plr.move(plr.Pos() + coord, gameWorld.getCell(plr.Pos() + coord));

	//Allow the monsters to take their turns
	gameWorld.update(&plr);
	//Check for death
	plr.update();
}

void update()
{
	if (tl_buttonwentdown())
	{
		if(Shop::inStore)
		{
			plr.sellItem(mousePosition, &gameWorld.inventory);
			Shop::buyItem(mousePosition, &gameWorld.inventory, &plr);
		}
		else
			plr.useItem(mousePosition);
	}
	if (tl_keywentdown("escape"))
	{
		save();
		tl_shutdown();
	}
	else if (tl_keywentdown("right")||tl_keywentdown("d"))
		takeTurn(RIGHT);
	else if (tl_keywentdown("left")||tl_keywentdown("a"))
		takeTurn(LEFT);
	else if (tl_keywentdown("down")||tl_keywentdown("s"))
		takeTurn(DOWN);
	else if (tl_keywentdown("up")||tl_keywentdown("w"))
		takeTurn(UP);
	else if (tl_keywentdown("return") && plr.getCell()->getType() == STAIRS)
		canChangeFloor = true;
	else if (tl_keywentdown("q"))
		canChangeFloor = true;
}

void draw()
{
	Console::draw();
	mousePosition = cint(tl_mousex(),tl_mousey());
	int currX = 0;
	int currY = 0;
	for (int i = 0; i < tl_xres() - 6; i++)
	{
		for (int j = 0; j < tl_yres() - 2; j++)
		{
			currX = (plr.Pos().X() - ((tl_xres() - 6) / 2) + i);
			currY = (plr.Pos().Y() - ((tl_yres() - 2) / 2) + j);
			tl_color(0xFFFFFF00 + gameWorld.getCell(currX , currY)->Visibility());
			tl_rendertile(gameWorld.getTile(currX , currY), i, j);
			if(gameWorld.getCell(currX , currY)->hasPickup())
				tl_rendertile(gameWorld.getCell(currX , currY)->getPickup()->Tile(), i, j);
			if(gameWorld.getCell(currX , currY)->hasActor())
				tl_rendertile(gameWorld.getCell(currX , currY)->getActor()->Tile(), i, j);
			
			tl_color(0xFFFFFFFF);

			if(cint(i, j) == mousePosition)
			{
				if(gameWorld.getCell(currX , currY)->hasActor())
					gameWorld.getCell(currX , currY)->getActor()->describe();
				if(gameWorld.getCell(currX , currY)->hasPickup())
					gameWorld.getCell(currX , currY)->getPickup()->describe();
				if(gameWorld.getCell(currX , currY)->getType() == STAIRS)
					Console::quickLog("Press Enter on Stairs to Descend");
			}
		}
	}
	plr.printInventory(mousePosition);
	if (canChangeFloor)
	{
		Sound::play("staircase.sfs");
		gameWorld.nextFloor(&plr);		
	}
	canChangeFloor = false;
	if (gameWorld.Shop->Pos() == plr.Pos())
		Shop::render(mousePosition, &gameWorld.inventory);
	else
		Shop::inStore = false;
}

void menuUpdate()
{
	if (tl_buttonwentdown())
	{
		if (newGameHover)
		{
			onMenu = false;
		}
		if (loadGameHover)
		{
			try
			{
				load();
				onMenu = false;
			}
			catch(InvalidHeaderException& error)
			{
				Console::menuPrint("INVALID HEADER", (tl_xres() - 15) / 2, (tl_yres() / 2) + 6, 1, 0xFF0000FF);
				Console::menuPrint("FILE NOT LOADED", (tl_xres() - 15) / 2, (tl_yres() / 2) + 7, 1, 0xFF0000FF);
				onMenu = true;
			}
			catch(InvalidVersionException& error)
			{
				Console::menuPrint("INVALID VERSION", (tl_xres() - 15) / 2, (tl_yres() / 2) + 6, 1, 0xFF0000FF);
				Console::menuPrint("FILE NOT LOADED", (tl_xres() - 15) / 2, (tl_yres() / 2) + 7, 1, 0xFF0000FF);
				onMenu = true;
			}
		}
	}
	if (tl_keywentdown("escape"))
	{
		tl_shutdown();
	}
}

void menuDraw()
{
	newGameHover = false;
	loadGameHover = false;
	mousePosition = cint(tl_mousex(),tl_mousey());
	char** newGame = new char*;
	char** loadGame = new char*; 
	*newGame = " New Game";
	*loadGame = " Load Game";

	Console::menuPrint("ENDLESS DUNGEON 2", (tl_xres() - 17) / 2, (tl_yres() / 2) - 2, 1, 0xE80F00FF);
	Console::menuPrint("ELECTRIC BOOGALOO", (tl_xres() - 17) / 2, (tl_yres() / 2) - 1, 1, 0x00D9E8FF);

	if ((mousePosition.X() > (tl_xres() - 9) / 2) && (mousePosition.X() < (tl_xres() + 9) / 2))
	{
		if (mousePosition.Y() == (tl_yres() / 2) + 2)
		{
			*newGame = ">New Game";
			*loadGame = " Load Game";
			newGameHover = true;
		}
		else if (mousePosition.Y() == (tl_yres() / 2) + 4)
		{
			*newGame = " New Game";
			*loadGame = ">Load Game";
			loadGameHover = true;
		}
	}
	Console::menuPrint(*newGame, (tl_xres() - 9) / 2, (tl_yres() / 2) + 2, 1, 0xDEDEDEFF);
	Console::menuPrint(*loadGame, (tl_xres() - 9) / 2, (tl_yres() / 2) + 4, 1, 0xDEDEDEFF);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{
	_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF);
	tl_init("Endless Dungeon 2: Electric Boogaloo", 800, 600, "tiles", 32, 3);

	while (onMenu)
	{
		tl_framestart(0);
		menuUpdate();
		menuDraw();
	}

	//Now on to the game
	if (!loaded)
		gameWorld.initialize();
	plr = Player(gameWorld.getStart());
	plr.move(gameWorld.getStart(), gameWorld.getCell(plr.Pos().X(), plr.Pos().Y()));
	gameWorld.updateVisibility(&plr);

	while(true)
	{
		tl_framestart(0);
		update();
		draw();
	}
};