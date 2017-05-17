/*************************************************************************
	> File Name: EventLoopThread.h
	> Author: 
	> Mail: 
	> Created Time: 2017年04月08日 星期六 12时57分24秒
 ************************************************************************/

#ifndef _EVENTLOOPTHREAD_H
#define _EVENTLOOPTHREAD_H

#include"../thread/Condition.h"
#include"../thread/ThreadBase.h"
#include"EventLoop.h"

namespace LeNet{
    class EventLoopThread : public ThreadBase{
    public:
        EventLoopThread();
        virtual void run();
        EventLoop *getLoopInThread();
    private:
        EventLoop *loop;
        Mutex mutex;
        Condition condition;
    };
}
#endif
