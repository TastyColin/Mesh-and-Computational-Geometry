#ifndef GLDISPLAYWIDGET_H
#define GLDISPLAYWIDGET_H

#include <QGLWidget>
#include <QtWidgets>
#include <QTimer>
#include "mesh.h"

enum DisplayMode
{
    PLAIN_FACE_MODE = 0,
    WIREFRAME_MODE = 1,
    CURVATURE_MODE = 2
};

class GLDisplayWidget : public QGLWidget
{
public:
    explicit GLDisplayWidget(QWidget *parent = 0);

    void initializeGL(); // The scene may be initialized in this function since the mesh is a data member...
    void paintGL(); // Display the scene Gl
    void resizeGL(int width, int height);

    void changeMode(int i);
    inline void loadFile(const char file_name[]) {_mesh.loadMesh(file_name);};

    void switchShowVertexConnexions();
    void switchShowTriangleConnexions();
    void switchShowLaplacian();
    void switchShowNormal();

protected:
    // Mouse Management
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    // Mode d'affichage
    DisplayMode _mode;
    bool _b_show_vertex_connexions;
    bool _b_show_triangle_connexions;
    bool _b_show_laplacian;
    bool _b_show_normal;

private:

    QTimer _timer; // To update the scene
    float _X, _Y ,_Z; // Translation
    float _angle; // Rotation

    QPoint _lastPosMouse; // To keep the last position of the mouse

    Mesh _mesh; // The object to be displayed, may be replaced by a scene if there are several objects....
};

#endif // GLDISPLAYWIDGET_H
