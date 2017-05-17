/*************************************************************************
	> File Name: EventCtrl.h
	> Author: 
	> Mail: 
	> Created Time: 2017年04月07日 星期五 16时40分17秒
 ************************************************************************/

#ifndef _EVENTCTL_H
#define _EVENTCTL_H

#include<map>
#include<vector>
#include<boost/weak_ptr.hpp>
#include"Epoll.h"
#include"Event.h"

namespace LeNet{

    class EventLoop;

    class EventCtl{
    public:
        EventCtl(EventLoop *loop);
        ~EventCtl();

        void addEvent(boost::shared_ptr<Event> event);
        void removeEvent(boost::shared_ptr<Event> event);
        void modifyEvent(boost::shared_ptr<Event> event);
        void modifyEvent(int fd);
        void removeEvent(int fd);
        void waitAndRunHandler(int timeout);
    private:
        static const int activeEventLength;
        std::map<int, boost::weak_ptr<Event>> eventPool;

        Epoll epoll;
        EventLoop *loop;
        std::vector<struct epoll_event> activeEvents;
    };
    
}
#endif
