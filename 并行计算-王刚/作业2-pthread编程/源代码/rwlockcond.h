#ifndef RWLOCKCOND_H
#define RWLOCKCOND_H

#include <pthread.h>

class RWlockCond
{
public:
    RWlockCond();
    void wlock();
    void uwlock();
    void rlock();
    void urlock();

private:
    //基于条件变量的读写锁
    pthread_mutex_t  rwlock_cond_mutex = PTHREAD_MUTEX_INITIALIZER;      //定义和初始化互斥锁
    pthread_cond_t  rwlock_cond = PTHREAD_COND_INITIALIZER;       //定义和初始化条件变量
    int condReaders = 0;     //记录读者的个数
    int condWriters = 0;     //记录写者的个数
};

#endif // RWLOCKCOND_H
