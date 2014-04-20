#pragma once
#include "Settings.h"
#include "Serializeable.h"
#include <random>
#include <cmath>

//typedef cint Coord<int>;

template <typename T>
class Coord : public Serializeable
{
	private:
		T x;
		T y;

	public:

		Coord ()
		{
			x = 0;
			y = 0;
		}
		Coord (T inX, T inY)
		{
			x = inX;
			y = inY;
		}

		void serialize(Serializer write)
		{
			write.IO<int>(this->x);
			write.IO<int>(this->y);
		}

		void reconstruct(Serializer read)
		{
			read.IO<int>(this->x);
			read.IO<int>(this->y);
		}
		
		T  X()
		{
			return x;
		}

		T  Y()
		{
			return y;
		}

		void X(T inX)
		{
			x = inX;
		}

		void Y(T inY)
		{
			y = inY;
		}

		Coord<T> Next(int x, int y)
		{
			if ((this->X() + x <= 0)||(this->X() + x >= Settings::worldx -1))
				x = 0;
			if ((this->Y() + y <= 0)||(this->Y() + y >= Settings::worldy -1))
				y = 0;
			return Coord<T>(this->X() + x, this->Y() + y);
		}

		Coord<T> Next(Coord<T> diff)
		{
			return Next(diff.X(), diff.Y());
		}

		static Coord<T> randNext()
		{
			int next = (rand() % 4);
			if (next == 0)
				return Coord<T>(1,0);
			else if (next == 1)
				return Coord<T>(-1,0);
			else if (next == 2)
				return Coord<T>(0,-1);
			else
				return Coord<T>(0,1);
		}

		static T manhattan(Coord<T> first, Coord<T> second)
		{
			T xTiles = first.x - second.x;
			T yTiles = first.y - second.y;
			return abs(xTiles) + abs(yTiles);
		}

		static Coord<T> getDirection(Coord<T> first, Coord<T> second)
		{
			T xTiles = first.x - second.x;
			T yTiles = first.y - second.y;
			if (xTiles != 0)
				(xTiles > 0) ? xTiles = 1 : xTiles = -1;
			if (yTiles != 0)
				(yTiles > 0) ? yTiles = 1 : yTiles = -1;

			return Coord<T>(xTiles, yTiles);
		}

		Coord<T>& operator+= (Coord<T>& rhs)
		{
			//Addition of rhs to *this
			this->x += rhs->X();
			this->Y += rhs->Y();
			return *this;
		}

		bool operator== (Coord<T>& rhs)
		{
			//If the x and y are equivalent, return true.
			return ((this->X() == rhs.X())&&(this->Y() == rhs.Y()));
		}

		bool operator!= (Coord<T>& rhs)
		{
			//If the x and y are NOT equivalent, return true.
			return (!((*this) == rhs));
		}
};

typedef Coord<int> cint;

inline cint operator+(cint& lhs, cint&  rhs)
{
	int newX = lhs.X() + rhs.X();
	int newY = lhs.Y() + rhs.Y();
	return cint(newX, newY);
}

