//
// Created by morass on 2022/7/5.
//

#ifndef WEBSERVER_EVENTLOOPTHREAD_H
#define WEBSERVER_EVENTLOOPTHREAD_H


#include <condition_variable>
#include <mutex>
#include "../base/Thread.h"
#include "../base/noncopyable.h"
#include <assert.h>



class EventLoop;

class EventLoopThread :noncopyable
{
public:
    EventLoopThread(const std::string& name = std::string());
    ~EventLoopThread();

    EventLoop* startLoop();

private:
    void threadFunc();

    EventLoop* loop_;
    bool exiting_;
    Thread thread_;
    std::mutex mutex_;
    std::condition_variable cond_;
};


#endif //WEBSERVER_EVENTLOOPTHREAD_H
