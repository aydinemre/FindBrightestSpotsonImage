#include <iostream>
using namespace std;
#pragma once
class EAPoint
{
	//  Private members:
private:
	int x;			// x coordinate of brightest point
	int y;			// y coordinate of brightest point
	double value;	// pixel value.
	int r;			// Radius of circle.

public:
	// No parameter constructor.
	EAPoint();
	// Three parameter constructor.
	EAPoint(const int xCoordinate,const int yCoordinate,const double pixelValue);
	// Copy constructor.
	EAPoint(const EAPoint &newBrightestPoint);

	// Comparasion operator overloads.
	bool operator>(EAPoint &other);

	// Inline setters and getters for all private members.
	inline int getX() const { return x; }
	inline int getY() const  { return y; }
	inline double getValue() const { return value; }
	inline int getRadius() const { return r; }

	inline void setX(int newX) {
		if (newX < 0) throw ("IllegalArgumentException for x coordinate : ");
		x = newX;
	}

	inline void setY(int newY) {
		if (newY < 0) throw ("IllegalArgumentException for y coordinate : ");
		y = newY;
	}

	inline void setValue(double newValue) {
		if (newValue < 0 || newValue > 255)
			throw ("IllegalArgumentException for pixel value");
		else
			value = newValue;
	}

	void setRadius(int newRadius) {
		if (newRadius < 0 )
			throw ("IllegalArgumentException for radius value");
		if (newRadius > 100)
			r = 100;
		else if (newRadius < 10)
			r = 10;
		else
			r = newRadius;
	}
};
