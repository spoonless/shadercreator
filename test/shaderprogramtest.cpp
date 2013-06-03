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

        QVERIFY(shaderProgram.attach(shader));

        QVERIFY(shaderProgram.has(shader));

        QVERIFY(shaderProgram.detach(shader));
        QVERIFY(!shaderProgram.has(shader));
    }

    void canDetachAllShaders()
    {
        ShaderProgram shaderProgram;
        Shader shader1(Shader::VERTEX_SHADER);
        Shader shader2(Shader::FRAGMENT_SHADER);

        QVERIFY(shaderProgram.attach(shader1));
        QVERIFY(shaderProgram.attach(shader2));

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

    void canExtractEmptyUniformInfoWhenNoUniform()
    {
        ShaderProgram shaderProgram;
        addShader(shaderProgram, Shader::FRAGMENT_SHADER, VALID_FRAGMENT_SHADER_SOURCE);
        QVERIFY(shaderProgram.link());

        UniformInfoVector uniformInfoVector;
        shaderProgram.extractActiveUniformInfo(uniformInfoVector);

        QVERIFY(uniformInfoVector.empty());
    }

    void canExtractUniformInfoWhenOneUniform()
    {
        ShaderProgram shaderProgram;
        const char* source = \
                "uniform vec4 position;"\
                "void main() {"\
                " gl_Position = position;"\
                "}";

        addShader(shaderProgram, Shader::VERTEX_SHADER, source);
        QVERIFY(shaderProgram.link());

        UniformInfoVector uniformInfoVector;
        shaderProgram.extractActiveUniformInfo(uniformInfoVector);

        QCOMPARE(uniformInfoVector.size(), static_cast<size_t>(1));

        UniformInfo& uniformInfo = uniformInfoVector[0];
        QCOMPARE(uniformInfo.getIndex(), static_cast<GLuint>(0));
        QCOMPARE(uniformInfo.getSize(), 1);
        QCOMPARE(uniformInfo.getType(), static_cast<GLenum>(GL_FLOAT_VEC4));
        QCOMPARE(uniformInfo.getName().c_str(), "position");
    }

    void canExtractFixedArrayUniformInfo()
    {
        ShaderProgram shaderProgram;
        const char* source = \
                "uniform vec4 position[2];"\
                "void main() {"\
                " gl_Position = position[0] + position[1];"\
                "}";

        addShader(shaderProgram, Shader::VERTEX_SHADER, source);
        QVERIFY(shaderProgram.link());

        UniformInfoVector uniformInfoVector;
        shaderProgram.extractActiveUniformInfo(uniformInfoVector);

        QCOMPARE(uniformInfoVector.size(), static_cast<size_t>(1));

        UniformInfo& uniformInfo = uniformInfoVector[0];
        QCOMPARE(uniformInfo.getIndex(), static_cast<GLuint>(0));
        QCOMPARE(uniformInfo.getSize(), 2);
        QCOMPARE(uniformInfo.getType(), static_cast<GLenum>(GL_FLOAT_VEC4));
        QCOMPARE(uniformInfo.getName().c_str(), "position");
    }

    void canExtractArrayUniformInfo()
    {
        ShaderProgram shaderProgram;
        const char* source = \
                "uniform vec4 position[];"\
                "void main() {"\
                " gl_Position = position[0] + position[1] + position[3];"\
                "}";

        addShader(shaderProgram, Shader::VERTEX_SHADER, source);
        QVERIFY(shaderProgram.link());

        UniformInfoVector uniformInfoVector;
        shaderProgram.extractActiveUniformInfo(uniformInfoVector);

        QCOMPARE(uniformInfoVector.size(), static_cast<size_t>(1));

        UniformInfo& uniformInfo = uniformInfoVector[0];
        QCOMPARE(uniformInfo.getIndex(), static_cast<GLuint>(0));
        QCOMPARE(uniformInfo.getSize(), 4);
        QCOMPARE(uniformInfo.getType(), static_cast<GLenum>(GL_FLOAT_VEC4));
        QCOMPARE(uniformInfo.getName().c_str(), "position");
    }

    void canExtractStructUniformInfo()
    {
        ShaderProgram shaderProgram;
        const char* source = \
                "struct MyStruct {"\
                " vec4 position1;"\
                " vec3 position2;"\
                "};"\
                "uniform MyStruct ms;"\
                "void main() {"\
                " gl_Position = ms.position1 + vec4(ms.position2, 0);"\
                "}";

        addShader(shaderProgram, Shader::VERTEX_SHADER, source);
        QVERIFY(shaderProgram.link());

        UniformInfoVector uniformInfoVector;
        shaderProgram.extractActiveUniformInfo(uniformInfoVector);

        UniformInfo* uniformInfo = &uniformInfoVector[0];
        QCOMPARE(uniformInfo->getIndex(), static_cast<GLuint>(0));
        QCOMPARE(uniformInfo->getSize(), 1);
        QCOMPARE(uniformInfo->getType(), static_cast<GLenum>(GL_FLOAT_VEC4));
        QCOMPARE(uniformInfo->getName().c_str(), "ms.position1");

        uniformInfo = &uniformInfoVector[1];
        QCOMPARE(uniformInfo->getIndex(), static_cast<GLuint>(1));
        QCOMPARE(uniformInfo->getSize(), 1);
        QCOMPARE(uniformInfo->getType(), static_cast<GLenum>(GL_FLOAT_VEC3));
        QCOMPARE(uniformInfo->getName().c_str(), "ms.position2");
    }

    void canExtractMultipleUniformInfo()
    {
        ShaderProgram shaderProgram;
        const char* source = \
                "uniform vec4 position;"\
                "uniform mat4 mvp;"
                "void main() {"\
                " gl_Position = mvp * position;"\
                "}";

        addShader(shaderProgram, Shader::VERTEX_SHADER, source);
        QVERIFY(shaderProgram.link());

        UniformInfoVector uniformInfoVector;
        shaderProgram.extractActiveUniformInfo(uniformInfoVector);

        QCOMPARE(uniformInfoVector.size(), static_cast<size_t>(2));

        for (unsigned int i = 0; i < uniformInfoVector.size(); ++i)
        {
            UniformInfo& uniformInfo = uniformInfoVector[i];
            QCOMPARE(uniformInfo.getIndex(), static_cast<GLuint>(i));
            if (uniformInfo.getName() == "position")
            {
                QCOMPARE(uniformInfo.getSize(), 1);
                QCOMPARE(uniformInfo.getType(), static_cast<GLenum>(GL_FLOAT_VEC4));
            }
            else if (uniformInfo.getName() == "mvp")
            {
                QCOMPARE(uniformInfo.getSize(), 1);
                QCOMPARE(uniformInfo.getType(), static_cast<GLenum>(GL_FLOAT_MAT4));
            }
            else
            {
                QFAIL("unexpected uniform");
            }
        }
    }

private:
    void addShader(ShaderProgram& shaderProgram, Shader::ShaderType type, const char* source)
    {
        Shader vertexShader(type);
        QVERIFY(vertexShader.compile(source));
        QVERIFY(shaderProgram.attach(vertexShader));
    }
};

TEST_DECL(ShaderProgramTest)
#include "shaderprogramtest.moc"
