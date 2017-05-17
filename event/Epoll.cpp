/*************************************************************************
	> File Name: Epoll.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年04月07日 星期五 16时43分00秒
 ************************************************************************/

#include"Epoll.h"
#include"Log.h"
#include<string.h>
#include<unistd.h>

using namespace LeNet;

Epoll::Epoll()
:epollfd(epoll_create1(EPOLL_CLOEXEC)){
    if(epollfd < 0){
        Log::Inst()->log("create epoll error.");
    }
}

Epoll::~Epoll(){
    ::close(epollfd);
}

int Epoll::epollCtl(int ope, int fd, int events){
    struct epoll_event event;
    bzero(&event, sizeof(event));

    event.events = events;
    event.data.fd = fd;
    int res = epoll_ctl(epollfd, ope, fd, &event);
    return res;
}

bool Epoll::addEvent(Event *ev){
    if(epollCtl(EPOLL_CTL_ADD, ev->getFd(), ev->getEvents()) < 0){
        Log::Inst()->log("add epoll error.");
        return false;
    }

    return true;
}

bool Epoll::removeEvent(Event *ev){
    if(epollCtl(EPOLL_CTL_DEL, ev->getFd(), ev->getEvents()) < 0){
        Log::Inst()->log("delete epoll error.");
        return false;
    }

    return true;
}

bool Epoll::removeEvent(int fd){
    if(epollCtl(EPOLL_CTL_DEL, fd, 0) < 0){
        Log::Inst()->log("delete epoll error.");
        return false;
    }

    return true;
}

bool Epoll::modifyEvent(Event *ev){
    if(epollCtl(EPOLL_CTL_MOD, ev->getFd(), ev->getEvents()) < 0){
        Log::Inst()->log("modify epoll error.");
        return false;
    }

    return true;
}

bool Epoll::waitEvent(struct epoll_event *eventList, int eventSize, int timeout){
    int res = epoll_wait(epollfd, eventList, eventSize, timeout);

    return res;
}
