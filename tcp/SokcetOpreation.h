/*************************************************************************
	> File Name: SokcetOpreation.h
	> Author: 
	> Mail: 
	> Created Time: 2017年04月08日 星期六 13时53分52秒
 ************************************************************************/
 
#ifndef _SOKCETOPREATION_H
#define _SOKCETOPREATION_H

#include<sys/types.h>
#include<string>
#include<netinet/in.h>
#include<linux/tcp.h>
#include"../log/Log.h"
#include<vector>
namespace LeNet{
    class SocketOperation{
    public:
        static int createNonblockingSocket(){
            int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPOROTO_TCP);
            if(sockfd < 0){
                Log::Inst()->log("create socket error.");
            }

            return sockfd;
        }

        static int bind(int sockfd, const struct sockaddr_in *addr){
            int ret = ::bind(sockfd, (struct sockaddr *)addr, (sizeof(struct sockaddr)));
            if(ret < 0){
                Log::Inst()->log("bind socket error.");
            }

            return ret;
        }

        static int listen(int fd){
            int ret = ::listen(sockfd, SOMAXCONN);
            if(ret < 0){
                Log::Inst()->log("listen socket error.");
            }

            return ret;
        }

        static int connect(int sockfd, const struct sockaddr *addr){
            return ::connect(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
        }

        static int accept(int sockfd, struct sockaddr_in *addr){
            socklen_t addr_len = sizeof(struct sockaddr_in);
            int ret = ::accept(sockfd, (struct sockaddr*)addr, &addr_len);
            if(ret < 0){
                Log::Inst()->log("accept socket error.");
            }

            return ret;
        }

        static ssize_t read(int sockfd, void *buf, size_t count){
            return ::read(sockfd, buf, count);
        } 

        static ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt){
            return ::readv(sockfd, iov, iovcnt);
        }

        static ssize_t write(int sockfd, const void *buf, size_t count){
            return ::write(sockfd, buf, count);
        }

        static void  close(int sockfd){
            if(::close(sockfd) < 0){
                Log::Inst()->log("close socket error.");
            }
        }

        static void getAddrAnyIpv4(struct sockaddr_in &addr, uint16_t port){
            bzero(&addr, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
        }

        static bool toAddrIpv4(const string &addrIp, struct sockaddr_in& addr){
            vector<string> strs;
            boost::split(strs, addrIp, boost::is_any_of(":"));
            if(strs.size() != 2)
                return false;

            uint16_t port;
            if(!stringToInt<uint16_t>(str[1], port))
                return false;

            return toAddrIpv4(strs[0], port, addr);
        }

        static bool toAddrIpv4(const string& str, uint16_t port, struct sockaddr_in &addr){
            vector<string> ip;

            boost::split(ip, addr, boost::is_any_of("."));
            if(ip.size() != 4)
                return false;

            uint16_t addrArray[4];
            for(int i =0; i < 4; ++i){
                if(!stringToInt<uint16_t>(ip[i], addrArray[i]))
                    return false;

                if(addrArray[i] > 255)
                    return false;
            }

            uint32_t addr32;
            for(int i = 0; i < 4; ++i){
                addr32 <<= 8;
                addr32 |= addrArray[i];
            }

            bzero(&addr, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = addr32;

            return true;
        }

        static bool toAddrIpv4(uint16_t port, struct sockaddr_in &addr){
            bzero(&addr, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = htonl(INADDR_ANY);

            return true;
        }

        static string ipToString(struct sockaddr_in addr){
            std::stringstream stream;

            uint8_t* addrArray = (uint8_t*)&addr.sin_addr.s_addr;
            for(int i = 0; i < 4; ++i){
                stream << (uint16_t)addrArray[i];
                if(i != 3)
                    stream << ".";
            }

            stream<<":"<<(((addr.sin_port << 8) & 0x00ffff) | (addr.sin_port >> 8));
            return stream.str();
        }

        static string toString(struct sockaddr_in addr){
            string addrPort;
            addrPort = ipToString(addr);
            return addrPort;
        }

        template<typename T>
        static bool stringToInt(const string &str, T &num){
            std::stringstream stream;
            stream << str << endl;
            stream >> num;
            return stream.good();
        }

        static int shutdownWrite(int sockfd){
            int res;

            if((res = ::shutdown(sockfd, SHUT_WR)) < 0){
                Log::Inst()->log("shutdown write error.");
            }

            return res;
        }

        static void setTcpNoDelay(int fd, bool isEnable){
            int opt = isEnable ? 1 : 0;
            ::setsockopt(fd, IPPOROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
        }

    };
}
#endif
