/*************************************************************************
	> File Name: TcpAccept.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年04月08日 星期六 16时16分02秒
 ************************************************************************/

#include"TcpAccept.h"
#include<boost/thread/thread.hpp>

using namespace LeNet;

TcpAccept::TcpAccept(EventLoop *loop, SocketAddr &addr)
:eventLoop(loop),
 socket(new Socket(SocketOperation::createNonblockingSocket())),
 event(new Event(loop, socket->getFd())),
 listening(false)
{
    loop->addEvent(event);
    socket->bind(addr);
    event->setReadFunc(boost::bind(&TcpAccept::acceptHandler, this));
}

TcpAccept::~TcpAccept(){
    event->disableAll();
    event->removeFromLoop();
}

void TcpAccept::listen(){
    socket->listen();
    event->enableReading(true);
}

bool TcpAccept::isListen(){
    return listening;
}

void TcpAccept::setNewConnectCallback(
    const boost::function<void(int sockfd, const SocketAddr&)> &callback){
        newConnectCallback = callback;
    }

void TcpAccept::acceptHandler(){
    SocketAddr connectAddr;
    int fd;

    if((fd = socket->accept(connectAddr)) > 0){
        if(newConnectCallback){
            newConnectCallback(fd, connectAddr);
        }
        else{
            ::close(fd);
        }
    }
}
