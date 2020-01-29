#include "mesh.h"


Mesh::Mesh() : _my_gradient()
{

    char const file_name[] ("../tetrahedron.off");
    loadMesh(file_name);
}



// ========================================================================= //
// Chargement
// ========================================================================= //
void Mesh::loadMesh(const char file_name[])
{
    _b_value_computed = false;
    vertexTab.clear();
    triangleTab.clear();
    int nb_edge = 0;
    FILE *fp;
    fp = fopen(file_name, "r");  //Ouverture d'un fichier en lecture


    if(fp == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }
    printf("Testing fscanf() function: \n\n");
    fscanf(fp, "%d %d %d\n", &_nb_vertex, &_nb_triangle, &nb_edge);
    printf("Fichier chargé\nNb de sommets : %d\nNb de triangles : %d\n", _nb_vertex, _nb_triangle);

    double x, y, z;
    for (int i_vertex = 0; i_vertex < _nb_vertex; i_vertex++)
    {
        fscanf(fp, "%lf %lf %lf\n", &x, &y, &z);
        vertexTab.push_back(Vertex {Point(x,y,z)} );
    }
    int n_face, iA, iB, iC;
    int i_vertices[3];
    for (int i_triangle = 0; i_triangle < _nb_triangle; i_triangle++)
    {
        fscanf(fp, "%d %d %d %d\n", &n_face, &iA, &iB, &iC);
        i_vertices[0] = iA;
        i_vertices[1] = iB;
        i_vertices[2] = iC;
        triangleTab.push_back(Triangle(i_vertices));
    }
    fclose(fp);

    // Association des sommets vers un triangle
    bool b_vertex_associated[_nb_vertex];
    for (int i_vertex = 0; i_vertex < _nb_vertex; i_vertex++)
    {
        b_vertex_associated[i_vertex] = false;
    }
    for (int i_triangle = 0; i_triangle < _nb_triangle; i_triangle++)
    {
        for (int k = 0; k < 3; k++)
        {
            iA = triangleTab[i_triangle].i_vertices[k];
            if (! b_vertex_associated[iA])
            {
                b_vertex_associated[iA] = true;
                vertexTab[iA].i_triangle = i_triangle;
            }
        }
    }

    // Association des voisins des triangles
    map<pair<int,int>, pair<int,int>> association_triangles;
    int i_other_triangle, k_other;
    for (int i_triangle = 0; i_triangle < _nb_triangle; i_triangle++)
    {
        for (int k = 0; k < 3; k++)
        {
            iA = triangleTab[i_triangle].i_vertices[k];
            iB = triangleTab[i_triangle].i_vertices[(k+1)%3];
            if (association_triangles.count({iB, iA}) > 0)
            {
                i_other_triangle = association_triangles[{iB,iA}].first;
                k_other = association_triangles[{iB,iA}].second;
                triangleTab[i_triangle].i_triangles[(k+2)%3] = i_other_triangle;
                triangleTab[i_other_triangle].i_triangles[(k_other+2)%3] = i_triangle;

            }
            else
            {
                pair<int,int> key = {iA,iB};
                association_triangles[key] = pair<int,int> {i_triangle, k};
            }
        }
    }
    //_test_iterators();

}



// ========================================================================= //
// Méthodes d'affichage
// ========================================================================= //
void glVertexDraw(const Point & p) {
    glVertex3f(p.x(), p.y(), p.z());
}


void Mesh::drawMesh() const
{
    int i_color, iR, iG, iB;
    for(int i_triangle = 0; i_triangle < _nb_triangle; i_triangle++) {
        i_color = i_triangle % 6 +1;
        iR = i_color%2;
        iG = (i_color/2)%2;
        iB = (i_color/4)%2;
        glColor3d(iR, iG, iB);

        glBegin(GL_TRIANGLES);
        for (int k = 0; k < 3; k++)
        {
            glVertexDraw(vertexTab[triangleTab[i_triangle].i_vertices[k]].point);
        }
        glEnd();
    }
}


void Mesh::drawMeshWireFrame() const
{
    glColor3d(0,1,0);
    for(int i_triangle = 0; i_triangle < _nb_triangle; i_triangle++) {
        for (int k_line = 0; k_line < 3; k_line++)
        {
            glBegin(GL_LINE_STRIP);
            glVertexDraw(vertexTab[triangleTab[i_triangle].i_vertices[k_line]].point);
            glVertexDraw(vertexTab[triangleTab[i_triangle].i_vertices[(k_line+1)%3]].point);
            glEnd();
        }
    }
}

void Mesh::drawMeshCurvature()
{
    if (!_b_value_computed)
    {
        _computeLaplacian();
        _b_value_computed = true;
    }
    double seuil = 0;
    for(int i_vertex = 0; i_vertex < _nb_vertex; i_vertex++) {
        double value = abs(1./vertexTab[i_vertex].vetor_value.computeNorm());
        if (value > seuil) {seuil = value;}
    }
    seuil = 0.1;
    double iR, iG, iB;
    for(int i_triangle = 0; i_triangle < _nb_triangle; i_triangle++) {
        Point U = vertexTab[triangleTab[i_triangle].i_vertices[1]].point - vertexTab[triangleTab[i_triangle].i_vertices[0]].point;
        Point V = vertexTab[triangleTab[i_triangle].i_vertices[2]].point - vertexTab[triangleTab[i_triangle].i_vertices[0]].point;
        Point normal_dir = prod(U,V);
        double value;
        glBegin(GL_TRIANGLES);
        for (int k = 0; k < 3; k++)
        {
            int i_vertex = triangleTab[i_triangle].i_vertices[k];
            if (dot(normal_dir, vertexTab[i_vertex].vetor_value) > 0)
            {
                value = 1/vertexTab[i_vertex].vetor_value.computeNorm()/seuil;
            }
            else
            {
                value = -1/vertexTab[i_vertex].vetor_value.computeNorm()/seuil;
            }
            value = max(min(1.,value),-1.);
            value = value/2+0.5;
            iR = GradientRayon::red(value);
            iG = GradientRayon::green(value);
            iB = GradientRayon::blue(value);
            glColor3d(iR, iG, iB);
            glVertexDraw(vertexTab[i_vertex].point);
        }
        glEnd();
    }
}


void Mesh::drawMeshPointConnexions() const
{
    glColor3d(0,1,1);
    for(int i_vertex = 0; i_vertex < _nb_vertex; i_vertex++) {
        Point triangle_center;
        for (int k_point = 0; k_point < 3; k_point++)
        {
            triangle_center += vertexTab[triangleTab[vertexTab[i_vertex].i_triangle].i_vertices[k_point]].point;
        }
        glBegin(GL_LINE_STRIP);
        glVertexDraw(vertexTab[i_vertex].point);
        glVertexDraw(triangle_center/3.);
        glEnd();
    }
}


void Mesh::drawMeshTriangleConnexions() const
{
    glColor3d(1,0,0.5);
    for(int i_triangle = 0; i_triangle < _nb_triangle; i_triangle++) {
        Point triangle_center;
        for (int k_point = 0; k_point < 3; k_point++)
        {
            triangle_center += vertexTab[triangleTab[i_triangle].i_vertices[k_point]].point;
        }
        triangle_center /= 3.;


        for (int k_edge = 0; k_edge < 3; k_edge++)
        {
            Point other_triangle_center;
            int i_other_triangle_center = triangleTab[i_triangle].i_triangles[k_edge];
            for (int k_point = 0; k_point < 3; k_point++)
            {
                other_triangle_center += vertexTab[triangleTab[i_other_triangle_center].i_vertices[k_point]].point;
            }
            other_triangle_center /= 3.;
            glBegin(GL_LINE_STRIP);
            glVertexDraw(triangle_center);
            glVertexDraw(2./3.*triangle_center + other_triangle_center/3);
            glEnd();
        }
    }
}


void Mesh::drawMeshLaplacian()
{
    if (!_b_value_computed)
    {
        _computeLaplacian();
        _b_value_computed = true;
    }

    glColor3d(1,1,1);
    for(int i_vertex = 0; i_vertex < _nb_vertex; i_vertex++) {
        glBegin(GL_LINE_STRIP);
        glVertexDraw(vertexTab[i_vertex].point);
        glVertexDraw(vertexTab[i_vertex].point + vertexTab[i_vertex].vetor_value);
        glEnd();
    }
}

void Mesh::drawMeshNormal()
{
    if (!_b_value_computed)
    {
        _computeLaplacian();
        _b_value_computed = true;
    }

    glColor3d(1,1,1);
    for(int i_vertex = 0; i_vertex < _nb_vertex; ++i_vertex) {
        Point normal = vertexTab[i_vertex].vetor_value/vertexTab[i_vertex].vetor_value.computeNorm();
        Point U = vertexTab[triangleTab[vertexTab[i_vertex].i_triangle].i_vertices[1]].point - vertexTab[triangleTab[vertexTab[i_vertex].i_triangle].i_vertices[0]].point;
        Point V = vertexTab[triangleTab[vertexTab[i_vertex].i_triangle].i_vertices[2]].point - vertexTab[triangleTab[vertexTab[i_vertex].i_triangle].i_vertices[0]].point;
        if ( dot(normal, prod(U,V)) < 0) {
            normal *= -1;
        }
        glBegin(GL_LINE_STRIP);
        glVertexDraw(vertexTab[i_vertex].point);
        glVertexDraw(vertexTab[i_vertex].point + normal);
        glEnd();
    }
}


void Mesh::_test_iterators(void)
{
    int c =0;
    for (IteratorOnVertices it = vertices_begin(); it != vertices_end(); ++it)
    {
        printf("Sommets: %d\n",c);
        for(CirculatorOnFaces cf = faces_begin_circle(*it); cf != faces_end_circle(*it); ++cf)
        {
            printf("Face: %d %d %d\n",(*cf).i_vertices[0], (*cf).i_vertices[1], (*cf).i_vertices[2]);
        }
        printf("\n");
        ++c;
    }
    c=0;
    for (IteratorOnVertices it = vertices_begin(); it != vertices_end(); ++it)
    {
        printf("Sommets: %d\n",c);
        for(CirculatorOnVertices cv = vertices_begin_circle(*it); cv != vertices_end_circle(*it); ++cv)
        {
            printf("Voisin: (%lf, %lf, %lf)\n",(*cv).point.x(), (*cv).point.y(), (*cv).point.z());
        }
        printf("\n");
        ++c;
    }
}



// ========================================================================= //
// Calculs
// ========================================================================= //

double Mesh::_computeAeraTriangle(Triangle &T) const
{
    Point U(vertexTab[T.i_vertices[1]].point - vertexTab[T.i_vertices[0]].point);
    Point V(vertexTab[T.i_vertices[2]].point - vertexTab[T.i_vertices[0]].point);
    return prod(U,V).computeNorm()/2;
}

double Mesh::_computeCotangente(Triangle &T, int k) const
{
    Point U(vertexTab[T.i_vertices[(k+1)%3]].point - vertexTab[T.i_vertices[k]].point);
    Point V(vertexTab[T.i_vertices[(k+2)%3]].point - vertexTab[T.i_vertices[k]].point);
    double x = dot(U,V)/(U.computeNorm()*V.computeNorm());
    return x/sqrt(1-x*x);
}


void Mesh::_computeLaplacian(void)
{
    int k_vertex = 0;
    for (IteratorOnVertices it_vertex = vertices_begin(); it_vertex != vertices_end(); ++it_vertex)
    {
        double A_i = 0;
        Point U_i = it_vertex->point;
        for (CirculatorOnFaces cir_face = faces_begin_circle(*it_vertex); cir_face != faces_end_circle(*it_vertex); ++cir_face)
        {
            for (int k = 0; k < 3; k++)
            {
                if (&vertexTab[cir_face->i_vertices[k]] == &(*it_vertex))
                {
                    k_vertex = k;
                }
            }
            Point U_j1 = vertexTab[cir_face->i_vertices[(k_vertex+1)%3]].point;
            Point U_j2 = vertexTab[cir_face->i_vertices[(k_vertex+2)%3]].point;
            double cot_1 = _computeCotangente(*cir_face, (k_vertex+2)%3);
            double cot_2 = _computeCotangente(*cir_face, (k_vertex+1)%3);
            it_vertex->vetor_value += cot_1* (U_j1-U_i) + cot_2* (U_j2-U_i);
            A_i += _computeAeraTriangle(*cir_face)/3.;
        }
        it_vertex->vetor_value /= (2*A_i);
    }
}



// ========================================================================= //
// Itérateurs et circulateurs
// ========================================================================= //

// Méthode de Mesh
IteratorOnFaces Mesh::faces_begin() { return IteratorOnFaces( *this); }
IteratorOnFaces Mesh::faces_end() { return IteratorOnFaces( *this, _nb_triangle); }

CirculatorOnFaces Mesh::faces_begin_circle(Vertex& vertex) { return CirculatorOnFaces( *this, vertex); }
CirculatorOnFaces Mesh::faces_end_circle(Vertex& vertex) { return CirculatorOnFaces( *this, vertex, 1); }

IteratorOnVertices Mesh::vertices_begin() { return IteratorOnVertices( *this); }
IteratorOnVertices Mesh::vertices_end() { return IteratorOnVertices( *this, _nb_vertex); }

CirculatorOnVertices Mesh::vertices_begin_circle(Vertex& vertex) { return CirculatorOnVertices( *this, vertex); }
CirculatorOnVertices Mesh::vertices_end_circle(Vertex& vertex) { return CirculatorOnVertices( *this, vertex, 1); }


// Itérateurs sur les faces :
IteratorOnFaces::IteratorOnFaces(Mesh& mesh, int current_pos): _mesh(mesh), _current_pos(current_pos)
{
}

IteratorOnFaces& IteratorOnFaces::operator++(void)
{
    ++_current_pos;
    return (*this);
}

bool IteratorOnFaces::operator==(IteratorOnFaces other)
{
    return (&_mesh == &other._mesh) && (_current_pos == other._current_pos);
}

bool IteratorOnFaces::operator!=(IteratorOnFaces other)
{
    return !(*this==other);
}

Triangle& IteratorOnFaces::operator*(void)
{
    return _mesh.triangleTab[_current_pos];
}

Triangle* IteratorOnFaces::operator->(void)
{
    return &_mesh.triangleTab[_current_pos];
}

// Circulateur sur les faces
CirculatorOnFaces::CirculatorOnFaces(Mesh& mesh, Vertex& vertex, int nb_lap):
    _mesh(mesh), _vertex(vertex), _current_i_face(vertex.i_triangle), _nb_lap(nb_lap)
{

}

CirculatorOnFaces& CirculatorOnFaces::operator++(void)
{
    for (int k_vertex = 0; k_vertex < 3; k_vertex++)
    {
        if (&_mesh.vertexTab[_mesh.triangleTab[_current_i_face].i_vertices[k_vertex]] == &_vertex)
        {
            _current_i_face = _mesh.triangleTab[_current_i_face].i_triangles[(k_vertex+1)%3];
            break;
        }
    }
    if (_current_i_face == _vertex.i_triangle) {++_nb_lap;}
    return *(this);
}

bool CirculatorOnFaces::operator==(CirculatorOnFaces other)
{
    return (&_mesh == &other._mesh)
            && (&_vertex == &other._vertex)
            && (_current_i_face == other._current_i_face)
            && (_nb_lap == other._nb_lap);
}

bool CirculatorOnFaces::operator!=(CirculatorOnFaces other)
{
    return !(*this==other);
}

Triangle& CirculatorOnFaces::operator*(void)
{
    return _mesh.triangleTab[_current_i_face];
}

Triangle* CirculatorOnFaces::operator->(void)
{
    return &_mesh.triangleTab[_current_i_face];
}



// Itérateurs sur les sommets :
IteratorOnVertices::IteratorOnVertices(Mesh& mesh, int current_pos): _mesh(mesh), _current_pos(current_pos)
{
}


IteratorOnVertices& IteratorOnVertices::operator++(void) //prefixe
{
    ++_current_pos;
    return (*this);
}

IteratorOnVertices IteratorOnVertices::operator++(int) //postfixe qu'on evite d'utiliser
{
    IteratorOnVertices clone(*this);
    ++_current_pos;
    return clone;
}

bool IteratorOnVertices::operator==(IteratorOnVertices other)
{
    return (&_mesh == &other._mesh) && (_current_pos == other._current_pos);
}

bool IteratorOnVertices::operator!=(IteratorOnVertices other)
{
    return !(*this==other);
}

Vertex& IteratorOnVertices::operator*(void)
{
    return _mesh.vertexTab[_current_pos];
}

Vertex* IteratorOnVertices::operator->(void)
{
    return &_mesh.vertexTab[_current_pos];
}


// Circulateur sur les sommets
CirculatorOnVertices::CirculatorOnVertices(Mesh& mesh, Vertex& vertex, int nb_lap):
    CirculatorOnFaces(mesh, vertex, nb_lap)
{
}

Vertex& CirculatorOnVertices::operator*(void)
{
    for (int k_vertex = 0; k_vertex < 3; k_vertex++)
    {
        if (&_mesh.vertexTab[_mesh.triangleTab[_current_i_face].i_vertices[k_vertex]] == &_vertex)
        {
            return _mesh.vertexTab[_mesh.triangleTab[_current_i_face].i_vertices[(k_vertex+1)%3]];
        }
    }
}

Vertex* CirculatorOnVertices::operator->(void)
{
    for (int k_vertex = 0; k_vertex < 3; k_vertex++)
    {
        if (&_mesh.vertexTab[_mesh.triangleTab[_current_i_face].i_vertices[k_vertex]] == &_vertex)
        {
            return &_mesh.vertexTab[_mesh.triangleTab[_current_i_face].i_vertices[(k_vertex+1)%3]];
        }
    }
}

