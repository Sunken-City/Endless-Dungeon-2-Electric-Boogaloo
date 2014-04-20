#pragma once

#include "Serializer.h"
#include <stdexcept>

class Serializeable
{
public:
	static const int null = 0;
	static const int notNull = 1;
	virtual void serialize(Serializer write) = 0;
	//virtual void reconstruct(Serializer read) = 0;
};

class InvalidHeaderException : public std::runtime_error {
public:
  InvalidHeaderException() : std::runtime_error("InvalidHeaderException") { }
};

class InvalidVersionException : public std::runtime_error {
public:
  InvalidVersionException() : std::runtime_error("InvalidVersionException") { }
};

class TruncatedFileException : public std::runtime_error {
public:
  TruncatedFileException() : std::runtime_error("TruncatedFileException") { }
};

class ExtendedFileException : public std::runtime_error {
public:
  ExtendedFileException() : std::runtime_error("ExtendedFileException") { }
};