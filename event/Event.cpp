/*************************************************************************
	> File Name: Event.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年04月07日 星期五 16时02分05秒
 ************************************************************************/

#include<sys/epoll.h>
#include<unistd.h>
#include"Event.h"

using namespace LeNet;
using namespace boost;

//EPOLLIN普通数据可读，EPOLLPRI带外数据可读
const int Event::EV_READ = EPOLLIN | EPOLLPRI;
const int Event::EV_WRITE = EPOLLOUT;
const int Event::EV_ERROR = EPOLLERR;

Event::Event(EventLoop *loop, int fd)
    : eventLoop(loop), eventfd(fd), events(0)
    {
        
    }

Event::~Event(){
    
}

void Event::enableReading(bool isEnable){
    if(isEnable)
        events |= EV_READ;
    else
        events &= ~EV_READ;

    updateEventfds();
}

void Event::enableWriteing(bool isEnable){
    if(isEnable)
        events |= EV_WRITE;
    else
        events &= ~EV_WRITE;

    updateEventfds();
}

void Event::enableErrorEvent(bool isEnable){
    if(isEnable)
        events |= EV_ERROR;
    else
        events &= ~EV_ERROR;

    updateEventfds();
}

void Event::disableAll(){
    events = 0;

    updateEventfds();
}

bool Event::isReading(){
    return events & EV_READ;
}

bool Event::isWriteing(){
    return events & EV_WRITE;
}

int Event::getFd(){
    return eventfd;
}

uint32_t Event::getEvents(){
    return events;
}

void Event::setReadFunc(const function<void()> &func){
    readHandler = func;
}

void Event::setWriteFunc(const function<void()> &func){
    writeHanlder = func;
}

void Event::setErrorFunc(const function<void()> &func){
    errorHandler = func;
}

void Event::setCloseFunc(const function<void()> &func){
    closeHandler = func;   
}

void Event::updateEventfds(){
    eventLoop->modifyEvent(eventfd);
}

void Event::removeFromLoop(){
    eventLoop->removeEvent(eventFd);
}

void Event::handler(uint32_t revents){
    if((revents & EPOLLHUP) && !(revents & EPOLLIN)){
        if(closeHandler)
            closeHandler();
    }

    if(revents & EPOLLERR){
        if(errorHandler)
            errorHandler();
    }
    
    if(revents & (EPOLLIN | EPOLLPRI | EPOLLHUP)){
        if(readHandler)
            readHandler();
    }

    if(revents & EPOLLOUT){
        if(writeHanlder)
            writeHanlder();
    }
}
