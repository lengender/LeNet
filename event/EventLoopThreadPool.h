/*************************************************************************
	> File Name: EventLoopThreadPool.h
	> Author: 
	> Mail: 
	> Created Time: 2017年04月08日 星期六 13时25分10秒
 ************************************************************************/

#ifndef _EVENTLOOPTHREADPOOL_H
#define _EVENTLOOPTHREADPOOL_H

#include<vector>
#include<boost/shared_ptr.hpp>
#include"EventLoopThread.h"
#include"EventLoop.h"

namespace LeNet{
    class EventLoopThreadPool{
    public:
        EventLoopThreadPool(EventLoop *loop);
        void init();
        void init(int num);
        bool isStarted();
        void setThreadNum(uint16_t num);

        EventLoop *getOneFromPool();
        EventLoop *getOneFromPoolWithHash(int hashConde);
    private:
        EventLoop *mainLoop;
        bool started;
        uint16_t threadNum;
        uint32_t threadIndex;
        std::vector<boost::shared_ptr<EventLoopThread>> eventLoopThreadPool;
        std::vector<EventLoop *> eventLoopPool;
    };
}
#endif
