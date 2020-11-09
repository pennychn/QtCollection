#ifndef POINT_H
#define POINT_H

#include <string>

class Point
{
	public:
		// constructors
		Point();
		Point(unsigned int xValue,unsigned int yValue,
				  unsigned int zValue);
		Point(unsigned int xValue,unsigned int yValue,
				  unsigned int zValue,char* name);
		~Point() { ; } 

		// modifiers
		inline void setX(unsigned int xValue) { x = xValue; }
		inline void setY(unsigned int yValue) { y = yValue; }
		inline void setZ(unsigned int zValue) { z = zValue; }

		// accessor
		inline unsigned int getX() const { return x; }
		inline unsigned int getY() const { return y; }
		inline unsigned int getZ() const { return z; }

	protected:
		unsigned int x, y, z;
		std::string label;
};

#endif // POINT_H
