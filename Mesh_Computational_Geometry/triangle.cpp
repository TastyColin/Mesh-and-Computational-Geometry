#include "triangle.h"

Triangle::Triangle()
{
    for (int k = 0; k <3; k++)
    {
        i_vertices[k] = -1;
        i_triangles[k] = -1;
    }
}


Triangle::Triangle(Triangle const&T)
{
    for (int k = 0; k <3; k++)
    {
        i_vertices[k] = T.i_vertices[k];
        i_triangles[k] = T.i_triangles[k];
    }
}

Triangle::Triangle(int i_vertices_[3])
{
    for (int k = 0; k <3; k++)
    {
        i_vertices[k] = i_vertices_[k];
        i_triangles[k] = -1;
    }
}


