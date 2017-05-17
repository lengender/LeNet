/*************************************************************************
	> File Name: EventLoop.h
	> Author: 
	> Mail: 
	> Created Time: 2017年04月07日 星期五 16时24分28秒
 ************************************************************************/

#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H

#include<vector>
#include<boost/thread.hpp>
#include"Mutex.h"
#include"Event.h"

namespace LeNet{
    class EventLoop{
    public:
        EventLoop();
        ~EventLoop();

        void addEvent(boost::shared_ptr<Event> event);
        void removeEvent(boost::shared_ptr<Event> event);
        void modifyEvent(boost::shared_ptr<Event> event);
        void readmoveEvent(int fd);
        void modifyEvent(int fd);
        void run();
        void runInLoop(const boost::function<void()> &func);

    private:
        static const int PollTimeMs;
        Mutex mutex;
        EventCtl *eventCtl;

        std::vector<boost::function<void()>> functions;
        void addFunInLoop(boost::function<void()> func);
        void runAllFunctionInLoop();
    };
}
#endif
