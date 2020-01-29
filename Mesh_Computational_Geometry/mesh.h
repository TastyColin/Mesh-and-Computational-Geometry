#ifndef MESH_H
#define MESH_H

#include <QGLWidget>
#include <string>
#include<stdio.h>
#include<stdlib.h>
#include <map>
#include <math.h>

#include "point.h"
#include "triangle.h"
#include "gradient.h"

using namespace std;

// TO MODIFY
// Advice => You should create a new file for each module but not necessarily for each class


struct Vertex
{
    Point point;
    int i_triangle = -1;
    Point vetor_value = Point(0,0,0);
};



// Iterators
class IteratorOnFaces;
class IteratorOnVertices;
class CirculatorOnFaces;
class CirculatorOnVertices;

// Mesh Class
class Mesh
{
    friend class IteratorOnFaces;
    friend class IteratorOnVertices;
    friend class CirculatorOnFaces;
    friend class CirculatorOnVertices;

public:
    Mesh();

    void drawMesh() const;
    void drawMeshWireFrame() const;
    void drawMeshCurvature();
    void drawMeshPointConnexions() const;
    void drawMeshTriangleConnexions() const;
    void drawMeshLaplacian();
    void drawMeshNormal();
    void loadMesh(const char file_name[]);

    IteratorOnFaces faces_begin();
    IteratorOnFaces faces_end();
    IteratorOnVertices vertices_begin();
    IteratorOnVertices vertices_end();

    CirculatorOnFaces faces_begin_circle(Vertex& vertex);
    CirculatorOnFaces faces_end_circle(Vertex& vertex);
    CirculatorOnVertices vertices_begin_circle(Vertex& vertex);
    CirculatorOnVertices vertices_end_circle(Vertex& vertex);


protected:
    // Attributs protégés
    int _nb_vertex;
    int _nb_triangle;
    QVector<Vertex> vertexTab;
    QVector<Triangle> triangleTab;
    bool _b_value_computed;


    // Méthodes protégées
    double _computeAeraTriangle(Triangle &T) const;
    double _computeCotangente(Triangle &T, int k) const;
    void _computeLaplacian(void);
    void _test_iterators(void);

    Gradient _my_gradient;
};




class IteratorOnFaces
{
public:
    IteratorOnFaces(Mesh& mesh, int pos = 0);
    IteratorOnFaces& operator++(void);
    bool operator==(IteratorOnFaces other);
    bool operator!=(IteratorOnFaces other);
    Triangle& operator*(void);
    Triangle* operator->(void);
protected:
    Mesh& _mesh;
    int _current_pos;
};

class CirculatorOnFaces
{
public:
    CirculatorOnFaces(Mesh& mesh, Vertex& vertex, int nb_lap = 0);
    CirculatorOnFaces& operator++(void);
    bool operator==(CirculatorOnFaces other);
    bool operator!=(CirculatorOnFaces other);
    Triangle& operator*(void);
    Triangle* operator->(void);
protected:
    Mesh& _mesh;
    Vertex& _vertex;
    int _current_i_face;
    int _nb_lap;
};


class IteratorOnVertices
{
public:
    IteratorOnVertices(Mesh& mesh, int pos = 0);
    IteratorOnVertices& operator++(void);
    IteratorOnVertices operator++(int);
    bool operator==(IteratorOnVertices other);
    bool operator!=(IteratorOnVertices other);
    Vertex& operator*(void);
    Vertex* operator->(void);
protected:
    Mesh& _mesh;
    int _current_pos;
};

class CirculatorOnVertices : public CirculatorOnFaces
{
public:
    CirculatorOnVertices(Mesh& mesh, Vertex& vertex, int nb_lap = 0);
    Vertex& operator*(void);
    Vertex* operator->(void);
};


#endif // MESH_H
