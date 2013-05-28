#ifndef QOPENGL_H
#define QOPENGL_H

#if QT_VERSION >= 0x050000

#include <QOpenGLFunctions>
#include <QOpenGLFormat>
#include <QOpenGLContext>

#else

#include <QGLFunctions>
#include <QGLFormat>
#include <QGLContext>

#define QOpenGLFunctions QGLFunctions
#define QOpenGLContext QGLContext
#define QOpenGLFormat QGLFormat

#endif

#endif // QOPENGL_H
