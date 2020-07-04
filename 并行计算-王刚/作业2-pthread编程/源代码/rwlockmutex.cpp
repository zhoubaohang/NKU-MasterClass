#include "rwlockmutex.h"

RWLockMutex::RWLockMutex()
{

}

void RWLockMutex::wlock()
{
    pthread_mutex_lock(&w_mutex);
}

void RWLockMutex::uwlock()
{
    pthread_mutex_unlock(&w_mutex);
}

void RWLockMutex::rlock()
{
    pthread_mutex_lock(&r_mutex);
    if (mutexReaders == 0)
        pthread_mutex_lock(&w_mutex);
    mutexReaders++;
    pthread_mutex_unlock(&r_mutex);
}

void RWLockMutex::urlock()
{
    pthread_mutex_lock(&r_mutex);
    mutexReaders--;
    if (mutexReaders == 0)
        pthread_mutex_unlock(&w_mutex);
    pthread_mutex_unlock(&r_mutex);
}
