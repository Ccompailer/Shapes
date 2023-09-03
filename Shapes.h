#include "Vector3D.h"
#include "Point.h"


class Shape
{
protected:

    Point3D center{};

    Shape() = default;
    Shape(const Point3D& point) : center(point) {}

public:

    virtual ~Shape() {}

    virtual Point3D curve_point(double t) const = 0;

    virtual Vector3D curve_vector(double t) const = 0;
};

#ifndef UNTITLED2_SHAPES_H
#define UNTITLED2_SHAPES_H

#endif //UNTITLED2_SHAPES_H
