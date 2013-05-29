#ifndef TEST_H
#define TEST_H

#include <QtTest/QtTest>
#include <QApplication>

#define TEST_DECL(name) \
int run##name(int argc, char** argv) \
{ \
    name test; \
    return QTest::qExec(&test, argc, argv); \
}

#define TEST_INCLUDE(name) \
int run##name(int argc, char** argv);

#define START_TEST_SUITE \
int main(int argc, char** argv) { \
    QApplication app(argc, argv); \
    int retval(0);

#define TEST(name) \
    retval += run##name(argc, argv);

#define END_TEST_SUITE \
    return (retval ? 1 : 0); \
}


#endif // TEST_H
