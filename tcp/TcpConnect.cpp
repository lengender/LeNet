/*************************************************************************
	> File Name: TcpConnect.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年04月10日 星期一 09时40分53秒
 ************************************************************************/

#include"TcpConnect.h"
#include<boost/thread/thread.hpp>
#include"../log/Log.h"
#include<iostream>
using namespace std;
using namespace LeNet;
using namespace boost;

TcpConnect::TcpConnect(EventLoop *loop, struct sockaddr_in addr, int fd)
: loop(loop), socketAddr(addr),
  name(socketAddr.toString()), socket(new Socket(fd)),
  event(new Event(loop, fd)), state(Disconnecting)
{
    setNoDelay(true);
    loop->addEvent(event);
    event->setReadFunc(boost::bind(&TcpConnect::readEvent, this));
    event->setCloseFunc(boost::bind(&TcpConnect::closeEvent, this));
    event->setWriteFunc(boost::bind(&TcpConnect::writeEvent, this));
    event->setErrorFunc(boost::bind(&TcpConnect::errorEvent, this));
}

TcpConnect::~TcpConnect(){
    event->disableAll();
    event->removeFromLoop();
}

void TcpConnect::setMessageCallback(const boost::function<void(boost::shared_ptr<TcpConnect>, Buffer&)> &callback){
    messageCallback = callback;
}

void TcpConnect::setCloseCallback(const boost::function<void(boost::shared_ptr<TcpConnect>)> &callback){
    closeCallback = callback;
}

void TcpConnect::setWriteCompleteCallback(const boost::function<void(boost::shared_ptr<TcpConnect>)> &callback){
    writeCompleteCallback = callback;
}

void TcpConnect::readEvent(){
    int error = 0;

    int n = readBuf.readFromIO(event->getFd(), error);
    if(n > 0){
        if(messageCallback){
            messageCallback(shared_from_this(), readBuf);
        }
    }
    else if(n == 0){
        closeEvent();
    }
    else{
        Log::Inst()->log("TcpConnection::handleRead error.");
        closeEvent();
    }
}

void TcpConnect::closeEvent(){
    state = Disconnected;
    if(closeCallback){
        closeCallback(shared_from_this());
    }
}

TcpConnect& TcpConnect::getRefer(){
    return (*this);
}

const SocketAddr& TcpConnect::getAddr() const{
    return socketAddr;
}

string TcpConnect::getName() const{
    return name;
}

void TcpConnect::connectedHandle(){
    state = Connected;
    event->enableReading(true);

    event->enableErrorEvent(true);
}

void TcpConnect::errorEvent(){
    closeEvent();
}

void TcpConnect::writeEvent(){
    if(event->isWriteing){
        int n = SocketOperation::write(event->getFd(), writeBuf.readIndexPtr(), writeBuf.readableBytes());
        if(n > 0){
            writeBuf.clearReadIndex(n);
            if(writeBuf.isEmpty()){
                event->enableWriteing(false);
                if(writeCompleteCallback){
                    shared_ptr<TcpConnect> tmp(this);
                    writeCompleteCallback(tmp);
                    if(writeCompleteCallback)
                        writeCompleteCallback(shared_from_this());
                }
            }
        }
        else{
            Log::Inst()->log("write data error.");
        }
    }
    else{
        Log::Inst()->log("connect fd is down, no more writeing.");
    }
}

void TcpConnect::write(const char *data){
    int len = ::strlen(data);
    write((void*)data, len);
}


void TcpConnect::write(const string &data){
    write(&(*data.begin()), data.length());
}

void TcpConnect::write(const void *data, uint32_t length){
    int n = 0;
    size_t remaining = length;

    bool faultError = false;
    if(state == Disconnected){
        Log::Inst()->log("disconnected , give up writeing");
        return;
    }

    if(!event->isWriteing && writeBuf.isEmpty()){
        n = SocketOperation::write(event->getFd(), data, length);
        if(n > 0){
            remaining = length - n;
            if(remaining == 0 && writeCompleteCallback){
                writeCompleteCallback(shared_from_this());
            }
        }
        else{
            n = 0;
            if(errno != EUOULDBLOCK){
                Log::Inst()->log("write data error.");
                if(errno == EPIPE || errno == ECONNRESET){
                    faultError = true;
                }
            }
        }
    }

    if(!faultError && remaining > 0){
        writeBuf.append(static_cast<const char*>(data) + n, remaining);
        if(!event->isWriteing()){
            event->enableWriteing(true);
        }
    }
}

void TcpConnect::writeInLoop(const void *data, uint32_t len){
    loop->runInLoop(boost::bind(&TcpConnect::write, this, data, len));
}

void TcpConnect::setNoDelay(bool enable){
    socket->setTcpNoDelay(enable);
}

void TcpConnect::shutdownWrite(){
    if(state == Connected){
        state = Disconnecting;
        socket->shutdownWrite();
    }
}

