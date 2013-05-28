#-------------------------------------------------
#
# Project created by QtCreator 2013-05-22T19:47:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = shadercreator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    shader.cpp \
    glerror.cpp \
    shaderprogram.cpp

HEADERS  += mainwindow.h \
    shader.h \
    glerror.h \
    shaderprogram.h

FORMS    += mainwindow.ui

unix: LIBS += $$PWD/../external/glew-1.9.0/lib/libGLEW.a

LIBS += -lglut

INCLUDEPATH += $$PWD/../external/glew-1.9.0/include
DEPENDPATH += $$PWD/../external/glew-1.9.0/include


