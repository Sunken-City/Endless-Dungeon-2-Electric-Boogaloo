#include "Serializer.h"

Serializer::Serializer(bool readingMode)
{
	if (readingMode)
		this->file = fopen("save.bin", "rb");
	else
		this->file = fopen("save.bin", "wb");
	this->readingMode = readingMode;
}

void Serializer::close()
{
	fclose(this->file);
}