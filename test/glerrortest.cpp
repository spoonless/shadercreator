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
        QVERIFY(glError.getErrorFlag() == GL_NO_ERROR);
        std::string message = glError.toString("cannotDetectErrorAtInit");
        QVERIFY2(message == "cannotDetectErrorAtInit: No error detected.", message.c_str());
    }

    void canDetectErrorAfterInit()
    {
        GlError glError;

        // force openGL error
        glCompileShader(1000);

        QVERIFY(glError.hasOccured());
        QVERIFY(glError.getErrorFlag() == GL_INVALID_VALUE);
        std::string message = glError.toString("canDetectErrorAfterInit");
        QVERIFY2(message == "canDetectErrorAfterInit: A numeric argument is out of range.", message.c_str());
    }
};

TEST_DECL(GlErrorTest)
#include "glerrortest.moc"
