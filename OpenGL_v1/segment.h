#ifndef SEGMENT_H
#define SEGMENT_H

#include "Point.h"

class Segment
{
	public:
		// constructor
		Segment(); 
		Segment(Point* a, Point* b); 
		Segment(Point* a, Point* b, char * name); // <-- prefered constructor
		~Segment(); 

		// modifiers
		void setPointOne(int x, int y) { one->setX(x); one->setY(y); }
		void setPointTwo(int x, int y) { two->setX(x); two->setY(y); }
		void setPointOneByRef(Point* aPoint) { one = aPoint; }
		void setPointTwoByRef(Point* aPoint) { two = aPoint; }

		void setPointOne(Point* x) { one->setX(x->getX()); one->setY(x->getY()); }
		void setPointTwo(Point* x) { two->setX(x->getX()); two->setY(x->getY()); }
		void setPoints(Point* x, Point* y) { setPointOne(x); setPointTwo(y); }

		// accessor
		Point* PointOne() { return one; }
		Point* PointTwo() { return two; }

	private:
		Point* one;
		Point* two;
		std::string label;
};

#endif // SEGMENT_H
