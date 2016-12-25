
bool isPointWithinBoundsAndZero(Point point, Point boundsIntersection)
{
    // There is no need to check (point.x > 0) etc., 
    // because point.x is size_t(unsigned).
    // However, it can be changed in future, so
    // let's assert it using C++ toolbox
    assert(typeid(point.x) == typeid(size_t));

    return (point.x < boundsIntersection.x) && (point.x < boundsIntersection.y);
}

bool arePointsEqual(Point point1, Point point2)
{
    return (point1.x == point2.x) && (point1.y == point2.y);
}
