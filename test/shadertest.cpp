#include <GL/glew.h>
#include <QGLFramebufferObject>
#include <QGLContext>

#include <test.h>

#include <shader.h>

class ShaderTest : public QObject
{
    Q_OBJECT

public:
    ShaderTest()
        :_glContext(NULL), _offscreen(1024,1024)
    {
    }

    virtual ~ShaderTest()
    {
        delete _glContext;
    }

private slots:

    void initTestCase()
    {
        QGLFormat glFormat;
        _glContext = new QGLContext(glFormat, &_offscreen);
        QVERIFY(_glContext->create());

        GLenum err = glewInit();
        QVERIFY(GLEW_OK != err);
    }

    void canCompileShader()
    {
        Shader shader(Shader::VERTEX_SHADER);
        QVERIFY(shader.compile("void main(){}"));
    }
private:
    QGLContext* _glContext;
    QGLFramebufferObject _offscreen;
};

TEST_DECL(ShaderTest)
#include "shadertest.moc"
