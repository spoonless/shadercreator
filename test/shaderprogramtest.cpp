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

        addShader(shaderProgram, Shader::FRAGMENT_SHADER, VALID_FRAGMENT_SHADER_SOURCE);
        QVERIFY(shaderProgram.link());

        addShader(shaderProgram, Shader::VERTEX_SHADER, VALID_VERTEX_SHADER_SOURCE);
        QVERIFY(shaderProgram.link());

        shaderProgram.link();

        QVERIFY(shaderProgram.validate());
    }

    void canExtractEmptyUniformDeclarationWhenNoUniform()
    {
        ShaderProgram shaderProgram;
        addShader(shaderProgram, Shader::FRAGMENT_SHADER, VALID_FRAGMENT_SHADER_SOURCE);
        QVERIFY(shaderProgram.link());

        UniformDeclarationVector uniformDeclarationVector;
        shaderProgram.extractActive(uniformDeclarationVector);

        QVERIFY(uniformDeclarationVector.empty());
    }

    void canExtractUniformDeclarationWhenOneUniform()
    {
        ShaderProgram shaderProgram;
        const char* source = \
                "uniform vec4 position;"\
                "void main() {"\
                " gl_Position = position;"\
                "}";

        addShader(shaderProgram, Shader::VERTEX_SHADER, source);
        QVERIFY(shaderProgram.link());

        UniformDeclarationVector uniformDeclarationVector;
        shaderProgram.extractActive(uniformDeclarationVector);

        QCOMPARE(uniformDeclarationVector.size(), static_cast<size_t>(1));

        QCOMPARE(uniformDeclarationVector[0], UniformDeclaration(0, 1, GL_FLOAT_VEC4, "position"));
    }

    void canExtractFixedArrayUniformDeclaration()
    {
        ShaderProgram shaderProgram;
        const char* source = \
                "uniform vec4 position[2];"\
                "void main() {"\
                " gl_Position = position[0] + position[1];"\
                "}";

        addShader(shaderProgram, Shader::VERTEX_SHADER, source);
        QVERIFY(shaderProgram.link());

        UniformDeclarationVector UniformDeclarationVector;
        shaderProgram.extractActive(UniformDeclarationVector);

        QCOMPARE(UniformDeclarationVector.size(), static_cast<size_t>(1));

        QCOMPARE(UniformDeclarationVector[0], UniformDeclaration(0, 2, GL_FLOAT_VEC4, "position[0]"));
    }

    void canExtractArrayUniformDeclaration()
    {
        ShaderProgram shaderProgram;
        const char* source = \
                "uniform vec4 position[];"\
                "void main() {"\
                " gl_Position = position[0] + position[1] + position[3];"\
                "}";

        addShader(shaderProgram, Shader::VERTEX_SHADER, source);
        QVERIFY(shaderProgram.link());

        UniformDeclarationVector UniformDeclarationVector;
        shaderProgram.extractActive(UniformDeclarationVector);

        QCOMPARE(UniformDeclarationVector.size(), static_cast<size_t>(1));

        QCOMPARE(UniformDeclarationVector[0], UniformDeclaration(0, 4, GL_FLOAT_VEC4, "position[0]"));
    }

    void canExtractStructUniformDeclaration()
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

        UniformDeclarationVector UniformDeclarationVector;
        shaderProgram.extractActive(UniformDeclarationVector);

        QCOMPARE(UniformDeclarationVector.size(), static_cast<size_t>(2));

        QCOMPARE(UniformDeclarationVector[0], UniformDeclaration(0, 1, GL_FLOAT_VEC4, "ms.position1"));
        QCOMPARE(UniformDeclarationVector[1], UniformDeclaration(1, 1, GL_FLOAT_VEC3, "ms.position2"));
    }

    void canExtractStructArrayUniformDeclaration()
    {
        ShaderProgram shaderProgram;
        const char* source = \
                "struct MyStruct {"\
                " vec4 position1[2];"\
                " vec3 position2;"\
                "};"\
                "uniform MyStruct ms[2];"\
                "void main() {"\
                " gl_Position = ms[1].position1[1] + vec4(ms[1].position2, 0);"\
                "}";

        addShader(shaderProgram, Shader::VERTEX_SHADER, source);
        QVERIFY(shaderProgram.link());

        UniformDeclarationVector UniformDeclarationVector;
        shaderProgram.extractActive(UniformDeclarationVector);

        QCOMPARE(UniformDeclarationVector.size(), static_cast<size_t>(2));
        QCOMPARE(UniformDeclarationVector[0], UniformDeclaration(0, 2, GL_FLOAT_VEC4, "ms[1].position1[0]"));
        QCOMPARE(UniformDeclarationVector[1], UniformDeclaration(1, 1, GL_FLOAT_VEC3, "ms[1].position2"));
    }

    void canExtractMultipleUniformDeclaration()
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

        UniformDeclarationVector UniformDeclarationVector;
        shaderProgram.extractActive(UniformDeclarationVector);

        QCOMPARE(UniformDeclarationVector.size(), static_cast<size_t>(2));

        for (unsigned int i = 0; i < UniformDeclarationVector.size(); ++i)
        {
            UniformDeclaration& UniformDeclaration = UniformDeclarationVector[i];
            QCOMPARE(UniformDeclaration.getIndex(), static_cast<GLuint>(i));
            if (UniformDeclaration.getName() == "position")
            {
                QCOMPARE(UniformDeclaration.getSize(), 1);
                QCOMPARE(UniformDeclaration.getType(), static_cast<GLenum>(GL_FLOAT_VEC4));
            }
            else if (UniformDeclaration.getName() == "mvp")
            {
                QCOMPARE(UniformDeclaration.getSize(), 1);
                QCOMPARE(UniformDeclaration.getType(), static_cast<GLenum>(GL_FLOAT_MAT4));
            }
            else
            {
                QFAIL("unexpected uniform");
            }
        }
    }

    void canCopyUniformDeclaration()
    {
        UniformDeclaration UniformDeclaration1(1, 10, GL_FLOAT_MAT2, "UniformDeclaration1");
        UniformDeclaration UniformDeclaration2(2, 20, GL_FLOAT_MAT3, "UniformDeclaration2");

        QVERIFY(UniformDeclaration1 != UniformDeclaration2);

        UniformDeclaration1 = UniformDeclaration2;
        QCOMPARE(UniformDeclaration1, UniformDeclaration2);
    }

private:
    void addShader(ShaderProgram& shaderProgram, Shader::ShaderType type, const char* source)
    {
        Shader vertexShader(type);
        if (!vertexShader.compile(source))
        {
            QFAIL(vertexShader.getLastCompilationLog().c_str());
        }
        QVERIFY(shaderProgram.attach(vertexShader));
    }
};

TEST_DECL(ShaderProgramTest)
#include "shaderprogramtest.moc"
