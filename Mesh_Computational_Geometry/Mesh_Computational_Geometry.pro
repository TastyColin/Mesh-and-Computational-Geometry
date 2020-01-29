#-------------------------------------------------
#
# Project created by QtCreator 2018-08-28T10:55:17
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mesh_Computational_Geometry
TEMPLATE = app


SOURCES += main.cpp\
    gradient.cpp \
        mainwindow.cpp \
    gldisplaywidget.cpp \
    mesh.cpp \
    point.cpp \
    triangle.cpp

HEADERS  += mainwindow.h \
    gldisplaywidget.h \
    gradient.h \
    mesh.h \
    point.h \
    triangle.h

FORMS    += mainwindow.ui

#---- Comment the following line on MacOS
# LIBS = -lGLU

LIBS += -lglu32
LIBS += -lOpengl32