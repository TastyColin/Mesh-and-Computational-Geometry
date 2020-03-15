/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "gldisplaywidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout;
    QRadioButton *plain_face_mode;
    QRadioButton *wireframe_mode;
    QRadioButton *curvature_mode;
    QRadioButton *voronoi_mode;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *show_points;
    QCheckBox *show_triangle_connexions;
    QCheckBox *show_vertex_connexions;
    QCheckBox *show_laplacian;
    QCheckBox *show_normal;
    QVBoxLayout *verticalLayout_3;
    QComboBox *select_file_comboBox;
    QPushButton *load_button;
    QPushButton *add_one_vertex;
    QPushButton *add_ten_vertices;
    QPushButton *add_all_vertices;
    QPushButton *exit_button;
    GLDisplayWidget *widget;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(747, 528);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        plain_face_mode = new QRadioButton(centralWidget);
        plain_face_mode->setObjectName(QString::fromUtf8("plain_face_mode"));
        plain_face_mode->setChecked(true);

        verticalLayout->addWidget(plain_face_mode);

        wireframe_mode = new QRadioButton(centralWidget);
        wireframe_mode->setObjectName(QString::fromUtf8("wireframe_mode"));

        verticalLayout->addWidget(wireframe_mode);

        curvature_mode = new QRadioButton(centralWidget);
        curvature_mode->setObjectName(QString::fromUtf8("curvature_mode"));

        verticalLayout->addWidget(curvature_mode);

        voronoi_mode = new QRadioButton(centralWidget);
        voronoi_mode->setObjectName(QString::fromUtf8("voronoi_mode"));

        verticalLayout->addWidget(voronoi_mode);


        verticalLayout_5->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        show_points = new QCheckBox(centralWidget);
        show_points->setObjectName(QString::fromUtf8("show_points"));

        verticalLayout_2->addWidget(show_points);

        show_triangle_connexions = new QCheckBox(centralWidget);
        show_triangle_connexions->setObjectName(QString::fromUtf8("show_triangle_connexions"));

        verticalLayout_2->addWidget(show_triangle_connexions);

        show_vertex_connexions = new QCheckBox(centralWidget);
        show_vertex_connexions->setObjectName(QString::fromUtf8("show_vertex_connexions"));

        verticalLayout_2->addWidget(show_vertex_connexions);

        show_laplacian = new QCheckBox(centralWidget);
        show_laplacian->setObjectName(QString::fromUtf8("show_laplacian"));

        verticalLayout_2->addWidget(show_laplacian);

        show_normal = new QCheckBox(centralWidget);
        show_normal->setObjectName(QString::fromUtf8("show_normal"));

        verticalLayout_2->addWidget(show_normal);


        verticalLayout_5->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        select_file_comboBox = new QComboBox(centralWidget);
        select_file_comboBox->addItem(QString());
        select_file_comboBox->addItem(QString());
        select_file_comboBox->addItem(QString());
        select_file_comboBox->addItem(QString());
        select_file_comboBox->addItem(QString());
        select_file_comboBox->setObjectName(QString::fromUtf8("select_file_comboBox"));

        verticalLayout_3->addWidget(select_file_comboBox);

        load_button = new QPushButton(centralWidget);
        load_button->setObjectName(QString::fromUtf8("load_button"));

        verticalLayout_3->addWidget(load_button);

        add_one_vertex = new QPushButton(centralWidget);
        add_one_vertex->setObjectName(QString::fromUtf8("add_one_vertex"));
        add_one_vertex->setFlat(false);

        verticalLayout_3->addWidget(add_one_vertex);

        add_ten_vertices = new QPushButton(centralWidget);
        add_ten_vertices->setObjectName(QString::fromUtf8("add_ten_vertices"));

        verticalLayout_3->addWidget(add_ten_vertices);

        add_all_vertices = new QPushButton(centralWidget);
        add_all_vertices->setObjectName(QString::fromUtf8("add_all_vertices"));

        verticalLayout_3->addWidget(add_all_vertices);


        verticalLayout_5->addLayout(verticalLayout_3);

        exit_button = new QPushButton(centralWidget);
        exit_button->setObjectName(QString::fromUtf8("exit_button"));

        verticalLayout_5->addWidget(exit_button);


        horizontalLayout->addLayout(verticalLayout_5);

        widget = new GLDisplayWidget(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));

        horizontalLayout->addWidget(widget);

        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

        retranslateUi(MainWindow);
        QObject::connect(exit_button, SIGNAL(released()), MainWindow, SLOT(close()));

        add_one_vertex->setDefault(false);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Mesh_Computational_Geometry", nullptr));
        plain_face_mode->setText(QApplication::translate("MainWindow", "Plain Face", nullptr));
        wireframe_mode->setText(QApplication::translate("MainWindow", "Wireframe", nullptr));
        curvature_mode->setText(QApplication::translate("MainWindow", "Curvature", nullptr));
        voronoi_mode->setText(QApplication::translate("MainWindow", "Vorono\303\257", nullptr));
        show_points->setText(QApplication::translate("MainWindow", "Show Points", nullptr));
        show_triangle_connexions->setText(QApplication::translate("MainWindow", "Show triangle connexions", nullptr));
        show_vertex_connexions->setText(QApplication::translate("MainWindow", "Show vertex connexions", nullptr));
        show_laplacian->setText(QApplication::translate("MainWindow", "Show Laplacian", nullptr));
        show_normal->setText(QApplication::translate("MainWindow", "Show Normal", nullptr));
        select_file_comboBox->setItemText(0, QApplication::translate("MainWindow", "Tetrahedron", nullptr));
        select_file_comboBox->setItemText(1, QApplication::translate("MainWindow", "Cube", nullptr));
        select_file_comboBox->setItemText(2, QApplication::translate("MainWindow", "Pyramide", nullptr));
        select_file_comboBox->setItemText(3, QApplication::translate("MainWindow", "Queen", nullptr));
        select_file_comboBox->setItemText(4, QApplication::translate("MainWindow", "Slick", nullptr));

        load_button->setText(QApplication::translate("MainWindow", "Load File", nullptr));
        add_one_vertex->setText(QApplication::translate("MainWindow", "Add 1 vertex (Step by step)", nullptr));
        add_ten_vertices->setText(QApplication::translate("MainWindow", "Add 10 vertices", nullptr));
        add_all_vertices->setText(QApplication::translate("MainWindow", "Add all vertices", nullptr));
        exit_button->setText(QApplication::translate("MainWindow", "Exit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
