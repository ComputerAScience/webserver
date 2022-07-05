//
// Created by morass on 2022/7/4.
//

#include "Timer.h"
#include "Connection.h"

Timer::Timer(EventLoop *loop, bool isRunning)
        : loop_(loop),
          timerfd_(::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC)),
          channnel_(new Channel(loop, timerfd_)),
          isRunning_(isRunning) {
    channnel_->setReadCallback([this](auto t){this->tick(t);});
}
extern const int timeslot;

Timer:: ~Timer() {
    if (isRunning_ == false)
        return;
    if (!stoped_) {
        channnel_->disableAll();
        channnel_->remove();
        close(timerfd_);
        stoped_ = true;
    }
}

void Timer::start(int64_t timeIntervalMicroSeconds)//心搏间隔，在EventLoopThreadPool中启动
{
    if (isRunning_ == false)
        return;
    stoped_ = false;
    channnel_->enableReading();
    timeIntervalMicroSeconds_ = timeIntervalMicroSeconds;
    struct itimerspec howlong;
    bzero(&howlong, sizeof howlong);

    howlong.it_value.tv_sec = static_cast<__time_t>(
            timeIntervalMicroSeconds / TimeSinceGMT::kMicroSecondsPerSecond);
    howlong.it_value.tv_nsec = static_cast<long>(
            (timeIntervalMicroSeconds % TimeSinceGMT::kMicroSecondsPerSecond) * 1000);

    howlong.it_interval.tv_sec = static_cast<__time_t>(
            timeIntervalMicroSeconds / TimeSinceGMT::kMicroSecondsPerSecond);
    howlong.it_interval.tv_nsec = static_cast<long>(
            (timeIntervalMicroSeconds % TimeSinceGMT::kMicroSecondsPerSecond) * 1000);

    ::timerfd_settime(timerfd_, 0, &howlong, NULL);//文件描述符timeIntervalMicroSeconds微秒后可读
}

void Timer::tick(int64_t receiveTime) {
    if (isRunning_ == false)
        return;
    //促发后read一下，否者会一直触发
    uint64_t howmany;
    ::read(timerfd_, &howmany, sizeof howmany);
    //receiveTime是epoll返回时的时间，离调用这个函数还是过了一会儿了
    for (auto &mapNode: timerQueue_) {
        int64_t overTimeSinceGMT = mapNode.first;
        timerNode oldNode = mapNode.second;
        if (overTimeSinceGMT < receiveTime + 200)//超时
        {
            removeConnection(overTimeSinceGMT, oldNode);
            oldNode->shutdown();
        } else//没有超时
        {
            timeIntervalMicroSeconds_ = overTimeSinceGMT - receiveTime + 200;
            //cout << "新的心搏间隔" << timeIntervalMicroSeconds_ << endl;
            stop();
            start(timeIntervalMicroSeconds_);
            break;
        }
    }
    if (timerQueue_.empty()) {
        if (timeIntervalMicroSeconds_ != timeslot) {
            stop();
            start(timeslot);
        }
    }
}

void Timer::addConnection(const timerNode &newNode, int64_t overTime)//overTime表示每个连接从接收开始的存活时间,单位微妙
{
    if (isRunning_ == false)
        return;
    newNode->setOverTime(overTime);
    int64_t overTimeSinceGMT = TimeSinceGMT().microSecondsSinceEpochOld() + overTime;
    newNode->setOverTimeSinceGMT(overTimeSinceGMT);
    timerQueue_.insert(make_pair(overTimeSinceGMT, newNode));
    //cout <<"add new node:" << newNode->name() <<"|over time:"<< newNode->getOverTimeSinceGMT()<<endl;

    //cout <<"loop:"<<CurrentThread::t_threadName<< "目前保存的连接如下:" << endl;
    //for (auto it : loop_->getTimerHeap())
    //{
    //	cout << it.first << "|" << it.second->name() << endl;
    //}
}

void Timer::multimapDelete(multimap<int64_t, timerNode> &mulMup, const int64_t &key, const string &value)
{
    pair<decltype(mulMup.begin()), decltype(mulMup.begin())> pos = mulMup.equal_range(key);
    auto it = pos.first;
    auto end = pos.second;
    while (it != end)
    {
        if (it->second->name() == value)
        {
            it = mulMup.erase(it);
            break;
        }
        else
        {
            ++it;
        }
    }
}
void Timer::removeConnection(int64_t overTimeSinceGMT, const timerNode &oldNode) {
    if (isRunning_ == false)
        return;
    multimapDelete(timerQueue_, overTimeSinceGMT, oldNode->name());
}

void Timer::modifyConnection(int64_t overTimeSinceGMT, const timerNode &oldNode) {
    if (isRunning_ == false)
        return;
    removeConnection(overTimeSinceGMT, oldNode);
    int64_t newOverTime = oldNode->getOverTime();
    addConnection(oldNode, newOverTime);
}


