#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include "/home/philipp/Documents/Ded/lib/common.h"

//Point struct

struct Point
{
    size_t x;
    size_t y;
};

bool isPointWithinBoundsAndZero(Point point, Point boundsIntersection);

bool arePointsEqual(Point point1, Point point2);

#include "point.cpp"

#endif //POINT_H_INCLUDED
