#include "Console.h"
#include "tilelib.h"
vector<consoleLine> Console::chatLog;

bool Console::hover = false;
char* Console::mouseLog = 0;

void Console::quickLog(const char* line)
{
	mouseLog = new char[strlen(line)];
	strcpy(mouseLog, line);
}

void Console::log(char* line, int color)
{
	consoleLine log;
	log.message = line;
	log.color = color;
	Console::chatLog.push_back(log);
}

void Console::log(const char* line, int color)
{	
	char* newString = new char[strlen(line)];
	strcpy(newString, line);

	consoleLine log;
	log.message = newString;
	log.color = color;

	Console::chatLog.push_back(log);
}

void Console::print(char* string, int x, int y)
{
	tl_scale(2);
	for(unsigned int i = 0; i < strlen(string); i++)
	{
		tl_rendertile(string[i], x + i, y);
	}
	tl_scale(1);
}

void Console::print(const char* string, int x, int y)
{
	tl_scale(2);
	for(unsigned int i = 0; i < strlen(string); i++)
	{
		tl_rendertile(string[i], x + i, y);
	}
	tl_scale(1);
}

void Console::print(const char* string, int x, int y, int color)
{
	tl_color(color);
	Console::print(string, x, y);
	tl_color(0xFFFFFFFF);
}

void Console::menuPrint(const char* string, int x, int y, int scale, int color)
{
	tl_scale(scale);
	tl_color(color);
	for(unsigned int i = 0; i < strlen(string); i++)
	{
		tl_rendertile(string[i], x + i, y);
	}
	tl_color(0xFFFFFFFF);
	tl_scale(1);
}

void Console::menuPrint(char* string, int x, int y, int scale, int color)
{
	tl_scale(scale);
	tl_color(color);
	for(unsigned int i = 0; i < strlen(string); i++)
	{
		tl_rendertile(string[i], x + i, y);
	}
	tl_color(0xFFFFFFFF);
	tl_scale(1);
}

void Console::draw()
{
	int chatOffset = 0;
	if (mouseLog != 0)
	{
		tl_color(0xCD21C5FF);
		print(mouseLog, 0, (tl_yres() - 2) * 2);//(tl_xres() - 6) * 2
		tl_color(0xFFFFFFFF);
		mouseLog = 0;
		chatOffset = 1;
	}
	int length = Console::chatLog.size() - 1;
	if (length > -1)
	{
		tl_color(Console::chatLog.at(length).color);
		print(Console::chatLog.at(length).message, 0, (tl_yres() - 2) * 2 + chatOffset);
		for (int i = 1; i < 6 - chatOffset; i++)
		{
			
			if (length - i > -1)
			{
				tl_color((Console::chatLog.at(length - i).color & 0xFEFEFEFF) >> 1);
				print(Console::chatLog.at(length - i).message, 0, (tl_yres() - 2) * 2 + chatOffset + i);
			}
		}
		tl_color(0xFFFFFFFF);
	}
}