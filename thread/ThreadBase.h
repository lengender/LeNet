/*************************************************************************
	> File Name: ThreadBase.h
	> Author: 
	> Mail: 
	> Created Time: 2017年04月08日 星期六 11时42分33秒
 ************************************************************************/

#ifndef _THREADBASE_H
#define _THREADBASE_H

#include<boost/thread.hpp>
#include<boost/shared_ptr.hpp>
#include"Condition.h"

namespace LeNet{
    class ThreadBase{
    public:
        ThreadBase();
        virtual ~ThreadBase();
        void start();
        static boost::thread::id getNowThreadId();
        bool isStarted();
        virtual void run() = 0;
    protected:
        void sleepSeconds(int seconds);
    private:
        void beginRun();
        bool started;
        Mutex mutex;
        Condition condition;
        boost::shared_ptr<boost::thread> thread;
    };
}
#endif
