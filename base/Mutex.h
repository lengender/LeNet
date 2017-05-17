/*************************************************************************
	> File Name: Mutex.h
	> Author: 
	> Mail: 
	> Created Time: 2017年05月17日 星期三 10时21分03秒
 ************************************************************************/

#ifndef _MUTEX_H
#define _MUTEX_H

#include"CurrentThread.h"
#include<boost/noncopyable.hpp>
#include<assert.h>
#include<pthread.h>

#ifdef CHECK_PTHREAD_RETURN_VALUE

#ifdef NDEBUG
__BEGIN_DECLS
extern void __assert_perror_fail(
    int errnum,
    const char *file,
    unsigned int line,
    const char *function
    )
__THROW __attribute__ ((__noreturn__));
__END_DECLS
#endif

#define MCHECK(ret) ( { __typeof__ (ret) errnum = (ret);    \
                      if(__builtin_expect(errnum != 0, 0))  \
                      __assert_perror_fail(errnum, __FILE__, __LINE__, __func__);} )
#else 

#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);     \
                     assert(errnum == 0);  (void) errnum;})
#endif

class MutexLock : boost:noncopyable
{
public:
    MutexLock()
        :holder_(0)
    {
        MCHECK(pthread_mutex_init(&mutex_, NULL));
    }

    ~MutexLock(){
        assert(holder_ == 0);
        MCHECK(pthread_mutex_destroy(&mutex_));
    }

    //must be called when lockecd, i.e. for assertion
    bool isLockedByThisThread() const{
        return holder_ == CurrentThread::tid();
    }

    void assertLocked() const{
        assert(isLockedByThisThread());
    }

    //internal usage
    void lock(){
        MCHECK(pthread_mutex_lock(&mutex));
        assignHolder();
    }

    void unlock(){
        unassignHolder();
        MCHECK(pthread_mutex_unlock(&mutex));
    }
    
    pthread_mutex_t *getPthreadMutex()  //non-const
    {
        return &mutex_;
    }

private:
    friend class Condition;

    class UnassignGuard : boost::noncopyable
    {
    public:
        UnassignGuard(MutexLock &owner)
            : owner_(owner)
        {
            owner_.unassignHolder();
        }

        ~UnassignGuard(){
            owner_.assignHolder();
        }
    private:
        MutexLock &owner_;
    };

    void unassignHolder(){
        holder_ = 0;
    }

    void assignHolder(){
        holder_ = CurrentThread::tid();
    }

    pthread_mutex_t mutex_;
    pid_t holder_;
};

/*
 * Use as a stack variable
 * int Foo::size() const{
 *  MutexLockGuard lock(mutex_);
 *  return data.size();
 * }
 */

class MutexLockGuard : boost::noncopyable
{
public:
    explicit MutexLockGuard(MutexLock &mutex)
    : mutex_(mutex){
        mutex_.lock();
    }

    ~MutexLockGuard(){
        mutex_.unlock();
    }

private:
    MutexLock &mutex_;
};

//Prevent misuse like
//MutexLockGuard(mutex_)
//a tempory object doesn't hold the lock for long
#define MutexLockGuard(x) error "Missing guard object name"

#endif
