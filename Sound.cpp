#include "Sound.h"
#include "tilelib.h"

void Sound::play(char* fileName)
{
	tl_play(fileName);
}