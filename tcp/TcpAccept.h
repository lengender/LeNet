/*************************************************************************
	> File Name: TcpAccept.h
	> Author: 
	> Mail: 
	> Created Time: 2017年04月08日 星期六 16时11分30秒
 ************************************************************************/

#ifndef _TCPACCEPT_H
#define _TCPACCEPT_H

#include"EventLoop.h"
#include"Socket.h"

namespace LeNet{
    class TcpAccept{
    public:
        TcpAccept(EventLoop *loop, SocketAddr &addr);
        ~TcpAccept();
        void listen();
        bool isListen();
        void setNewConnectCallback(const boost::function<void(int, const SocketAddr&)> &callback);
    private:
        EventLoop *eventLoop;
        boost::shared_ptr<Socket> socket;
        boost::shared_ptr<Event> event;
        bool listening;
        void acceptHandler();
        boost::function<void(int sockfd, const SocketAddr &)> newConnectCallback;
    };
}
#endif
