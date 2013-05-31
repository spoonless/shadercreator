#include <test.h>
#include <shaderprogram.h>

class ShaderProgramTest : public QObject
{
    Q_OBJECT

private slots:

    void canCreateShaderProgram()
    {
        ShaderProgram shaderProgram;

        QVERIFY(shaderProgram.isValid());
        QVERIFY(shaderProgram.getId() != 0);
        QVERIFY(glIsProgram(shaderProgram.getId()));
    }

    void canAttachDetachShader()
    {
        ShaderProgram shaderProgram;
        Shader shader(Shader::FRAGMENT_SHADER);

        shaderProgram.attach(shader);

        QVERIFY(shaderProgram.has(shader));

        shaderProgram.detach(shader);
        QVERIFY(!shaderProgram.has(shader));
    }

    void canDetachAllShaders()
    {
        ShaderProgram shaderProgram;
        Shader shader1(Shader::VERTEX_SHADER);
        Shader shader2(Shader::FRAGMENT_SHADER);

        shaderProgram.attach(shader1);
        shaderProgram.attach(shader2);

        QVERIFY(shaderProgram.has(shader1));
        QVERIFY(shaderProgram.has(shader2));

        shaderProgram.detachAllShaders();

        QVERIFY(!shaderProgram.has(shader1));
        QVERIFY(!shaderProgram.has(shader2));
    }
};

TEST_DECL(ShaderProgramTest)
#include "shaderprogramtest.moc"
