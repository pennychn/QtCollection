#include "segment.h"

Segment::Segment()
{
	one = two = NULL;
	label = "unknown";
}

Segment::Segment(Point *a, Point *b)
{
	one = a;
	two = b;
	label = "unknown";
}


Segment::Segment(Point *a, Point *b, char *name)
{
	one = a;
	two = b;
	label = name;
}

