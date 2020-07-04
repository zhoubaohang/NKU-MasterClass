#ifndef PARALLELTEST_H
#define PARALLELTEST_H

#define HAVE_STRUCT_TIMESPEC

#include "linklist.h"
#include "linklist.cpp"
#include "rwlockmutex.h"
#include "rwlocksem.h"
#include "rwlockcond.h"
#include <iostream>
#include <random>
#include <ctime>
#include <Windows.h>
#include "pthread.h"

#define NUM_THREADS 4 //线程数
//数据量分布
#define NUM_READ 80000
#define NUM_ADD 10000
#define NUM_REMOVE 10000

//pthead
typedef struct
{
    int threadID;
} threadParm_t;

using namespace std;

void *threadMutexFunc(void *param);
void *threadRWlockFunc(void *param);
void *threadRWlockMutexFunc(void *param);
void *threadRWlockSemFunc(void *param);
void *threadRWlockCondFunc(void *param);
void testResult(LinkList<int> m1, LinkList<int> m2);

void ParallelTest();
void TestSeries();
void TestMutex();
void TestRWlock();
void TestRWlockSem();
void TestRWlockCond();
void TestRWlockMutex();
#endif // PARALLELTEST_H
