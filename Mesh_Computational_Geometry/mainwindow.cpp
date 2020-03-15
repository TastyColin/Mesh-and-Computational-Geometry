#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->plain_face_mode, SIGNAL(clicked()), this, SLOT(plainModeSelected()));
    connect(ui->wireframe_mode, SIGNAL(clicked()), this, SLOT(wireModeSelected()));
    connect(ui->curvature_mode, SIGNAL(clicked()), this, SLOT(curvatureModeSelected()));
    connect(ui->voronoi_mode, SIGNAL(clicked()), this, SLOT(voronoiModeSelected()));
    connect(ui->show_points, SIGNAL(clicked()), this, SLOT(switchShowPoints()));
    connect(ui->show_vertex_connexions, SIGNAL(clicked()), this, SLOT(switchShowVertexConnexions()));
    connect(ui->show_triangle_connexions, SIGNAL(clicked()), this, SLOT(switchShowTriangleConnexions()));
    connect(ui->show_laplacian, SIGNAL(clicked()), this, SLOT(switchShowLaplacian()));
    connect(ui->show_normal, SIGNAL(clicked()), this, SLOT(switchShowNormal()));
   }

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::plainModeSelected()
{
    ui->widget->changeMode(0);
}


void MainWindow::wireModeSelected()
{
    ui->widget->changeMode(1);
}

void MainWindow::curvatureModeSelected()
{
    ui->widget->changeMode(3);
}

void MainWindow::voronoiModeSelected()
{
    ui->widget->changeMode(4);
}

void MainWindow::switchShowPoints()
{
    ui->widget->switchShowPoints();
}


void MainWindow::switchShowVertexConnexions()
{
    ui->widget->switchShowVertexConnexions();
}

void MainWindow::switchShowTriangleConnexions()
{
    ui->widget->switchShowTriangleConnexions();
}

void MainWindow::switchShowLaplacian()
{
    ui->widget->switchShowLaplacian();
}

void MainWindow::switchShowNormal()
{
    ui->widget->switchShowNormal();
}


void MainWindow::on_load_button_clicked()
{
    QString s_file("../"+ui->select_file_comboBox->currentText()+".off");
    const char* c_file= s_file.toStdString().c_str();
    ui->widget->loadFile(c_file);
}

void MainWindow::on_add_one_vertex_clicked()
{
    ui->widget->addVertex(1);   // Ajoute un sommet
}

void MainWindow::on_add_ten_vertices_clicked()
{
    ui->widget->addVertex(10);  // Ajoute tous les sommets
}

void MainWindow::on_add_all_vertices_clicked()
{
    ui->widget->addVertex(-1);  // Ajoute tous les sommets
}
