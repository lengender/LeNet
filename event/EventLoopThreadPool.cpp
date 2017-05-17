/*************************************************************************
	> File Name: EventLoopThreadPool.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年04月08日 星期六 13时30分25秒
 ************************************************************************/

#include"EventLoopThreadPool.h"

using namespace LeNet;

EventLoopThreadPool::EventLoopThreadPool(EventLoop *loop)
    : mainLoop(loop), started(false), threadNum(0), threadIndex(0)
    {

    }

void EventLoopThreadPool::init(){
    for(int i = 0; i < threadNum; ++i){
        shared_ptr<EventLoopThread> thread(new EventLoopThread());
        thread->start();
        eventLoopThreadPool.push_back(thread);
        eventLoopPool.push_back(thread->getLoopInThread());
    }
    started = true;
}

void EventLoopThreadPool::init(int num){
    setThreadNum(num);
    init();
}

bool EventLoopThreadPool::isStarted(){
    return started;
}

void EventLoopThreadPool::setThreadNum(uint16_t num){
    threadNum = num;
}

EventLoop *EventLoopThreadPool::getOneLoopFromPool(){
    //如果线程池为空，则返回主loop
    if(eventLoopPool.empty())
        return mainLoop;

    threadIndex++;
    if(threadIndex >= eventLoopPool.size())
        threadIndex = 0;

    return eventLoopPool[threadIndex];
}

EventLoop* EventLoopThreadPool::getOneLoopFromPoolWithHash(int hashCode){
    //如果线程池为空，则返回主loop
    if(eventLoopPool.empty())
        return mainLoop;

    uint16_t index = hashCode % eventLoopPool.size();
    return eventLoopPool[index];
}

