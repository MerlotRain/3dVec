#ifndef SHAPE_H
#define SHAPE_H

#include <CGAL/Point_3.h>
#include <CGAL/Simple_cartesian.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point_3;

class Shape
{
};

class Point : public Shape
{
    Point_3 point;
};

#endif