#include <test.h>
#include <uniformdeclaration.h>

class UniformDeclarationTest : public QObject
{
    Q_OBJECT

private slots:

    void canCopyUniformDeclaration()
    {
        UniformDeclaration uniformDeclaration1(1, 10, GL_FLOAT_MAT2, "uniformDeclaration1");
        UniformDeclaration uniformDeclaration2(2, 20, GL_FLOAT_MAT3, "uniformDeclaration2");

        QVERIFY(uniformDeclaration1 != uniformDeclaration2);

        uniformDeclaration1 = uniformDeclaration2;
        QCOMPARE(uniformDeclaration1, uniformDeclaration2);
    }

    void canNormalizeArrayUniformDeclaration()
    {
        UniformDeclaration uniformDeclaration(1, 10, GL_FLOAT_MAT2, "array[0]");
        QCOMPARE(uniformDeclaration.getName().c_str(), "array");

        uniformDeclaration = UniformDeclaration(1, 10, GL_FLOAT_MAT2, "anotherArray");
        QCOMPARE(uniformDeclaration.getName().c_str(), "anotherArray");
    }
};

TEST_DECL(UniformDeclarationTest)
#include "uniformdeclarationtest.moc"
