/*************************************************************************
	> File Name: Buffer.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年04月08日 星期六 20时37分38秒
 ************************************************************************/

#include"Buffer.h"
#include<errno.h>
#include<sys/uio.h>
#include"SocketOperation.h"

using namespace std;
using namespace LeNet;

const uint32_t Buffer::InitSize = 1024;

Buffer::Buffer(uint32_t size)
:readIndex(0), writeIndex(0), buffer(size)
{
    
}

Buffer::~Buffer(){
    
}

void Buffer::append(const char *data, size_t len){
    ensureWriteableBytes(len);
    std::copy(data, data+len, buffer.begin() + writeIndex);
    writeIndex += len;
}

void Buffer::append(const string &str){
    const char *data = &(*str.begin());
    append(data, str.size());
}

void Buffer::ensureWriteableBytes(size_t len){
    if(writeableBytes() < len){
        buffer.resize(writeIndex + len);
    }
}

uint32_t Buffer::writeableBytes(){
    return buffer.size() - writeIndex;
}

uint32_t Buffer::readableBytes() const{
    return writeIndex - readIndex;
}

int Buffer::readFromIO(int fd, int &errorNo){
    char extrabuf[65536];
    struct iovec vec[2];
    uint32_t writeable = writeableBytes();
    vec[0].iov_base = &(*buffer.begin()) + writeIndex;
    vec[0].iov_len = writeable;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof(extrabuf);

    uint32_t iovcnt = (writeable < sizeof(extrabuf)) ? 2 : 1;
    int n = SocketOperation::readv(fd, vec, iovcnt);
    if(n < 0){
        errorNo = errno;
    }
    else if((uint32_t)n <= writeable){
        writeIndex += n;
    }
    else{
        writeIndex = buffer.size();
        append(extrabuf, n - writeable);
    }

    return n;
}

void Buffer::swap(Buffer &rhs){
    buffer.swap(rhs.buffer);
    std::swap(readIndex, rhs.readIndex);
    std::swap(writeIndex, rhs.writeIndex);
}
