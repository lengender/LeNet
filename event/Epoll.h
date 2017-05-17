/*************************************************************************
	> File Name: Epoll.h
	> Author: 
	> Mail: 
	> Created Time: 2017年04月05日 星期三 19时36分40秒
 ************************************************************************/

#ifndef _EPOLL_H
#define _EPOLL_H

#include<boost/noncopyable.hpp>
#include<sys/epoll.h>

namespace LeNet{
    
    class Epoll : boost::noncopyable{
    public:
        Epoll();
        ~Epoll();
        bool addEvent(Event *ev);
        bool removeEvent(Event *ev);
        bool modifyEvent(Event *ev);
        bool removeEvent(int fd);
        int waitEvent(struct epoll_event *epollfd, int eventSize, int timeout);
    private:
        int epollfd;
        int epollCtl(int operator, int fd, int events);
    };
}
#endif
