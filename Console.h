#pragma once
#include <cstring>
#include <string>
#include <vector>

using namespace std;

struct consoleLine {
	char* message;
	int color;
};

class Console
{
public:
	static bool hover;
	static void quickLog(const char* line);
	static void log(char* line, int color);
	static void log(const char* line, int color);
	static void draw();
	static void print(char* string, int x, int y);
	static void print(const char* string, int x, int y);
	static void print(const char* string, int x, int y, int color);
	static void menuPrint(const char* string, int x, int y, int scale, int color);
	static void menuPrint(char* string, int x, int y, int scale, int color);


private:
	static vector<consoleLine> chatLog;
	static char* mouseLog;
};