#include "paralleltest.h"

// 随机数生成器
random_device rd;
default_random_engine engine(rd());
uniform_int_distribution<int> dis(1, 100);

pthread_mutex_t mutex_task = PTHREAD_MUTEX_INITIALIZER;
pthread_rwlock_t rwlock_task = PTHREAD_MUTEX_INITIALIZER;
pthread_t thread[NUM_THREADS];
threadParm_t threadParm[NUM_THREADS];

RWLockMutex rwlockMutex;
RWlockSem rwlockSem;
RWlockCond rwlockCond;

long long head, freq, tail;
LinkList<int> mList_series;         //串行
LinkList<int> mList_mutex;          //互斥量
LinkList<int> mList_rwlock;         //读写锁
LinkList<int> mList_rwlock_mutex;	//互斥量实现的读写锁
LinkList<int> mList_rwlock_sem;     //信号量实现的读写锁
LinkList<int> mList_rwlock_cond;	//条件变量实现的读写锁
LinkList<int> mList_temp;           //用于统一添加删除的数据

void ParallelTest()
{
    int temp;

    //初始化链表
    for (int i = 0; i < NUM_REMOVE; i++)
    {
        temp = dis(engine);
        mList_series.add(temp);
        mList_mutex.add(temp);
        mList_rwlock.add(temp);
        mList_rwlock_mutex.add(temp);
        mList_rwlock_sem.add(temp);
        mList_rwlock_cond.add(temp);

        mList_temp.add(temp);
    }
    testResult(mList_series, mList_mutex);
    testResult(mList_series, mList_rwlock);
    testResult(mList_series, mList_rwlock_mutex);
    testResult(mList_series, mList_rwlock_sem);
    testResult(mList_series, mList_rwlock_cond);

    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    cout<<freq<<endl;
}

void TestSeries()
{
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    for (int i = 0; i < NUM_READ; i++)
    {
        mList_series.find(dis(engine));
    }

    for (int i = 0; i < NUM_ADD;i++)
    {
        mList_series.add(mList_temp.find(i));
    }

    for (int i = 0; i < NUM_REMOVE;i++)
    {
        mList_series.remove(mList_temp.find(i));
    }

    QueryPerformanceCounter((LARGE_INTEGER*)&tail);

    float series = (tail - head) * 1000.0 / freq;
    cout << "series: " << series << "ms" << endl;
}

void TestMutex()
{
    mutex_task = PTHREAD_MUTEX_INITIALIZER;
    rwlock_task = PTHREAD_RWLOCK_INITIALIZER;
    pthread_t thread[NUM_THREADS];
    threadParm_t threadParm[NUM_THREADS];

    QueryPerformanceCounter((LARGE_INTEGER*)&head);

    for (int i = 0; i<NUM_THREADS; i++)
    {
        threadParm[i].threadID = i;
        pthread_create(&thread[i], NULL, threadMutexFunc, (void*)&threadParm[i]);
    }

    for (int i = 0; i<NUM_THREADS; i++)
    {
        pthread_join(thread[i], NULL);
    }
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);

    float mutex = (tail - head) * 1000.0 / freq;
    cout << "mutex: " << mutex << "ms" << endl;
}

//互斥量测试
void *threadMutexFunc(void *parm)
{
    threadParm_t*p = (threadParm_t*)parm;

    pthread_mutex_lock(&mutex_task);
    for (int i = 0; i < NUM_ADD / NUM_THREADS;i++)
    {
        mList_mutex.add(mList_temp.find(NUM_ADD / NUM_THREADS * p->threadID + i));
    }
    pthread_mutex_unlock(&mutex_task);

    pthread_mutex_lock(&mutex_task);
    for (int i = 0; i < NUM_REMOVE / NUM_THREADS;i++)
    {
        mList_mutex.remove(mList_temp.find(NUM_REMOVE / NUM_THREADS * p->threadID + i));
    }
    pthread_mutex_unlock(&mutex_task);

    pthread_mutex_lock(&mutex_task);
    for (int i = 0; i < NUM_READ / NUM_THREADS; i++)
    {
        mList_mutex.find(dis(engine));
    }
    pthread_mutex_unlock(&mutex_task);

    return 0;
}

//读写锁测试
void *threadRWlockFunc(void *parm)
{
    threadParm_t*p = (threadParm_t*)parm;

    pthread_rwlock_rdlock(&rwlock_task);
    for (int i = 0; i < NUM_READ / NUM_THREADS; i++)
    {
        mList_rwlock.find(dis(engine));
    }
    pthread_rwlock_unlock(&rwlock_task);

    pthread_rwlock_wrlock(&rwlock_task);
    for (int i = 0; i < NUM_ADD / NUM_THREADS;i++)
    {

        mList_rwlock.add(mList_temp.find(NUM_ADD / NUM_THREADS * p->threadID + i));
    }
    pthread_rwlock_unlock(&rwlock_task);

    pthread_rwlock_wrlock(&rwlock_task);
    for (int i = 0; i < NUM_REMOVE / NUM_THREADS;i++)
    {
        mList_rwlock.remove(mList_temp.find(NUM_REMOVE / NUM_THREADS * p->threadID + i));
    }
    pthread_rwlock_unlock(&rwlock_task);

    return 0;
}

void TestRWlock()
{
    QueryPerformanceCounter((LARGE_INTEGER*)&head);

    for (int i = 0; i<NUM_THREADS; i++)
    {
        threadParm[i].threadID = i;
        pthread_create(&thread[i], NULL, threadRWlockFunc, (void*)&threadParm[i]);
    }

    for (int i = 0; i<NUM_THREADS; i++)
    {
        pthread_join(thread[i], NULL);
    }
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);

    float rwlock = (tail - head) * 1000.0 / freq;
    cout << "rwlock: " << rwlock << "ms" << endl;

    pthread_mutex_destroy(&mutex_task);
    pthread_rwlock_destroy(&rwlock_task);
}

void TestRWlockMutex()
{
    QueryPerformanceCounter((LARGE_INTEGER*)&head);

    for (int i = 0; i<NUM_THREADS; i++)
    {
        threadParm[i].threadID = i;
        pthread_create(&thread[i], NULL, threadRWlockMutexFunc, (void*)&threadParm[i]);
    }

    for (int i = 0; i<NUM_THREADS; i++)
    {
        pthread_join(thread[i], NULL);
    }
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);

    float rwlockMutex = (tail - head) * 1000.0 / freq;
    cout << "rwlockMutex: " << rwlockMutex << "ms" << endl;
}

//互斥量实现的读写锁测试
void *threadRWlockMutexFunc(void *parm)
{
    threadParm_t*p = (threadParm_t*)parm;

    rwlockMutex.rlock();
    for (int i = 0; i < NUM_READ / NUM_THREADS; i++)
    {
        mList_rwlock_mutex.find(dis(engine));
    }
    rwlockMutex.urlock();

    rwlockMutex.wlock();
    for (int i = 0; i < NUM_ADD / NUM_THREADS;i++)
    {

        mList_rwlock_mutex.add(mList_temp.find(NUM_ADD / NUM_THREADS * p->threadID + i));
    }
    rwlockMutex.uwlock();

    rwlockMutex.wlock();
    for (int i = 0; i < NUM_REMOVE / NUM_THREADS;i++)
    {
        mList_rwlock_mutex.remove(mList_temp.find(NUM_REMOVE / NUM_THREADS * p->threadID + i));
    }
    rwlockMutex.uwlock();

    return 0;
}

//信号量实现的读写锁测试
void *threadRWlockSemFunc(void *parm)
{
    threadParm_t*p = (threadParm_t*)parm;

    rwlockSem.rlock();
    for (int i = 0; i < NUM_READ / NUM_THREADS; i++)
    {
        mList_rwlock_sem.find(dis(engine));
    }
    rwlockSem.urlock();

    rwlockSem.wlock();
    for (int i = 0; i < NUM_ADD / NUM_THREADS;i++)
    {
        mList_rwlock_sem.add(mList_temp.find(NUM_ADD / NUM_THREADS * p->threadID + i));
    }
    rwlockSem.uwlock();

    rwlockSem.wlock();
    for (int i = 0; i < NUM_REMOVE / NUM_THREADS;i++)
    {
        mList_rwlock_sem.remove(mList_temp.find(NUM_REMOVE / NUM_THREADS * p->threadID + i));
    }
    rwlockSem.uwlock();

    return 0;
}

void TestRWlockSem()
{
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    for (int i = 0; i<NUM_THREADS; i++)
    {
        threadParm[i].threadID = i;
        pthread_create(&thread[i], NULL, threadRWlockSemFunc, (void*)&threadParm[i]);
    }

    for (int i = 0; i<NUM_THREADS; i++)
    {
        pthread_join(thread[i], NULL);
    }
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);

    float rwlockSem = (tail - head) * 1000.0 / freq;
    cout << "rwlockrwlockSem: " << rwlockSem << "ms" << endl;
}

//条件变量实现的读写锁测试
void *threadRWlockCondFunc(void *parm)
{
    threadParm_t*p = (threadParm_t*)parm;

    rwlockCond.rlock();
    for (int i = 0; i < NUM_READ / NUM_THREADS; i++)
    {
        mList_rwlock_cond.find(dis(engine));
    }
    rwlockCond.urlock();

    rwlockCond.wlock();
    for (int i = 0; i < NUM_ADD / NUM_THREADS;i++)
    {
        mList_rwlock_cond.add(mList_temp.find(NUM_ADD / NUM_THREADS * p->threadID + i));
    }
    rwlockCond.uwlock();

    rwlockCond.wlock();
    for (int i = 0; i < NUM_REMOVE / NUM_THREADS;i++)
    {
        mList_rwlock_cond.remove(mList_temp.find(NUM_REMOVE / NUM_THREADS * p->threadID + i));
    }
    rwlockCond.uwlock();

    return 0;
}

void TestRWlockCond()
{
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    for (int i = 0; i<NUM_THREADS; i++)
    {
        threadParm[i].threadID = i;
        pthread_create(&thread[i], NULL, threadRWlockCondFunc, (void*)&threadParm[i]);
    }

    for (int i = 0; i<NUM_THREADS; i++)
    {
        pthread_join(thread[i], NULL);
    }
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);

    float rwlockCond = (tail - head) * 1000.0 / freq;
    cout << "rwlockrwlockCond: " << rwlockCond << "ms" << endl;
}

//测试运行结果
void testResult(LinkList<int> m1, LinkList<int> m2)
{
    if (m1.size() == m2.size())
    {
        if (m1.size() == 0)
        {
            cout << "Test is successing" << endl;
            return;
        }

        for (int i = 0; i < m1.size(); i++)
        {
            if (m1.find(i) == m2.find(i));
            else
            {
                cout << "The vaule are not equal" << endl;
                return;
            }
        }
    }
    else
    {
        cout << "The length are not equal" << endl;
    }

    cout << "Test is successing" << endl;
}
