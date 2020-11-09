#include "rectangle.h"

Block::Block()
{
	// bl = tr = NULL;
}

Block::Block(Point* bottom_left, Point* top_right)
{
	bl = bottom_left;
	tr = top_right;
}

Block::~Block()
{
	delete bl;
	delete tr;
}
