SOURCES += mainwindow.cpp \
    shader.cpp \
    glerror.cpp \
    shaderprogram.cpp

HEADERS  += mainwindow.h \
    shader.h \
    glerror.h \
    shaderprogram.h

FORMS    += mainwindow.ui

INCLUDEPATH += $$PWD/../external/glew-1.9.0/include
DEPENDPATH += $$PWD/../external/glew-1.9.0/include

unix: LIBS += $$PWD/../external/glew-1.9.0/lib/libGLEW.a

LIBS += -lglut
LIBS += -lGL
