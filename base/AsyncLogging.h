//
// Created by morass on 2022/7/1.
//

#ifndef WEBSERVER_ASYNCLOGGING_H
#define WEBSERVER_ASYNCLOGGING_H


#include <functional>
#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "CountDownLatch.h"
#include "LogStream.h"
#include "Thread.h"
#include "noncopyable.h"


class AsyncLogging : noncopyable {
public:
    AsyncLogging(const std::string basename, int flushInterval = 2);

    ~AsyncLogging() {
        if (running_) stop();
    }

    void append(const char *logline, int len);

    void start() {
        running_ = true;
        thread_.start();
        latch_.wait();
    }

    void stop() {
        running_ = false;
        cond_.notify_one();
        thread_.join();
    }

private:
    void threadFunc();

    typedef FixedBuffer<kLargeBuffer> Buffer;
    typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
    typedef BufferVector::value_type BufferPtr;

    const int flushInterval_;
    std::atomic<bool> running_;
    std::string basename_;
    Thread thread_;
    std::mutex mutex_;
    std::condition_variable cond_;
    BufferPtr currentBuffer_;
    BufferPtr nextBuffer_;
    BufferVector buffers_;
    CountDownLatch latch_;
};


#endif //WEBSERVER_ASYNCLOGGING_H
