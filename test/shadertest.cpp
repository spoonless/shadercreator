#include <test.h>
#include <shader.h>

class ShaderTest : public QObject
{
    Q_OBJECT

private slots:

    void canCreateVertexShader()
    {
        Shader shader(Shader::VERTEX_SHADER);

        QVERIFY(shader.getId() != 0);
        QVERIFY(shader.exists());
        QCOMPARE(shader.getType(), Shader::VERTEX_SHADER);
        QCOMPARE(getShaderType(shader), GL_VERTEX_SHADER);
    }

    void canCreateFragmentShader()
    {
        Shader shader(Shader::FRAGMENT_SHADER);

        QVERIFY(shader.getId() != 0);
        QVERIFY(shader.exists());
        QCOMPARE(shader.getType(), Shader::FRAGMENT_SHADER);
        QCOMPARE(getShaderType(shader), GL_FRAGMENT_SHADER);
    }

    void canCompileShader()
    {
        Shader shader(Shader::FRAGMENT_SHADER);
        bool compilationResult = shader.compile("void main(){}");

        QVERIFY2(compilationResult, "shader compilation failed");
    }

    void cannotCompileErroneousCode()
    {
        Shader shader(Shader::FRAGMENT_SHADER);
        bool compilationResult = shader.compile("ERRONEOUS CODE");

        QVERIFY2(!compilationResult, "shader compilation succeeded");
        QVERIFY(shader.getLastCompilationLog() != "");
    }

    void canExtractSource()
    {
        std::string source = "void main(){}";
        Shader shader(Shader::VERTEX_SHADER);
        shader.compile(source.c_str());

        std::string outputSource;
        shader.extractSource(outputSource);

        QCOMPARE(outputSource, source);
    }

    void canCopyShader()
    {
        Shader shader(Shader::FRAGMENT_SHADER);
        shader.compile("void main(){}");

        Shader copyShader = shader;
        std::string copySource;
        copyShader.extractSource(copySource);

        QVERIFY(copyShader.getId() != shader.getId());
        QVERIFY(glIsShader(copyShader.getId()));
        QCOMPARE(copySource.c_str(), "void main(){}");
    }

    void canAssignShader()
    {
        Shader shader(Shader::VERTEX_SHADER);
        shader.compile("void vertex(){}");

        Shader otherShader(Shader::VERTEX_SHADER);
        otherShader.compile("void otherVertex(){}");

        shader = otherShader;
        std::string source;
        shader.extractSource(source);

        QCOMPARE(shader.getType(), Shader::VERTEX_SHADER);
        QCOMPARE(source.c_str(), "void otherVertex(){}");
        QCOMPARE(getShaderType(shader), GL_VERTEX_SHADER);
    }

    void cannotAssignShaderOfDifferentType()
    {
        Shader shader(Shader::VERTEX_SHADER);
        shader.compile("void vertex(){}");

        Shader otherShader(Shader::FRAGMENT_SHADER);
        otherShader.compile("void fragment(){}");

        shader = otherShader;
        std::string source;
        shader.extractSource(source);

        QCOMPARE(shader.getType(), Shader::VERTEX_SHADER);
        QCOMPARE(source.c_str(), "void vertex(){}");
        QCOMPARE(getShaderType(shader), GL_VERTEX_SHADER);
    }

private:
    GLint getShaderType(Shader shader)
    {
        GLint shaderType = 0;
        glGetShaderiv(shader.getId(), GL_SHADER_TYPE, &shaderType);
        return shaderType;
    }
};

TEST_DECL(ShaderTest)
#include "shadertest.moc"
