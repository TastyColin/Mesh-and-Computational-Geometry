#include "mesh.h"


Mesh::Mesh() : _my_gradient()
{
    char const file_name[] ("../tetrahedron.off");
    loadMesh(file_name);
}
void delay(int mtime)
{
    QTime dieTime= QTime::currentTime().addSecs(mtime / 1000.);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


// ========================================================================= //
// Chargement
// ========================================================================= //
void Mesh::loadMesh(const char file_name[])
{
    _b_value_computed = false;
    _b_triangle_centers_computed = false;
    _vertex_tab.clear();
    _triangle_tab.clear();
    int nb_edge = 0;
    FILE *fp;
    printf("%%=======================================%%\nOuverture du ficher: %s\n", file_name);
    fp = fopen(file_name, "r");  //Ouverture d'un fichier en lecture


    if(fp == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }
    fscanf(fp, "%d %d %d\n", &_nb_vertex, &_nb_triangle, &nb_edge);
    printf("Fichier charge\nNb de sommets : %d\nNb de triangles : %d\n", _nb_vertex, _nb_triangle);

    double x, y, z;
    for (int i_vertex = 0; i_vertex < _nb_vertex; i_vertex++)
    {
        fscanf(fp, "%lf %lf %lf\n", &x, &y, &z);
        _vertex_tab.push_back(Vertex {Point(x,y,z)} );
    }
    int n_face, iA, iB, iC;
    int i_vertices[3];
    for (int i_triangle = 0; i_triangle < _nb_triangle; i_triangle++)
    {
        fscanf(fp, "%d %d %d %d\n", &n_face, &iA, &iB, &iC);
        i_vertices[0] = iA;
        i_vertices[1] = iB;
        i_vertices[2] = iC;
        _triangle_tab.push_back(Triangle(i_vertices));
    }
    fclose(fp);

    // Association des sommets vers un triangle
    bool b_vertex_associated[_nb_vertex];
    for (int i_vertex = 0; i_vertex < _nb_vertex; i_vertex++)
    {
        b_vertex_associated[i_vertex] = false;
    }
    int nb_associated_vertex = 0;
    for (int i_triangle = 0; i_triangle < _nb_triangle; i_triangle++)
    {
        for (int k = 0; k < 3; k++)
        {
            iA = _triangle_tab[i_triangle].i_vertices[k];
            if (! b_vertex_associated[iA])
            {
                b_vertex_associated[iA] = true;
                nb_associated_vertex += 1;
                _vertex_tab[iA].i_triangle = i_triangle;
            }
        }
    }
    printf("%d/%d sommets associes a un triangle\nSoit %d sommet(s) non associe(s)\n", nb_associated_vertex, _nb_vertex, _nb_vertex-nb_associated_vertex );

    // Association des voisins des triangles
    map<pair<int,int>, pair<int,int>> association_triangles;
    int i_other_triangle, k_other;
    for (int i_triangle = 0; i_triangle < _nb_triangle; i_triangle++)
    {
        for (int k = 0; k < 3; k++)
        {
            iA = _triangle_tab[i_triangle].i_vertices[k];
            iB = _triangle_tab[i_triangle].i_vertices[(k+1)%3];
            if (association_triangles.count({iB, iA}) > 0)
            {
                i_other_triangle = association_triangles[{iB,iA}].first;
                k_other = association_triangles[{iB,iA}].second;
                _triangle_tab[i_triangle].i_triangles[(k+2)%3] = i_other_triangle;
                _triangle_tab[i_other_triangle].i_triangles[(k_other+2)%3] = i_triangle;

            }
            else
            {
                pair<int,int> key = {iA,iB};
                association_triangles[key] = pair<int,int> {i_triangle, k};
            }
        }
    }
    printf("%%=======================================%%\n\n");
}


// ========================================================================= //
// Complétion du fichier
// ========================================================================= //
void Mesh::addVertex(int nb_vertices)
{
    _b_value_computed = false;
    _b_triangle_centers_computed = false;
    _b_step_by_step = (nb_vertices==1);
    for (int i_vertex = 0; i_vertex < _nb_vertex; ++i_vertex)
    {
        if (_vertex_tab[i_vertex].i_triangle == -1)
        {
            if (_b_step_by_step)
                i_target = i_vertex;
            _addVertexToMesh(i_vertex);
            nb_vertices -= 1;
            if (nb_vertices == 0)
            {
                break;
            }
        }
    }
    _b_step_by_step = false;
}

template<typename T>
void pop_front(std::vector<T>& vec)
{
    assert(!vec.empty());
    vec.front() = std::move(vec.back());
    vec.pop_back();
}


void Mesh::_addVertexToMesh(const int &i_vertex)
{
    std::vector<Edge> edges_to_test;
    int i_triangle = _findTriangle(i_vertex);
    _splitTriangle(i_triangle, i_vertex, edges_to_test);
    while (!edges_to_test.empty())
    {
        Edge e = edges_to_test.back();
        if (_b_step_by_step)
            _i_path_tab.push_back(e.i_triangle);
        if (_toFlipEdgeTest(e))
        {
            if (_b_step_by_step)
                delay(1000);
            _edgeFlip(e.i_triangle, e.k_edge, edges_to_test);
        }
        edges_to_test.pop_back();
    }
    if (_b_step_by_step)
    {
        delay(3000);
        _i_path_tab.clear();
    }
}

int Mesh::_findTriangle(const int& i_vertex)
// Visibility walk
{
    const Point& M = _vertex_tab[i_vertex].point;
    int i_triangle = 0;
    int k_failure;
    while (true)
    {
        if (_b_step_by_step)
        {
            _i_path_tab.push_back(i_triangle);
            delay(1000);
        }

        bool found = true;
        for (int k = 0; k < 3; ++k)
            if (!_orientationTest(
                        _vertex_tab[_triangle_tab[i_triangle].i_vertices[k]].point,
                        _vertex_tab[_triangle_tab[i_triangle].i_vertices[(k+1)%3]].point,
                        M))
            {
                k_failure = k;
                found = false;
            }
        if (found)
        {
            if (_b_step_by_step)
            {
                delay(1000);
                _i_path_tab.clear();
            }
            return i_triangle;
        }
        i_triangle = _triangle_tab[i_triangle].i_triangles[(k_failure+2)%3];
    }
}




// ========================================================================= //
// Méthodes d'affichage
// ========================================================================= //
void glVertexDraw(const Point & p) {
    glVertex3f(p.x(), p.y(), p.z());
}


void Mesh::drawMesh() const
{
    float alpha = 0.5;
    float beta = 1.;
    int i_color;
    float iR, iG, iB;
    for(int i_triangle = 0; i_triangle < _nb_triangle; i_triangle++) {
        if (_b_step_by_step && _i_path_tab.contains(i_triangle))
        {
            alpha = 0.95;
            beta = 1.;
        }
        else if (_b_step_by_step)
        {
            alpha = 0.5;
            beta = 0.5;
        }
        else
        {
            alpha = 0.4;
            beta = 1.;
        }
        i_color = i_triangle % 6 +1;
        iR = i_color%2;
        iG = (i_color/2)%2;
        iB = (i_color/4)%2;
        iR = beta*((1-alpha)*iR + alpha);
        iG = beta*((1-alpha)*iG + alpha);
        iB = beta*((1-alpha)*iB + alpha);
        glColor3d(iR, iG, iB);

        glBegin(GL_TRIANGLES);
        for (int k = 0; k < 3; k++)
        {
            glVertexDraw(_vertex_tab[_triangle_tab[i_triangle].i_vertices[k]].point);
        }
        glEnd();
    }
    glColor3d(0.,0.,0.);
    glLineWidth(3.);
    for(int i_triangle = 0; i_triangle < _nb_triangle; i_triangle++) {
        for (int k_line = 0; k_line < 3; k_line++)
        {
            glBegin(GL_LINE_STRIP);
            glVertexDraw(_vertex_tab[_triangle_tab[i_triangle].i_vertices[k_line]].point);
            glVertexDraw(_vertex_tab[_triangle_tab[i_triangle].i_vertices[(k_line+1)%3]].point);
            glEnd();
        }
    }
}


void Mesh::drawMeshWireFrame() const
{
    glColor3d(0,1,0);
    glLineWidth(1.);
    for(int i_triangle = 0; i_triangle < _nb_triangle; i_triangle++) {
        for (int k_line = 0; k_line < 3; k_line++)
        {
            glBegin(GL_LINE_STRIP);
            glVertexDraw(_vertex_tab[_triangle_tab[i_triangle].i_vertices[k_line]].point);
            glVertexDraw(_vertex_tab[_triangle_tab[i_triangle].i_vertices[(k_line+1)%3]].point);
            glEnd();
        }
    }
}


void Mesh::drawMeshPoints() const
{
    for (int i_point = 0; i_point < _nb_vertex; ++i_point)
    {
        if (_vertex_tab[i_point].i_triangle != -1)
        {
            glColor3d(0,0.5,0.2);
            glPointSize(10.);
            glBegin(GL_POINTS);
            glVertex3f(_vertex_tab[i_point].point[0], _vertex_tab[i_point].point[1], _vertex_tab[i_point].point[2]);
            glEnd();
        } else if (i_target == i_point){
            glColor3d(1.,1.,1.);
            glPointSize(10.);
            glBegin(GL_POINTS);
            glVertex3f(_vertex_tab[i_point].point[0], _vertex_tab[i_point].point[1], _vertex_tab[i_point].point[2]);
            glEnd();
        }
        else
        {
            glColor3d(0.5,0,0);
            glPointSize(5.);
            glBegin(GL_POINTS);
            glVertex3f(_vertex_tab[i_point].point[0], _vertex_tab[i_point].point[1], _vertex_tab[i_point].point[2]);
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
    for(int i_vertex = 0; i_vertex < _nb_vertex; i_vertex++)
    {
        if (_vertex_tab[i_vertex].i_triangle != -1)
        {
            double value = abs(1./_vertex_tab[i_vertex].vector_value.computeNorm());
            if (value > seuil) {seuil = value;}
        }
    }
    seuil = 0.1;
    double iR, iG, iB;
    for(int i_triangle = 0; i_triangle < _nb_triangle; i_triangle++) {
        Point U = _vertex_tab[_triangle_tab[i_triangle].i_vertices[1]].point - _vertex_tab[_triangle_tab[i_triangle].i_vertices[0]].point;
        Point V = _vertex_tab[_triangle_tab[i_triangle].i_vertices[2]].point - _vertex_tab[_triangle_tab[i_triangle].i_vertices[0]].point;
        Point normal_dir = prod(U,V);
        double value;
        glBegin(GL_TRIANGLES);
        for (int k = 0; k < 3; k++)
        {
            int i_vertex = _triangle_tab[i_triangle].i_vertices[k];
            if (dot(normal_dir, _vertex_tab[i_vertex].vector_value) > 0)
            {
                value = 1/_vertex_tab[i_vertex].vector_value.computeNorm()/seuil;
            }
            else
            {
                value = -1/_vertex_tab[i_vertex].vector_value.computeNorm()/seuil;
            }
            value = max(min(1.,value),-1.);
            value = value/2+0.5;
            iR = GradientRayon::red(value);
            iG = GradientRayon::green(value);
            iB = GradientRayon::blue(value);
            glColor3d(iR, iG, iB);
            glVertexDraw(_vertex_tab[i_vertex].point);
        }
        glEnd();
    }
}


void Mesh::drawMeshVoronoi()
{
    if (!_b_triangle_centers_computed)
        _computeTriangleCenters();
    glColor3d(1,0,0);
    Point previous_center, center;
    for (IteratorOnVertices it_vertex = vertices_begin(); it_vertex != vertices_end(); ++it_vertex)
    {
        if (it_vertex->i_triangle != -1)
        {
            CirculatorOnFaces cir_face = faces_begin_circle(*it_vertex);
            previous_center = _triange_center_tab[cir_face->i_center];
            ++cir_face;
            for (; cir_face != faces_end_circle(*it_vertex); ++cir_face)
            {
                center = _triange_center_tab[cir_face->i_center];
                glBegin(GL_LINE_STRIP);
                glVertexDraw(previous_center);
                glVertexDraw(center);
                glEnd();
                previous_center = center;
            }
            center = _triange_center_tab[cir_face->i_center];
            glBegin(GL_LINE_STRIP);
            glVertexDraw(previous_center);
            glVertexDraw(center);
            glEnd();
        }
    }
}


void Mesh::drawMeshPointConnexions() const
{
    glColor3d(0,1,1);
    for(int i_vertex = 0; i_vertex < _nb_vertex; i_vertex++) {
        if (_vertex_tab[i_vertex].i_triangle != -1)
        {
            Point triangle_center;
            for (int k_point = 0; k_point < 3; k_point++)
            {
                triangle_center += _vertex_tab[_triangle_tab[_vertex_tab[i_vertex].i_triangle].i_vertices[k_point]].point;
            }
            glBegin(GL_LINE_STRIP);
            glVertexDraw(_vertex_tab[i_vertex].point);
            glVertexDraw(triangle_center/3.);
            glEnd();
        }
    }
}


void Mesh::drawMeshTriangleConnexions() const
{
    glColor3d(1,0,0.5);
    for(int i_triangle = 0; i_triangle < _nb_triangle; i_triangle++) {
        Point triangle_center;
        for (int k_point = 0; k_point < 3; k_point++)
        {
            triangle_center += _vertex_tab[_triangle_tab[i_triangle].i_vertices[k_point]].point;
        }
        triangle_center /= 3.;


        for (int k_edge = 0; k_edge < 3; k_edge++)
        {
            Point other_triangle_center;
            int i_other_triangle_center = _triangle_tab[i_triangle].i_triangles[k_edge];
            for (int k_point = 0; k_point < 3; k_point++)
            {
                other_triangle_center += _vertex_tab[_triangle_tab[i_other_triangle_center].i_vertices[k_point]].point;
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
        glVertexDraw(_vertex_tab[i_vertex].point);
        glVertexDraw(_vertex_tab[i_vertex].point + _vertex_tab[i_vertex].vector_value);
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

    glColor3d(0,0,0.7);
    for(int i_vertex = 0; i_vertex < _nb_vertex; ++i_vertex) {
        if (_vertex_tab[i_vertex].i_triangle != -1)
        {
            Point normal = _vertex_tab[i_vertex].vector_value/_vertex_tab[i_vertex].vector_value.computeNorm();
            Point U = _vertex_tab[_triangle_tab[_vertex_tab[i_vertex].i_triangle].i_vertices[1]].point - _vertex_tab[_triangle_tab[_vertex_tab[i_vertex].i_triangle].i_vertices[0]].point;
            Point V = _vertex_tab[_triangle_tab[_vertex_tab[i_vertex].i_triangle].i_vertices[2]].point - _vertex_tab[_triangle_tab[_vertex_tab[i_vertex].i_triangle].i_vertices[0]].point;
            if ( dot(normal, prod(U,V)) < 0) {
                normal *= -1;
            }
            glBegin(GL_LINE_STRIP);
            glVertexDraw(_vertex_tab[i_vertex].point);
            glVertexDraw(_vertex_tab[i_vertex].point + normal );
            glEnd();
        }
    }
}



// ========================================================================= //
// Calculs
// ========================================================================= //

// Normales et laplacien
double Mesh::_computeAeraTriangle(const Triangle &T) const
{
    Point U(_vertex_tab[T.i_vertices[1]].point - _vertex_tab[T.i_vertices[0]].point);
    Point V(_vertex_tab[T.i_vertices[2]].point - _vertex_tab[T.i_vertices[0]].point);
    return prod(U,V).computeNorm()/2;
}

double Mesh::_computeCotangente(const Triangle &T, const int& k) const
{
    Point U(_vertex_tab[T.i_vertices[(k+1)%3]].point - _vertex_tab[T.i_vertices[k]].point);
    Point V(_vertex_tab[T.i_vertices[(k+2)%3]].point - _vertex_tab[T.i_vertices[k]].point);
    double x = dot(U,V)/(U.computeNorm()*V.computeNorm());
    return x/sqrt(1-x*x);
}


void Mesh::_computeLaplacian(void)
{
    int k_vertex = 0;
    for (IteratorOnVertices it_vertex = vertices_begin(); it_vertex != vertices_end(); ++it_vertex)
    {
        if (it_vertex->i_triangle != -1)    // Si le sommet est associé à un triangle
        {
            double A_i = 0;
            Point U_i = it_vertex->point;
            for (CirculatorOnFaces cir_face = faces_begin_circle(*it_vertex); cir_face != faces_end_circle(*it_vertex); ++cir_face)
            {
                for (int k = 0; k < 3; k++)
                {
                    if (&_vertex_tab[cir_face->i_vertices[k]] == &(*it_vertex))
                    {
                        k_vertex = k;
                    }
                }
                Point U_j1 = _vertex_tab[cir_face->i_vertices[(k_vertex+1)%3]].point;
                Point U_j2 = _vertex_tab[cir_face->i_vertices[(k_vertex+2)%3]].point;
                double cot_1 = _computeCotangente(*cir_face, (k_vertex+2)%3);
                double cot_2 = _computeCotangente(*cir_face, (k_vertex+1)%3);
                it_vertex->vector_value += cot_1* (U_j1-U_i) + cot_2* (U_j2-U_i);
                A_i += _computeAeraTriangle(*cir_face)/3.;
            }
            it_vertex->vector_value /= (2*A_i);
        }
    }
}

// Split triangle
void Mesh::_splitTriangle(const int &i_triangle, const int &i_vertex, std::vector<Edge>& edges_to_test)
{
    // Nouveaux triangles ajoutés et leur indice
    int i_new_triangle1 = _nb_triangle;
    int i_new_triangle2 = _nb_triangle + 1;
    // Ajout des deux triangles
    _triangle_tab.push_back(_triangle_tab[i_triangle]);
    _triangle_tab.push_back(_triangle_tab[i_triangle]);
    _nb_triangle += 2;

    // Changement de l'association sommet triangle
    _vertex_tab[_triangle_tab[i_triangle].i_vertices[0]].i_triangle = i_new_triangle1;
    _vertex_tab[i_vertex].i_triangle = i_triangle;

    // Les triangles adjacents au triangle (leur indice)
    int i_adjacent_triangle1, i_adjacent_triangle2;
    i_adjacent_triangle1 = _triangle_tab[i_triangle].i_triangles[1];
    i_adjacent_triangle2 = _triangle_tab[i_triangle].i_triangles[2];

    // Changement des sommets
    _triangle_tab[i_triangle].i_vertices[0] = i_vertex;
    _triangle_tab[i_new_triangle1].i_vertices[1] = i_vertex;
    _triangle_tab[i_new_triangle2].i_vertices[2] = i_vertex;

    // Changement des triangles adjacents aux trois triangles
    _triangle_tab[i_triangle].i_triangles[1] = i_new_triangle1;
    _triangle_tab[i_triangle].i_triangles[2] = i_new_triangle2;
    _triangle_tab[i_new_triangle1].i_triangles[0] = i_triangle;
    _triangle_tab[i_new_triangle1].i_triangles[2] = i_new_triangle2;
    _triangle_tab[i_new_triangle2].i_triangles[0] = i_triangle;
    _triangle_tab[i_new_triangle2].i_triangles[1] = i_new_triangle1;

    // Changement des triangles adjacents aux triangles adjacents
    for (int k = 0; k <3; ++k)
    {
        if (_triangle_tab[i_adjacent_triangle1].i_triangles[k] == i_triangle) _triangle_tab[i_adjacent_triangle1].i_triangles[k] = i_new_triangle1;
        if (_triangle_tab[i_adjacent_triangle2].i_triangles[k] == i_triangle) _triangle_tab[i_adjacent_triangle2].i_triangles[k] = i_new_triangle2;
    }

    // Ajout des arêtes à tester
    edges_to_test.push_back(Edge {i_triangle, 0});
    edges_to_test.push_back(Edge {i_new_triangle1, 1});
    edges_to_test.push_back(Edge {i_new_triangle2, 2});


}


void Mesh::_edgeFlip(const int &i_triangle, const int &k_edge, std::vector<Edge>& edges_to_test)
{
    // Triangle de l'autre côté de l'arrête
    int i_other_triangle = _triangle_tab[i_triangle].i_triangles[k_edge];
    if(_b_step_by_step)
        _i_path_tab.push_back(i_other_triangle);
    int k_edge_other = 2;
    for (int k = 0; k <2; ++k)
    {
        if (_triangle_tab[i_other_triangle].i_triangles[k] == i_triangle) k_edge_other = k;
    }

    // Récupération des variables
    int i_vertex_triangle = _triangle_tab[i_triangle].i_vertices[k_edge];
    int i_vertex_other_triangle = _triangle_tab[i_other_triangle].i_vertices[k_edge_other];
    int i_adjacent_triangle = _triangle_tab[i_triangle].i_triangles[(k_edge+1)%3];
    int i_adjacent_triangle_other = _triangle_tab[i_other_triangle].i_triangles[(k_edge_other+1)%3];

    // Modification des triangles
    _triangle_tab[i_triangle].i_vertices[(k_edge+2)%3] = i_vertex_other_triangle;
    _triangle_tab[i_other_triangle].i_vertices[(k_edge_other+2)%3] = i_vertex_triangle;
    _triangle_tab[i_triangle].i_triangles[k_edge] = i_adjacent_triangle_other;
    _triangle_tab[i_triangle].i_triangles[(k_edge+1)%3] = i_other_triangle;
    _triangle_tab[i_other_triangle].i_triangles[k_edge_other] = i_adjacent_triangle;
    _triangle_tab[i_other_triangle].i_triangles[(k_edge_other+1)%3] = i_triangle;

    // Coutures des triangles adjacents
    for (int k = 0; k <3; ++k)
    {
        if (_triangle_tab[i_adjacent_triangle].i_triangles[k] == i_triangle) _triangle_tab[i_adjacent_triangle].i_triangles[k] = i_other_triangle;
        if (_triangle_tab[i_adjacent_triangle_other].i_triangles[k] == i_other_triangle) _triangle_tab[i_adjacent_triangle_other].i_triangles[k] = i_triangle;
    }

    // Changement de l'association sommet triangle
    _vertex_tab[_triangle_tab[i_triangle].i_vertices[(k_edge+1)%3]].i_triangle = i_triangle;
    _vertex_tab[_triangle_tab[i_other_triangle].i_vertices[(k_edge_other+1)%3]].i_triangle = i_other_triangle;

    // Ajout des arêtes à tester
    edges_to_test.push_back(Edge{i_triangle, k_edge});
    edges_to_test.push_back(Edge{i_other_triangle, (k_edge_other + 2)%3});
}

// ========================================================================= //
// Test
// ========================================================================= //

bool Mesh::_orientationTest(const Point &A, const Point &B, const Point &C) const
{
    return (B[0]-A[0])*(C[1]-A[1]) - (B[1]-A[1])*(C[0]-A[0]) > 0;
}


bool Mesh::_inTriangleTest(const Point &M, const int &i_triangle) const
{
    bool res = true;
    for (int k = 0; k < 3; ++k)
        res = res && _orientationTest(_vertex_tab[_triangle_tab[i_triangle].i_vertices[k]].point, _vertex_tab[_triangle_tab[i_triangle].i_vertices[(k+1)%3]].point, M);
    return res;
}


bool Mesh::_inCircleTest(Point M, const int &i_triangle) const
{
    if (M[2] != 0)
        return false;
    Point A = _vertex_tab[_triangle_tab[i_triangle].i_vertices[0]].point;
    Point B = _vertex_tab[_triangle_tab[i_triangle].i_vertices[1]].point;
    Point C = _vertex_tab[_triangle_tab[i_triangle].i_vertices[2]].point;
    B[2] = dot(B-A,B-A);
    C[2] = dot(C-A,C-A);
    M[2] = dot(M-A,M-A);
    return dot(prod(B-A,C-A),M-A) < 0.;
}


bool Mesh::_toFlipEdgeTest(const Edge &e) const
{
    int i_other_triangle = _triangle_tab[e.i_triangle].i_triangles[e.k_edge];
    int k_other = 0;
    for (int k = 1; k < 3; ++k)
    {
        if (_triangle_tab[i_other_triangle].i_triangles[k] == e.i_triangle)
            k_other = k;
    }
    int i_vertex = _triangle_tab[i_other_triangle].i_vertices[k_other];
    return _inCircleTest(_vertex_tab[i_vertex].point, e.i_triangle);
}

void Mesh::_computeTriangleCenters(void)
{
    _triange_center_tab.clear();
    for (int k_triangle = 0; k_triangle < _nb_triangle; ++k_triangle)
    {
        Point center = (_vertex_tab[_triangle_tab[k_triangle].i_vertices[0]].point +
                _vertex_tab[_triangle_tab[k_triangle].i_vertices[1]].point +
                _vertex_tab[_triangle_tab[k_triangle].i_vertices[2]].point) / 3.;
        _triange_center_tab.push_back(center);
        _triangle_tab[k_triangle].i_center = k_triangle;
    }
    _b_triangle_centers_computed = true;
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
    return _mesh._triangle_tab[_current_pos];
}

Triangle* IteratorOnFaces::operator->(void)
{
    return &_mesh._triangle_tab[_current_pos];
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
        if (&_mesh._vertex_tab[_mesh._triangle_tab[_current_i_face].i_vertices[k_vertex]] == &_vertex)
        {
            _current_i_face = _mesh._triangle_tab[_current_i_face].i_triangles[(k_vertex+1)%3];
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
    return _mesh._triangle_tab[_current_i_face];
}

Triangle* CirculatorOnFaces::operator->(void)
{
    return &_mesh._triangle_tab[_current_i_face];
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
    return _mesh._vertex_tab[_current_pos];
}

Vertex* IteratorOnVertices::operator->(void)
{
    return &_mesh._vertex_tab[_current_pos];
}


// Circulateur sur les sommets
CirculatorOnVertices::CirculatorOnVertices(Mesh& mesh, Vertex& vertex, int nb_lap):
    CirculatorOnFaces(mesh, vertex, nb_lap)
{
}

Vertex& CirculatorOnVertices::operator*(void)
{
    for (int k_vertex = 0; k_vertex < 2; k_vertex++)
    {
        if (&_mesh._vertex_tab[_mesh._triangle_tab[_current_i_face].i_vertices[k_vertex]] == &_vertex)
        {
            return _mesh._vertex_tab[_mesh._triangle_tab[_current_i_face].i_vertices[(k_vertex+1)%3]];
        }
    }
    return _mesh._vertex_tab[_mesh._triangle_tab[_current_i_face].i_vertices[0]];
}

Vertex* CirculatorOnVertices::operator->(void)
{
    for (int k_vertex = 0; k_vertex < 2; k_vertex++)
    {
        if (&_mesh._vertex_tab[_mesh._triangle_tab[_current_i_face].i_vertices[k_vertex]] == &_vertex)
        {
            return &_mesh._vertex_tab[_mesh._triangle_tab[_current_i_face].i_vertices[(k_vertex+1)%3]];
        }
    }
    return &_mesh._vertex_tab[_mesh._triangle_tab[_current_i_face].i_vertices[0]];
}

