#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "point.h"

class Block
{
	public:
		Block(); 
		Block(Point* bottom_left, Point* top_right);
		// based class must be virtual destructor
		~Block();

		// accessor
		inline int getWidth() const { return (tr->getX() - bl->getX()); }
		inline int getHeight() const { return (tr->getY() - bl->getY()); }
		inline int getBLX() const { return bl->getX(); }
		inline int getBLY() const { return bl->getY(); }
		inline int getTRX() const { return tr->getX(); }
		inline int getTRY() const { return tr->getY(); }
		inline int getCenterX() const { return (bl->getX() + tr->getX()) / 2; } 
		inline int getCenterY() const { return (bl->getY() + tr->getY()) / 2; }
		inline Point* getBLP() { return bl; }
		inline Point* getTRP() { return tr; }

	protected:
		Point* bl; // bottom_left
		Point* tr; // top_right
};

#endif // RECTANGLE_H
