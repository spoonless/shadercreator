#include <test.h>
#include <shader.h>

class ShaderTest : public QObject
{
    Q_OBJECT

private slots:

    void canCompileVertexShader()
    {
        Shader shader(Shader::VERTEX_SHADER);
        bool compilationResult = shader.compile("void main(){}");

        QVERIFY2(compilationResult, "shader compilation failed");
        QVERIFY(shader.getId() != 0);
        QVERIFY(glIsShader(shader.getId()));
        QVERIFY(shader.getType() == Shader::VERTEX_SHADER);
        QVERIFY(getShaderType(shader) == GL_VERTEX_SHADER);
    }

    void canCompileFragmentShader()
    {
        Shader shader(Shader::FRAGMENT_SHADER);
        bool compilationResult = shader.compile("void main(){}");

        QVERIFY2(compilationResult, "shader compilation failed");
        QVERIFY(shader.getId() != 0);
        QVERIFY(glIsShader(shader.getId()));
        QVERIFY(shader.getType() == Shader::FRAGMENT_SHADER);
        QVERIFY(getShaderType(shader) == GL_FRAGMENT_SHADER);
    }

    void cannotCompileErroneousCode()
    {
        Shader shader(Shader::FRAGMENT_SHADER);
        bool compilationResult = shader.compile("ERRONEOUS CODE");

        QVERIFY2(!compilationResult, "shader compilation succeeded");
        QVERIFY(shader.getLog() != "");
    }

    void canExtractSource()
    {
        std::string source = "void main(){}";
        Shader shader(Shader::VERTEX_SHADER);
        shader.compile(source.c_str());

        std::string outputSource;
        shader.extractSource(outputSource);

        QVERIFY2(outputSource == source, outputSource.c_str());
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
        QVERIFY(copySource == "void main(){}");
    }

    void canAssignShader()
    {
        Shader shader(Shader::FRAGMENT_SHADER);
        shader.compile("void fragment(){}");

        Shader otherShader(Shader::VERTEX_SHADER);
        otherShader.compile("void vertex(){}");

        shader = otherShader;
        std::string source;
        shader.extractSource(source);

        QVERIFY(shader.getType() == Shader::VERTEX_SHADER);
        QVERIFY(source == "void vertex(){}");
        QVERIFY(getShaderType(shader) == GL_VERTEX_SHADER);
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
