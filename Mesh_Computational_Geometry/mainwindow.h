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
    void switchShowPoints();
    void switchShowVertexConnexions();
    void switchShowTriangleConnexions();
    void switchShowLaplacian();
    void switchShowNormal();

private slots:
    void on_load_button_clicked();
    void on_complete_button_clicked();
    void on_delaunay_button_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
