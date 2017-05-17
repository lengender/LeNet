/*************************************************************************
	> File Name: Event.h
	> Author: 
	> Mail: 
	> Created Time: 2017年04月07日 星期五 15时38分58秒
 ************************************************************************/

#ifndef _EVENT_H
#define _EVENT_H

#include<boost/function.hpp>

namespace LeNet{
    class EventLoop;

    class Event{
    public:
        static const int EV_READ;
        static const int EV_WRITE;
        static const int EV_ERROR;

        Event(EventLoop *loop, int fd);
        ~Event();
        int getFd();
        uint32_t getEvents();

        void enableReading(bool isEnable);
        void enableWriteing(bool isEnable);
        void enableErrorEvent(bool isEnable);
        void disableAll();
        bool isWriteing();
        bool isReading();
        void removeFromLoop();

        void handler(uint32_t revents);

        void setReadFunc(const boost::function<void()> &func);
        void setWriteFunc(const boost::function<void()> &func);
        void setErrorFunc(const boost::function<void()> &func);
        void setCloseFunc(const boost::function<void()> &func);

    private:
        EventLoop *eventLoop;
        int eventfd;
        uint32_t events;
        boost::function<void()> readHandler;
        boost::function<void()> writeHandler;
        boost::function<void()> errorHandler;
        boost::function<void()> closeHandler;
        updateEventfds();
    };
}
#endif
