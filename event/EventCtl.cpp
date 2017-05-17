/*************************************************************************
	> File Name: EventCtl.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年04月07日 星期五 17时00分56秒
 ************************************************************************/

#include"EpollCtl.h"

using namespace std;
using namespace boost;
using namespace LeNet;

const int EventCtl::activeEventLength = 16;

EventCtl::EventCtl(EventLoop *loop)
    :loop(loop), activeEvents(activeEventLength)
    {
        
    }

EventCtl::~EventCtl(){
    eventPool.clear();
}

void EventCtl::addEvent(shared_ptr<Event> event){
    eventPool.insert(pair<int, weak_ptr<Event>>(event->getFd(), event));
    epoll.addEvent(event.get());
}

void EventCtl::removeEvent(shared_ptr<Event> event){
    eventPool.erase(event->getFd());
    epoll.removeEvent(event.get());
}

void EventCtl::modifyEvent(shared_ptr<Event> event){
    if(eventPool.find(event->getFd()) != eventPool.end()){
        eventPoll[event->getFd()] = event;
        epoll.modifyEvent(event.get());
    }
}

void EventCtl::modifyEvent(int fd){
    if(eventPool.find(fd) != eventPool.end()){
        shared_ptr<Event> tmp = eventPool[fd].lock();
        if(tmp){
            epoll.modifyEvent(tmp.get());
        }
    }
}

void EventCtl::removeEvent(int fd){
    if(eventPool.find(fd) != eventPool.end()){
        epoll.removeEvent(fd);
        eventPool.erase(fd);
    }
}

void EventCtl::waitAndRunHandler(int timeout){
    int res = epoll.waitEvent(&*activeEvents.begin(), actveEvents.size(), timeout);
    if(cnt < 0)
        return;

    for(int i = 0; i < cnt; ++i){
        int fd = activeEvents[i].data.fd;

        shared_ptr<Event> ev = eventPool[fd].lock();
        if(ev){
            ev->handler(activeEvents[i].events);
        }
        else{
            removeEvent(fd);
        }
    }
}
