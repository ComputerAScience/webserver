//
// Created by morass on 2022/7/5.
//

#ifndef WEBSERVER_SOCKET_H
#define WEBSERVER_SOCKET_H

#include <arpa/inet.h>
#include"../base/noncopyable.h"
#include"../base/Logging.h"

class Socket {
public:
    explicit Socket(int sockfd)
            : sockfd_(sockfd) {}

    ssize_t read(void *buf, size_t count);

    ssize_t readv(const struct iovec *iov, int iovcnt);

    ssize_t write(const void *buf, size_t count);

    void close();

    void shutdownWrite();

    int setSocketNonBlocking();

    void setSocketNodelay(bool on = true);

    void setSocketNoLinger();

    void setReuseAddr(bool on = true);

    void setKeepAlive(bool on = true);

private:
    const int sockfd_;
};

#endif //WEBSERVER_SOCKET_H
