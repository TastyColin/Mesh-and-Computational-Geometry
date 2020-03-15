#include "gldisplaywidget.h"

#ifdef __APPLE__
    #include <glu.h>
#else
    #include <GL/glu.h>
#endif

#include "QDebug"

GLDisplayWidget::GLDisplayWidget(QWidget *parent) :
    QGLWidget(parent),
    _mode(PLAIN_FACE_MODE),
    _b_show_points(false),
    _b_show_vertex_connexions(false),
    _b_show_triangle_connexions(false),
    _b_show_laplacian(false),
    _b_show_normal(false)
{
    // Update the scene
    connect( &_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    _timer.start(16);
}

void GLDisplayWidget::initializeGL()
{
    // background color
    glClearColor(1., 1., 0.8, 1.);

    // Shader
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    // Construction of the mesh before it is displayed
    // To add....
}

void GLDisplayWidget::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Center the camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,5,  0,0,0,   0,1,0);

    // Translation
    glTranslated(_X, _Y, _Z);

    // Rotation
    glRotatef(_angle, 1.0f, 1.0f, 0.0f);

    // Color for your mesh
    glColor3f(0, 1 ,0);

    // example with a tetraedre
    switch(_mode)
    {
    case PLAIN_FACE_MODE:
        _mesh.drawMesh();
        break;
    case WIREFRAME_MODE:
        _mesh.drawMeshWireFrame();
        break;
    case POINT_MODE:
        _mesh.drawMeshPoints();
        break;
    case CURVATURE_MODE:
        _mesh.drawMeshCurvature();
        break;
    case VORONOI_MODE:
        _mesh.drawMeshVoronoi();
        break;
    }
    if (_b_show_points) {_mesh.drawMeshPoints();}
    if (_b_show_vertex_connexions) { _mesh.drawMeshPointConnexions(); }
    if (_b_show_triangle_connexions) { _mesh.drawMeshTriangleConnexions(); }
    if (_b_show_laplacian) { _mesh.drawMeshLaplacian(); }
    if (_b_show_normal) { _mesh.drawMeshNormal(); }
}

void GLDisplayWidget::resizeGL(int width, int height){

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);

    updateGL();
}

// - - - - - - - - - - - - Mouse Management  - - - - - - - - - - - - - - - -
// When you click, the position of your mouse is saved
void GLDisplayWidget::mousePressEvent(QMouseEvent *event)
{
    if( event != NULL )
        _lastPosMouse = event->pos();
}

// Mouse movement management
void GLDisplayWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - _lastPosMouse.x();
    // int dy = event->y() - lastPosMouse.y();

    if( event != NULL )
    {
        _angle += dx;
        _lastPosMouse = event->pos();

        updateGL();
    }
}

// Mouse Management for the zoom
void GLDisplayWidget::wheelEvent(QWheelEvent *event) {
    QPoint numDegrees = event->angleDelta();
    double stepZoom = 0.25;
    if (!numDegrees.isNull())
    {
      _Z = (numDegrees.x() > 0 || numDegrees.y() > 0) ? _Z + stepZoom : _Z - stepZoom;
    }
}


// ========================================================================= //
// Switch mode
// ========================================================================= //
void GLDisplayWidget::changeMode(int i)
{
    _mode = (DisplayMode) i;
}

void GLDisplayWidget::switchShowPoints()
{
    _b_show_points = !_b_show_points;
}

void GLDisplayWidget::switchShowVertexConnexions()
{
    _b_show_vertex_connexions = !_b_show_vertex_connexions;
}

void GLDisplayWidget::switchShowTriangleConnexions()
{
    _b_show_triangle_connexions = !_b_show_triangle_connexions;
}

void GLDisplayWidget::switchShowLaplacian()
{
    _b_show_laplacian = !_b_show_laplacian;
}


void GLDisplayWidget::switchShowNormal()
{
    _b_show_normal = !_b_show_normal;
}
