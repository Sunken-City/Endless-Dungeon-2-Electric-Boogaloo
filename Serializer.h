#pragma once
#include <cstdio>
#include <cstring>

class Serializer
{
public:
	Serializer(bool readingMode);
	template<class T> void IO(T& IOable);
	void close();
private:
	FILE* file;
	bool readingMode;
};

template<class T> void Serializer::IO(T& IOable)
{
	if (this->readingMode)
		fread(&IOable, sizeof(T), 1, this->file);
	else
		fwrite(&IOable, sizeof(T), 1, this->file);
}