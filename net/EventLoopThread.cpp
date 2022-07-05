//
// Created by morass on 2022/7/5.
//

#include "EventLoopThread.h"
#include "EventLoop.h"
extern const int timeslot = 1000 * 1000;
EventLoopThread::EventLoopThread(const std::string& name)
        :loop_(NULL),
         exiting_(false),
         thread_(std::bind(&EventLoopThread::threadFunc, this), name),
         mutex_(),
         cond_()
{
}
EventLoopThread::~EventLoopThread()
{
    exiting_ = true;
    if (loop_ != NULL)
    {
        loop_->quit();
        thread_.join();
    }
}

EventLoop* EventLoopThread::startLoop()
{
    thread_.start();
    {
        std::unique_lock<std::mutex> lock(mutex_);
        // 一直等到threadFun在Thread里真正跑起来
        while (loop_ == NULL)
            cond_.wait(lock);
    }
    return loop_;
}

void EventLoopThread::threadFunc()
{
    EventLoop loop;

    {
        unique_lock<std::mutex> lock(mutex_);
        loop_ = &loop;
        loop_->timerStart(timeslot);
        cond_.notify_one();
    }

    loop.loop();
    loop_ = NULL;
}
