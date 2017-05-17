/*************************************************************************
	> File Name: EventLoopThread.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年04月08日 星期六 13时00分34秒
 ************************************************************************/

#include"EventLoopThread.h"
#include<iostream>

using namespace LeNet;
using namespace std;

EventLoopThread::EventLoopThread()
: loop(NULL), condition(mutex){

}


EventLoopThread::~EventLoopThread(){
    
}

EventLoop * EventLoopThread::getLoopInThread(){
    {
        MutexGuard lock(mutex);
        //需要阻塞等待线程初始化
        if(loop == NULL){
            //如果线程执行到这里还没有wait, loop被初始化了，并执行类notify,
            //那么这里一直阻塞，所以需要加锁
            condition.wait();
        }
    }

    return loop;
}

void EventLoopThread::run(){
    EventLoop loop;
    {
        MutexGuard lock(mutex);
        this->loop = &loop;
        condition.notify();
    }

    loop.run();
}

