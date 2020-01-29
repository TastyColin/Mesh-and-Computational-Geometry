#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "point.h"

class Triangle
{
public:
    Triangle();
    Triangle(Triangle const&T);
    Triangle(int i_vertices[3]);

    int i_vertices[3];
	int i_triangles[3];

    
};

#endif // TRIANGLE_H
