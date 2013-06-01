#include <test.h>
#include <shaderprogram.h>

#define VALID_VERTEX_SHADER_SOURCE "void main(){gl_Position = gl_Vertex;}"
#define VALID_FRAGMENT_SHADER_SOURCE "void main(){}"

class ShaderProgramTest : public QObject
{
    Q_OBJECT

private slots:

    void canCreateShaderProgram()
    {
        ShaderProgram shaderProgram;

        QVERIFY(shaderProgram.exists());
        QVERIFY(shaderProgram.getId() != 0);
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

    void cannotLinkProgramWithoutShader()
    {
        ShaderProgram shaderProgram;

        QVERIFY(!shaderProgram.link());
        QCOMPARE(shaderProgram.getLastLinkLog().c_str(), "Cannot link program because no shader is attached!");
    }

    void cannotLinkProgramWhenShaderNotCompiled()
    {
        ShaderProgram shaderProgram;
        Shader shader(Shader::FRAGMENT_SHADER);

        shaderProgram.attach(shader);

        QVERIFY(!shaderProgram.link());
        QVERIFY(!shaderProgram.getLastLinkLog().empty());
    }

    void canLinkProgramWhenShadersCompiled()
    {
        ShaderProgram shaderProgram;

        Shader fragmentShader(Shader::FRAGMENT_SHADER);
        shaderProgram.attach(fragmentShader);
        fragmentShader.compile(VALID_FRAGMENT_SHADER_SOURCE);

        QVERIFY(shaderProgram.link());

        Shader vertexShader(Shader::VERTEX_SHADER);
        vertexShader.compile(VALID_VERTEX_SHADER_SOURCE);
        shaderProgram.attach(vertexShader);

        QVERIFY(shaderProgram.link());
    }

    void canCopyProgram()
    {
        ShaderProgram shaderProgram;

        Shader fragmentShader(Shader::FRAGMENT_SHADER);
        fragmentShader.compile(VALID_FRAGMENT_SHADER_SOURCE);
        shaderProgram.attach(fragmentShader);

        Shader vertexShader(Shader::VERTEX_SHADER);
        vertexShader.compile(VALID_VERTEX_SHADER_SOURCE);
        shaderProgram.attach(vertexShader);

        ShaderProgram copy = shaderProgram;

        QVERIFY(copy.exists());
        QVERIFY(copy.getId() != 0);
        QVERIFY(glIsProgram(copy.getId()));
        QVERIFY(copy.has(fragmentShader));
        QVERIFY(copy.has(vertexShader));
        QVERIFY(copy.link());
    }

    void canAssignProgram()
    {
        ShaderProgram shaderProgram;

        Shader fragmentShader(Shader::FRAGMENT_SHADER);
        fragmentShader.compile(VALID_FRAGMENT_SHADER_SOURCE);
        shaderProgram.attach(fragmentShader);

        Shader vertexShader(Shader::VERTEX_SHADER);
        vertexShader.compile(VALID_VERTEX_SHADER_SOURCE);
        shaderProgram.attach(vertexShader);

        ShaderProgram copy;
        Shader anotherShader(Shader::VERTEX_SHADER);
        copy.attach(anotherShader);
        QVERIFY(copy.has(anotherShader));

        copy = shaderProgram;

        QVERIFY(copy.has(fragmentShader));
        QVERIFY(copy.has(vertexShader));
        QVERIFY(!copy.has(anotherShader));
        QVERIFY(copy.link());
    }

    void cannotValidateUnlinkProgram()
    {
        ShaderProgram shaderProgram;

        QVERIFY(!shaderProgram.validate());
        QVERIFY(!shaderProgram.getLastValidationLog().empty());
    }

    void canValidateLinkedProgramWhenShadersCompiled()
    {
        ShaderProgram shaderProgram;

        Shader fragmentShader(Shader::FRAGMENT_SHADER);
        shaderProgram.attach(fragmentShader);
        fragmentShader.compile(VALID_FRAGMENT_SHADER_SOURCE);

        QVERIFY(shaderProgram.link());

        Shader vertexShader(Shader::VERTEX_SHADER);
        vertexShader.compile(VALID_VERTEX_SHADER_SOURCE);
        shaderProgram.attach(vertexShader);

        shaderProgram.link();

        QVERIFY(shaderProgram.validate());
    }
};

TEST_DECL(ShaderProgramTest)
#include "shaderprogramtest.moc"
