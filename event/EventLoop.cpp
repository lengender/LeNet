/*************************************************************************
	> File Name: EventLoop.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年04月07日 星期五 16时30分44秒
 ************************************************************************/

#include<signal.h>
#include"EventLoop.h"
#include"Log.h"
#include<iostream>

using namespace std;
using namespace LeNet;
using namespace boost;

class IgnoreSigPipe{
public:
    static IgnoreSigPipe *geetInstance(){
        return sig;
    }
private:
    IgnoreSigPipe(){
        ::signal(SIGPIPE, SIG_IGN);
    }

    static IgnoreSigPipe *sig;
};

IgnoreSigPipe* IgnoreSigPipe::sig = new IgnoreSigPipe();


const int EventLoop::PollTimeMs = 3000;

EventLoop::EventLoop()
    : eventCtl(new EventCtl(this))
{}


EventLoop::~EventLoop(){
    delete eventCtl;
}

void EventLoop::run(){
    if(!inThisThread){
        thisThreadId = Thread::getNowThreadId();
        Log::Inst()->log("Loop create and run in different thread.");
    }

    while(true){
        eventCtl->waitAndRunHandler(PollTimeMs);
        runAllFunctionInLoop();
    }
}

void EventLoop::addEvent(shared_ptr<Event> event){
    eventCtl->addEvent(event);
}

void EventLoop::removeEvent(shared_ptr<Event> event){
    eventCtl->removeEvent(event);
}

void EventLoop::removeEvent(int fd){
    eventCtl->removeEvent(fd);
}

void EventLoop::modifyEvent(shared_ptr<Event> event){
    eventCtl->modifyEvent(event);
}

void EventLoop::modifyEvent(int fd){
    eventCtl->modifyEvent(fd);
}

void EventLoop::runInLoop(const function<void()> &func){
    if(inThisThread()){
        func();
    }
    else{
        addFunInLoop(func);
    }
}

void EventLoop::addFunInLoop(function<void()> func){
    MutexGuard lock(mutex);
    functions.push_back(func);
}

void EventLoop::runAllFunctionInLoop(){
    MutexGuard lock(mutex);
    for(int i = 0; i < functions.size(); ++i){
        functions[i]();
    }
    functions.clear();
}

bool EventLoop::inThisThread(){
    return (thisThreadId == Thread::getNowThreadId());
}

