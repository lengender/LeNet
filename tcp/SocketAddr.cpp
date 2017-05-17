/*************************************************************************
	> File Name: SocketAddr.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年04月08日 星期六 14时50分48秒
 ************************************************************************/

#include"SocketAddr.h"
#include"SocketOperation.h"
using namespace std;
using namespace LeNet;

SocketAddr::SocketAddr()
: valid(false){

}

SocketAddr::SocketAddr(bool isAny, int port)
{
    if(isAny){
        SocketOperation::getAddrAnyIpv4(sockAddr, port);
    }
}

SocketAddr::SocketAddr(struct sockaddr_in addr)
:valid(true), sockAddr(addr){
    
}

SocketAddr::~SocketAddr()
{
    
}

SocketAddr::SocketAddr(const string &addrPort)
:valid(false)
{
    struct sockaddr_in addr;
    if(!SocketOperation::toAddrIpv4(addrPort, addrIn)){
        return;
    }

    new(this) SocketAddr(addrIn);
}

SocketAddr::SocketAddr(const string& addr, uint16_t port)
:valid(false)
{
    struct sockaddr_in addIn;
    if(!SocketOperation::toAddrIpv4(addr, port, addIn))
        return;

    new(this)SocketAddr(addrIn);
}

SocketAddr::SocketAddr(uint16_t port)
:valid(false){
    struct sockaddr_in addrIn;
    if(!SocketOperation::toAddrIpv4(port, addrIn)){
        return;
    }

    new (this) SocketAddr(addrIn);
}

void SocketAddr::setAddr(struct sockaddr_in addr){
    sockAddr = addr;
    valid = true;
}

struct sockaddr_in SocketAddr::getAddr() const{
    return sockAddr;
}

struct sockaddr_in * SocketAddr::getAddrPtr(){
    return &sockAddr;
}

bool SocketAddr::isValid(){
    return valid;
}

string SocketAddr::toString() const{
    return SocketOperation::toString(sockAddr);
}

