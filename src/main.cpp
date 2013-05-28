/*
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}

*/

#include "GL/glew.h"
#include <GL/glut.h>
#include <iostream>

#include "shader.h"


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutCreateWindow("GLEW Test");
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      /* Problem: glewInit failed, something is seriously wrong. */
        std::cerr << "Error:" << glewGetErrorString(err) << std::endl;
        return 1;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;

    Shader shader(Shader::VERTEX_SHADER);
    shader.compile("void main(){}");
    std::cout << shader.getLog() << std::endl;
    std::cout << "compilation time: " << shader.getCompilationDuration() << " ms" << std::endl;

    std::string source;
    shader.extractSource(source);
    std::cout << "source compiled: " << source << std::endl;


    Shader shader2 = shader;
    std::cout << shader2.getLog() << std::endl;
    std::cout << "compilation time: " << shader2.getCompilationDuration() << " ms" << std::endl;

    shader2.compile("not valid vertex shader");
    std::cout << shader2.getLog() << std::endl;
    std::cout << "compilation time: " << shader2.getCompilationDuration() << " ms" << std::endl;

    return 0;
}
