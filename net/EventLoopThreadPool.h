//
// Created by morass on 2022/7/5.
//

#ifndef WEBSERVER_EVENTLOOPTHREADPOOL_H
#define WEBSERVER_EVENTLOOPTHREADPOOL_H


#include "../base/noncopyable.h"
#include"Timer.h"
#include <functional>
#include <memory>
#include <vector>
#include <string>
#include <assert.h>
#include<unordered_map>

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool :noncopyable
{
public:
    EventLoopThreadPool(EventLoop* baseLoop, const std::string& nameArg);
    ~EventLoopThreadPool();
    void setThreadNum(int numThreads) { numThreads_ = numThreads; }
    void start(bool starTimer = false);

    // round-robin
    EventLoop* getNextLoop();

private:

    EventLoop* baseloop_;										//和accept所属的eventloop相同
    std::string name_;
    bool started_;
    int next_;													//新连接到来时，所选择的EventLoop对象下标
    int numThreads_;											//线程数

    std::vector<std::unique_ptr<EventLoopThread>>threads_;		//IO线程列表
    std::vector<EventLoop*> loops_;								//EventLoop列表

};



#endif //WEBSERVER_EVENTLOOPTHREADPOOL_H
