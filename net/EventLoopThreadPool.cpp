//
// Created by morass on 2022/7/5.
//

#include "EventLoopThreadPool.h"
#include "EventLoopThread.h"
#include <iostream>

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop, const std::string& nameArg)
        :baseloop_(baseLoop),
         name_(nameArg),
         started_(false),
         next_(0),
         numThreads_(0)
{}
EventLoopThreadPool::~EventLoopThreadPool()
{
}

void EventLoopThreadPool::start(bool starTimer)
{
    assert(!started_);
    baseloop_->assertInLoopThread();
    started_ = true;

    for (int i = 0; i < numThreads_; ++i)
    {
        char buf[name_.size() + 32];
        snprintf(buf, sizeof buf, "%s%d", name_.c_str(), i);
        EventLoopThread* t = new EventLoopThread(buf);
        threads_.push_back(std::unique_ptr<EventLoopThread>(t));
        loops_.push_back(t->startLoop());
    }
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
    baseloop_->assertInLoopThread();         // 若loops_为0，则返回MainReactor
    assert(started_);
    EventLoop* loop = baseloop_;

    if (!loops_.empty())
    {
        // round-robin
        loop = loops_[next_];
        ++next_;
        if (static_cast<size_t>(next_) >= loops_.size())
        {
            next_ = 0;
        }
    }
    return loop;
}