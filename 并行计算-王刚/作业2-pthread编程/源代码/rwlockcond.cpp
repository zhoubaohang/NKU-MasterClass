#include "rwlockcond.h"

RWlockCond::RWlockCond()
{

}

void RWlockCond::wlock()
{
    pthread_mutex_lock(&rwlock_cond_mutex);     //加锁
    while (condWriters != 0 || condReaders > 0)
    {
        pthread_cond_wait(&rwlock_cond, &rwlock_cond_mutex);      //等待条件变量的成立
    }
    condWriters = 1;

    pthread_mutex_unlock(&rwlock_cond_mutex);
}

void RWlockCond::uwlock()
{
    pthread_mutex_lock(&rwlock_cond_mutex);
    condWriters = 0;
    pthread_cond_broadcast(&rwlock_cond);       //唤醒其他因条件变量而产生的阻塞
    pthread_mutex_unlock(&rwlock_cond_mutex);    //解锁
}

void RWlockCond::rlock()
{
    pthread_mutex_lock(&rwlock_cond_mutex);
    while (condWriters != 0)
    {
        pthread_cond_wait(&rwlock_cond, &rwlock_cond_mutex);      //等待条件变量的成立
    }
    condReaders++;
    pthread_mutex_unlock(&rwlock_cond_mutex);
}

void RWlockCond::urlock()
{
    pthread_mutex_lock(&rwlock_cond_mutex);
    condReaders--;
    if (condReaders == 0)
        pthread_cond_broadcast(&rwlock_cond);       //唤醒其他因条件变量而产生的阻塞
    pthread_mutex_unlock(&rwlock_cond_mutex);    //解锁
}
