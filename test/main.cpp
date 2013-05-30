#include "GL/glew.h"
#include <QApplication>
#include <QGLWidget>
#include <QDebug>

#include "test.h"

TEST_INCLUDE(ShaderTest)
TEST_INCLUDE(GlErrorTest)

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QGLWidget glWidget;
    glWidget.makeCurrent();

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        qWarning() << "Error while initializing GLEW: " << glewGetErrorString(err);
        return 1;
    }

    int retval(0);

    retval += TEST_RUN(ShaderTest);
    retval += TEST_RUN(GlErrorTest);

    return (retval ? 1 : 0);
}
