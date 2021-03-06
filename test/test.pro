QT += core gui widgets opengl testlib

CONFIG += testcase
TEMPLATE = app

DEPENDPATH += ../src
INCLUDEPATH += ../src
include(../src/shadercreator.pri)

SOURCES += shadertest.cpp \
    main.cpp \
    glerrortest.cpp \
    shaderprogramtest.cpp \
    uniformdeclarationtest.cpp

HEADERS += \
    test.h
