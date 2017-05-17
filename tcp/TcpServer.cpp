/*************************************************************************
	> File Name: TcpServer.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年04月10日 星期一 11时03分45秒
 ************************************************************************/

#include"TcpServer.h"
#include<boost/thread/thread.hpp>
#include"../log/Log.h"
#include<iostream>
using namespace std;
using namespace LeNet;
using namespace boost;

TcpServer::TcpServer(EventLoop *loop, SocketAddr& addr)
: eventLoop(loop), tcpAddr(addr),
    tcpAccept(new TcpAccept(loop, addr)),
    isStart(false),
    threadPool(new EventLoopThreadPool(loop))
    {
        tcpAccept->setNewConnectCallback(&TcpServer::newConnected, this, _1, _2);
    }

TcpServer::~TcpServer(){
    
}

TcpServer::setThreadPoolSize(uint16_t num){
    threadPool->setThreadNum(num);
}

void TcpServer::start(){
    threadPool->init();
    tcpAccept->listen();
    isStart.set(true);
}

void TcpServer::newConnected(int sockfd, const SocketAddr &addr){
    Log::Inst()->log("new tcp connect addr.");

    EventLoop *loop = threadPool->getOneLoopFromPool();
    shared_ptr<TcpConnect> tcpConnect(new TcpConnect(loop, addr.getAddr(), sockfd));
    addConnect(addr.toString(), tcpConnect);
    tcpConnect->setMessageCallback(boost::bind(&TcpServer::messageCallback, this, _1, _2));
    tcpConnect->setCloseCallback(boost::bind(&TcpServer::connectCloseCallback, this, -1));
    tcpConnect->connectedHandle();
    connectCallback(tcpConnect);
}

void TcpServer::addConnect(string name, shared_ptr<TcpConnect> connect){
    connectPool.insert(pair<string, shared_ptr<TcpConnect>>(name, connect));
}

void TcpServer::addConnect(string name, TcpConnect *connect){
    shared_ptr<TcpConnect> connectPtr(connect);
    addConnect(name, connectPtr);
}

void TcpServer::removeConnect(string name){
    connectPool.erase(name);
}

void TcpServer::connectCloseEvent(boost::shared_ptr<TcpConnect> connect){
    connectCloseCallback(connect);
    removeConnect(connect->getName());
}

long TcpServer::getConnectCount(){
    return connectPool.size();
}

bool TcpServer::haveConnect(std::string name){
    return (connectPool.find(name) != connectPool.end());
}

void TcpServer::write(TcpConnect &connect, void *data, uint32_t length){
    connect.writeInLoop(data, length);
}

void TcpServer::write(shared_ptr<TcpConnect> connect, void *data, uint32_t length){
    connect->write(data, length);
}

void TcpServer::write(string &name, void *data, uint32_t length){
    if(haveConnect(name)){
        write(connectPool[name], data, length);
    }else{
        Log::Inst()->log("try write no exist connect.");
    }
}

void TcpServer::write(string &name, string &data){
    write(name, &(*data.begin()), data.length());
}

