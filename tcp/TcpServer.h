/*************************************************************************
	> File Name: TcpServer.h
	> Author: 
	> Mail: 
	> Created Time: 2017年04月10日 星期一 10时35分17秒
 ************************************************************************/

#ifndef _TCPSERVER_H
#define _TCPSERVER_H

#include<map>
#include"../event/EventLoop.h"
#include"TcpAccept.h"
#include"TcpConnect.h"
#include"../event/EventLoopThreadPool.h"


namespace LeNet{
    class TcpServer{
    public:
        TcpServer(EventLoop *loop, SocketAddr &addr);
        virtual ~TcpServer();
        void start();
        virtual void connectCallback(boost::shared_ptr<TcpConnect> tcpConnect) = 0;
        virtual void messageCallback(boost::shared_ptr<TcpConnect>, Buffer&) = 0;
        virtual void writeCompletCallback(boost::shared_ptr<TcpConnect> tcpConnect) = 0;
        virtual void connectCloseCallback(boost::shared_ptr<TcpConnect>) = 0;

        void addConnect(std::string name, boost::shared_ptr<TcpConnect> connect);
        void addConnect(std::string name, TcpConnect *connect);
        bool haveConnect(std::string name);
        void removeConnect(std::string name);
        long getConnectionCount();

        void write(TcpConnect& connect, void *data, uint32_t length);
        void write(std::string &name, void *data, uint32_t length);
        void write(boost::shared_ptr<TcpConnect> connect, void *data, uint32_t length);
        void write(std::string &name, std::string &data);
        void setThreadPoolSize(uint16_t num);
    private:
        void newConnected(int sockfd, const SocketAddr &addr);
        EventLoop *eventLoop;
        SocketAddr tcpAddr;
        boost::shared_ptr<TcpConnect> tcpAccept;

        std::map<std::string, boost::shared_ptr<TcpConnect>> connectPool;
        void connectCloseEvent(boost::shared_ptr<TcpConnect> connect);
        AtomicInt8 isStart;
        boost::shared_ptr<EventLoopThreadPool> threadPool;
    };
}
#endif
