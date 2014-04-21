#pragma once

#include "Serializer.h"
#include <stdexcept>

class Serializeable
{
public:
	static int null;
	static int notNull;
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