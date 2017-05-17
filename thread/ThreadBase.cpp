/*************************************************************************
	> File Name: ThreadBase.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年04月08日 星期六 12时50分34秒
 ************************************************************************/

#include"ThreadBase.h"
using namespace LeNet;

ThreadBase::ThreadBase()
    :started(false), condition(mutex),
    thread(new boost::thread(boost::bind(&ThreadBase::beginRun, this)))
    {

    }

ThreadBase::~ThreadBase(){
    
}

void ThreadBase::start(){
    {
        MutexGuard lock(mutex);
        started = true;
        condition.notify();
    }
}

void ThreadBase::beginRun(){
    {
        MutexGuard lock(mutex);
        //如果wait函数执行前执行notify则不会有效，所以先判断started状态
        if(!started){
            condition.wait();
        }
    }

    run();
}

bool ThreadBase::isStarted(){
    return started;
}

void ThreadBase::sleepSeconds(int seconds){
    boost::this_thread::sleep(boost::posix_time::seconds(seconds));
}

boost::thread::id ThreadBase::getNowThreadId(){
    return boost::this_thread::get_id();
}

