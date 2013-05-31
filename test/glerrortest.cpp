#include <test.h>
#include <glerror.h>

class GlErrorTest : public QObject
{
    Q_OBJECT

private slots:

    void cannotDetectErrorAtInit()
    {
        // force openGL error
        glCompileShader(1000);
        GlError glError;

        QVERIFY(!glError.hasOccured());
        QCOMPARE(glError.getErrorFlag(), (GLenum)GL_NO_ERROR);
        std::string message = glError.toString("cannotDetectErrorAtInit");
        QCOMPARE(message.c_str(), "cannotDetectErrorAtInit: No error detected.");
    }

    void canDetectErrorAfterInit()
    {
        GlError glError;

        // force openGL error
        glCompileShader(1000);

        QVERIFY(glError.hasOccured());
        QCOMPARE(glError.getErrorFlag(), (GLenum)GL_INVALID_VALUE);
        std::string message = glError.toString("canDetectErrorAfterInit");
        QCOMPARE(message.c_str(), "canDetectErrorAfterInit: A numeric argument is out of range.");
    }
};

TEST_DECL(GlErrorTest)
#include "glerrortest.moc"
