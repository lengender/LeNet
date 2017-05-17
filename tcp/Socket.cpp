/*************************************************************************
	> File Name: Socket.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年04月08日 星期六 15时59分40秒
 ************************************************************************/

#include"Socket.h"
#include"SocketOperation.h"

using namespace LeNet;

Socket::Socket(int fd)
:sockfd(fd){

}

Socket::~Socket(){
    SocketOperation::close(sockfd);
}

int Socket::getFd(){
    return sockfd;
}

void Socket::bind(SocketAddr &addr){
    SocketOperation::bind(sockfd, addr.getAddrPtr());
}

int Socket::accept(SocketAddr &addr){
    struct sockaddr_in clientAddr;
    int res = SocketOperation::accept(socketFd, &clientAddr);
    if(res > 0){
        addr.setAddr(clientAddr);
    }

    return res;
}

void Socket::listen(){
    SocketOperation::listen(sockfd);
}

void Socket::setTcpNoDelay(bool enable){
    SocketOperation::setTcpNoDelay(sockfd, enable);
}

int Socket::shutdownWrite(){
    return SocketOperation::shutdownWrite(sockfd);
}
