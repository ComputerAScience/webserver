//
// Created by morass on 2022/7/4.
//

#ifndef WEBSERVER_TIMER_H
#define WEBSERVER_TIMER_H

#include "../base/noncopyable.h"
#include "../base/TimeSinceGMT.h"
#include "EventLoop.h"
#include "Channel.h"
#include <iostream>
#include <memory>
#include <unistd.h>
#include <sys/timerfd.h>
#include <queue>
#include <map>
#include <deque>
#include <string>
using namespace  std;
class Connection;

class Timer {
    typedef std::shared_ptr<Connection> timerNode;
public:
    Timer(EventLoop *loop, bool isRunning = false);

    ~Timer();

    void start(int64_t timeIntervalMicroSeconds);//心搏间隔，在EventLoopThreadPool中启动

    void tick(int64_t receiveTime);

    void addConnection(const timerNode &newNode, int64_t overTime);//overTime表示每个连接从接收开始的存活时间,单位微妙

    void removeConnection(int64_t overTimeSinceGMT, const timerNode &oldNode);

    void modifyConnection(int64_t overTimeSinceGMT, const timerNode &oldNode);

    void multimapDelete(std::multimap<int64_t, timerNode> &mulMup, const int64_t &key, const string &value);

private:
    void stop() {
        if (isRunning_ == false)
            return;
        if (!stoped_) {
            channnel_->disableAll();
            channnel_->remove();
            stoped_ = true;
        }
    }

    EventLoop *loop_;
    int timerfd_;
    std::multimap<int64_t, timerNode> timerQueue_;
    unique_ptr<Channel> channnel_;
    int64_t timeIntervalMicroSeconds_;            //心搏时间间隔
    bool stoped_;
    bool isRunning_;
};


#endif //WEBSERVER_TIMER_H
