#include "point.h"

Point::Point()
{
	x = y = z = 0;
	label = "unknow";
}

Point::Point(unsigned int xValue, unsigned int yValue, 
		         unsigned int zValue)
{
	x = xValue;
	y = yValue;
	z = zValue;
	label = "unknown";
}

Point::Point(unsigned int xValue, unsigned int yValue, 
		         unsigned int zValue, char* name)
{
	x = xValue;
	y = yValue;
	z = zValue;
	label = name;
}
