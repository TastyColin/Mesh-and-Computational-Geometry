#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void plainModeSelected();
    void wireModeSelected();
    void curvatureModeSelected();
    void voronoiModeSelected();
    void switchShowPoints();
    void switchShowVertexConnexions();
    void switchShowTriangleConnexions();
    void switchShowLaplacian();
    void switchShowNormal();

private slots:
    void on_load_button_clicked();
    void on_add_one_vertex_clicked();
    void on_add_ten_vertices_clicked();
    void on_add_all_vertices_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
