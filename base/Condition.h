/*************************************************************************
	> File Name: Condition.h
	> Author: 
	> Mail: 
	> Created Time: 2017年05月17日 星期三 10时55分41秒
 ************************************************************************/

#ifndef _CONDITION_H
#define _CONDITION_H

#include "Mutex.h"

#include<boost/noncopyable.hpp>
#include<pthread.h>

class Condition : boost::noncopyable{
public:
    explicit Condition(MutexLock& mutex)
    mutex_(mutex){
        MCHECK(pthread_cond_init(&pcond_, NULL));
    }

    ~Condition(){
        MCHECK(pthread_cond_destroy(&pcond_));
    }

    void wait(){
        MutexLock::UnassignGuard ug(mutex_);
        MCHECK(pthread_cond_wait(&pcond_, mutex_.getPthreadMutex()));
    }

    //return true if time out, false otherwise.
    bool waitForSeconds(double seconds);

    void notify(){
        MCHECK(pthread_cond_signal(&pcond_));
    }

    void notifyAll(){
        MCHECK(pthread_cond_broadcast(&pcond_));
    }

private:
    MutexLock & mutex_;
    pthread_cond_t pcond_;
};

#endif
