/*************************************************************************
	> File Name: Socket.h
	> Author: 
	> Mail: 
	> Created Time: 2017年04月08日 星期六 13时50分32秒
 ************************************************************************/

#ifndef _SOCKET_H
#define _SOCKET_H


namespace LeNet{
    class Socket{
    public:
        Socket(int fd);
        ~Socket();
        void bind(SockAddr &addr);
        int accept(SocketAddr &addr);
        void listen();
        int getFd();
        void setTcpNoDelay(bool enable);
        int shutdownWrite();
    private:
        int sockfd;
    }; 
}
#endif
