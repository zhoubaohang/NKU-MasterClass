#include "paralleltest.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ParallelTest();

    TestSeries();
    TestMutex();
    TestRWlock();
    TestRWlockSem();
    TestRWlockCond();
    TestRWlockMutex();

    return a.exec();
}
