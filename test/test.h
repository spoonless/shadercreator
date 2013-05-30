#ifndef TEST_H
#define TEST_H

#include <QtTest/QtTest>

#define TEST_DECL(name) \
int run##name(int argc, char** argv) \
{ \
    name test; \
    return QTest::qExec(&test, argc, argv); \
}

#define TEST_INCLUDE(name) \
int run##name(int argc, char** argv);


#define TEST_RUN(name) \
    run##name(argc, argv)

#endif // TEST_H
