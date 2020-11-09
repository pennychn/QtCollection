#include "point.h"

Point::Point()
{
	m_nId = 0;
	x = y = z = 0;
	label = "unknow";
}

Point::Point(int id, unsigned int xValue, unsigned int yValue, 
		         unsigned int zValue)
{
	m_nId = id;
	x = xValue;
	y = yValue;
	z = zValue;
	label = "unknown";
}

Point::Point(unsigned int xValue, unsigned int yValue, 
		         unsigned int zValue)
{
	m_nId = 0;
	x = xValue;
	y = yValue;
	z = zValue;
	label = "unknown";
}

Point::Point(unsigned int xValue, unsigned int yValue, 
		         unsigned int zValue, char* name)
{
	m_nId = 0;
	x = xValue;
	y = yValue;
	z = zValue;
	label = name;
}
