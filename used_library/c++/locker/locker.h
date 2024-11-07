#ifndef __LOCKER_H__
#define __LOCKER_H__

#include <semaphore.h>
#include <exception>
#include<pthread.h>


class Sem   // 信号量
{
public:
    Sem()
    {
        int ret = sem_init(&m_sem , 0 , 0); // 信号量个数为0
        if(ret == -1)
        {
            throw std::exception(); // 抛出异常信息
        }
    }

    Sem(int num)
    {
        int ret = sem_init(&m_sem , 0 , num); // 信号量个数为num
        if(ret == -1)
        {
            throw std::exception(); // 抛出异常信息
        }        
    }

    ~Sem()
    {
        int ret = sem_destroy(&m_sem);
        if(ret == -1)
        {
            throw std::exception(); // 抛出异常信息
        }        
    }

    bool Sem_wait() // 申请信号量
    {
        return sem_wait(&m_sem) == 0;   // 申请成功返回0
    }

    bool Sem_post() // 释放信号量
    {
        return sem_post(&m_sem) == 0; // 申请成功返回0
    }


private:
    sem_t m_sem;
};

class Locker_mutex  // 互斥锁
{
public:
    Locker_mutex()
    {
        int ret = pthread_mutex_init(&m_mutex , NULL);
        if(ret == -1)
        {
            throw std::exception(); // 抛出异常信息
        }
    }

    ~Locker_mutex()
    {
        int ret = pthread_mutex_destroy(&m_mutex);
        if(ret == -1)
        {
            throw std::exception(); // 抛出异常信息
        }
    }

    bool Locker()   // 加锁
    {
        return pthread_mutex_lock(&m_mutex) == 0;
    }

    bool UnLocker() // 解锁
    {
        return pthread_mutex_unlock(&m_mutex) == 0;
    }

    pthread_mutex_t* Get_mutex() // 获取锁
    {
        return &m_mutex;
    }


private:
    pthread_mutex_t m_mutex;
};

class Cond  // 条件变量
{
public:
    Cond()
    {
        int ret = pthread_cond_init(&m_cond , NULL);
        if(ret != 0)
        {
            throw std::exception(); // 抛出异常信息
        }
    }

    ~Cond()
    {
        int ret = pthread_cond_destroy(&m_cond);
        if(ret != 0)
        {
            throw std::exception(); // 抛出异常信息
        }        
    }

    /**
     * 线程阻塞等待条件变量
     * true：得到条件变量
     * false：没有得到条件变量
     */
    bool Wait(pthread_mutex_t mutex) // 线程等待
    {
        // pthread_mutex_lock(&mutex); // 加锁

        int ret = pthread_cond_wait(&m_cond , &mutex);   // 解锁当前的线程的锁 ， 进入等待
        
        // pthread_mutex_unlock(&mutex); // 解锁
        
        return ret == 0; 
    }

    bool Wait_time(pthread_mutex_t mutex , struct timespec t)    // 设置线程等待的时间
    {
        int ret = pthread_cond_timedwait(&m_cond , &mutex , &t);

        return ret == 0;
    }

    bool Signal()   // 唤醒单个线程
    {
        return pthread_cond_signal(&m_cond) == 0;
    }

    bool BroadCast()    // 唤醒所有等待的线程
    {
        return pthread_cond_broadcast(&m_cond) == 0;
    }


private:
    pthread_cond_t m_cond;
};


#endif