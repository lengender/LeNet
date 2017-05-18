/*************************************************************************
	> File Name: AsyncLogging.h
	> Author: 
	> Mail: 
	> Created Time: 2017年05月18日 星期四 15时12分40秒
 ************************************************************************/

#ifndef _ASYNCLOGGING_H
#define _ASYNCLOGGING_H

#include"CountDownLatch.h"
#include"Mutex.h"
#include"Thread.h"

#include<boost/bind.hpp>
#include<boost/noncopyable.hpp>
#include<boost/scoped_ptr.hpp>
#include<boost/ptr_container/ptr_vector.hpp>

class AsyncLogging : boost::noncopyable
{
public:
    AsyncLogging(const string& basename,
                size_t rollSize,
                int flushInterval = 3);

    ~AsyncLooging(){
        if(running_){
            stop();
        }
    }

    void append(const char* logline, int len);

    void start(){
        running_ = true;
        thread_.start();
        latch_.wait();
    }

    void stop(){
        running_ = false;
        cond_.notify();
        thread_.join();
    }

private:

    //declare but not define, prevent compiler-synthesized functions
    AsyncLogging(const AsyncLogging&);   //ptr_container
    void operator=(const AsyncLogging&);  //ptr_container

    void threadFunc();

    typedef FixedBuffer<kLargeBuffer> Buffer;
    typedef boost::ptr_vector<BUffer> BufferVector;
    typedef BufferVector::auto_type BufferPtr;

    const int flushInterval_;
    bool running_;
    string basename_;
    size_t rollSize_;
    Thread thread_;
    CountDownLatch latch_;
    MutexLock mutex_;
    Condition cond_;
    BufferPtr currentBuffer_;
    BufferPtr nextBuffer_;
    BufferVector buffers_;
}
#endif
