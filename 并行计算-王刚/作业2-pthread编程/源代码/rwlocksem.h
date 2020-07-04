#ifndef RWLOCKSEM_H
#define RWLOCKSEM_H

#include <semaphore.h>

class RWlockSem
{
public:
    RWlockSem();
    void wlock();
    void uwlock();
    void rlock();
    void urlock();

private:
    //基于信号量的读写锁
    sem_t r_sem;     //定义信号量
    sem_t w_sem;     //定义信号量
    int semReaders = 0;

};

#endif // RWLOCKSEM_H
