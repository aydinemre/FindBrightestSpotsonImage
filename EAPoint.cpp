#include "EAPoint.h"

EAPoint::EAPoint()
{
	x = 0;
	y = 0;
	value = 0;
	r = 0;
}

EAPoint::EAPoint(const int xCoordinate, const int yCoordinate, const double pixelValue)
{
	setX(xCoordinate);
	setY(yCoordinate);
	setValue(pixelValue);
}

EAPoint::EAPoint(const EAPoint &newBrightestPoint)
{
	setX(newBrightestPoint.getX());
	setY(newBrightestPoint.getY());
	setValue(newBrightestPoint.getValue());
}

bool EAPoint::operator>(EAPoint &other)
{
	return (getValue() > other.getValue());
}

