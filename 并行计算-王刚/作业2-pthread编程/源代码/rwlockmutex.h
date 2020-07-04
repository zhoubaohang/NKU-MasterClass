#ifndef RWLOCKMUTEX_H
#define RWLOCKMUTEX_H
#include <pthread.h> //多线程、互斥锁所需头文件

class RWLockMutex
{
public:
    RWLockMutex();
    void wlock();   //写锁
    void uwlock();  //解除写锁
    void rlock();   //读锁
    void urlock();  //解除读锁

private:
    int mutexReaders = 0;
    //基于互斥锁的读写锁
    //定义和初始化互斥锁
    pthread_mutex_t r_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t w_mutex = PTHREAD_MUTEX_INITIALIZER;
};

#endif // RWLOCKMUTEX_H
