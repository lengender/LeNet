/*************************************************************************
	> File Name: TcpConnect.h
	> Author: 
	> Mail: 
	> Created Time: 2017年04月08日 星期六 16时32分59秒
 ************************************************************************/

#ifndef _TCPCONNECT_H
#define _TCPCONNECT_H

#include<boost/shared_ptr.hpp>
#include<boost/function.hpp>
#include<boost/enable_shared_from_this.hpp>
#include"../event/EventLoop.h"
#include"Socket.h"

namespace LeNet{
    class TcpConnect : public boost::enable_shared_from_this<TcpConnect>{
    public:
        TcpConnect(EventLoop *loop, struct sockaddr_in, int fd);
        ~TcpConnect();
        void setMessageCallback(const boost::function<void(boost::shared_ptr<TcpConnect>, Buffer &)> &callback);
        void setCloseCallback(const boost::function<void(boost::shared_ptr<TcpConnect>)> &callback);
        void setWriteCompleteCallback(const boost::function<void(boost::shared_ptr<TcpConnet>)> &callback);

        const SocketAddr& getAddr() const;
        std::string getName() const;
        void write(const char *data);
        void write(const std::string& data);
        void write(const void *data, uint32_t len);
        void writeInLoop(const void *data, uint32_t len);
        void connectedHandler();

        void setNoDelay(bool enable);
        void shutdownWrite();
    private:
        EventLoop *loop;
        SocketAddr socketAddr;
        std::string name;
        boost::shared_ptr<Socket> socket;
        boost::shared_ptr<Event> event;

        void readEvent();
        void closeEvent();
        void writeEvent();
        void errorEvent();
        boost::function<void(boost::shared_ptr<TcpConnect>, Buffer&)> messageCallback;
        boost::function<void(boost::shared_ptr<TcpConnect>)> closeCallback;
        boost::function<void(boost::shared_ptr<TcpConnect>)> writeCompleteCallback;

        Buffer readBuf;
        Buffer writeBuf;
        TcpConnect &geetRefer();

        int state;
        enum ConnectStat{ Disconnected, Connectiong, Connected, Disconnecting };
    };
}
#endif
