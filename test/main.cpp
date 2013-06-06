#include "GL/glew.h"
#include <QApplication>
#include <QGLWidget>
#include <QDebug>

#include "test.h"

TEST_INCLUDE(GlErrorTest)
TEST_INCLUDE(ShaderTest)
TEST_INCLUDE(ShaderProgramTest)
TEST_INCLUDE(UniformDeclarationTest)

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

    int retval = 0;

    retval |= TEST_RUN(GlErrorTest);
    retval |= TEST_RUN(ShaderTest);
    retval |= TEST_RUN(ShaderProgramTest);
    retval |= TEST_RUN(UniformDeclarationTest);

    return retval;
}
