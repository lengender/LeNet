/*************************************************************************
	> File Name: CountDownLatch.h
	> Author: 
	> Mail: 
	> Created Time: 2017年05月17日 星期三 11时20分29秒
 ************************************************************************/

#ifndef _COUNTDOWNLATCH_H
#define _COUNTDOWNLATCH_H

#include "Condition.h"
#include "Mutex.h"
#include <boost/noncopyable.hpp>

class CountDownLatch : boost::noncopyable
{
public:
    explicit CountDownLatch(int count);

    void wait();

    void countDown();

    int getCount() const;

private:
    mutable MutexLock mutex_;
    Condition condition_;
    int count_;
};
#endif
