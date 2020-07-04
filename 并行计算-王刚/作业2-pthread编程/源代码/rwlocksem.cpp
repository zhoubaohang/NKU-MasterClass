#include "rwlocksem.h"

RWlockSem::RWlockSem()
{
    sem_init(&r_sem, 0, 1);     //初始化信号量
    sem_init(&w_sem, 0, 1);     //初始化信号量
}

void RWlockSem::wlock()
{
    sem_wait(&w_sem);
}

void RWlockSem::uwlock()
{
    sem_post(&w_sem);
}

void RWlockSem::rlock()
{
    sem_wait(&r_sem);
    if (semReaders == 0)
        sem_wait(&w_sem);
    semReaders++;
    sem_post(&r_sem);
}

void RWlockSem::urlock()
{
    sem_wait(&r_sem);
    semReaders--;
    if (semReaders == 0)
        sem_post(&w_sem);
    sem_post(&r_sem);
}
