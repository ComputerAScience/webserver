//
// Created by morass on 2022/7/1.
//

#ifndef WEBSERVER_THREAD_H
#define WEBSERVER_THREAD_H


#include "CountDownLatch.h"
#include "noncopyable.h"
#include <atomic>
#include <functional>
#include <memory>
#include <pthread.h>


#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <functional>
#include <memory>
#include <string>
#include "CountDownLatch.h"
#include "noncopyable.h"

class Thread : noncopyable {
public:
    typedef std::function<void()> ThreadFunc;
    explicit Thread(const ThreadFunc&, const std::string& name = std::string());
    ~Thread();
    void start();
    int join();
    bool started() const { return started_; }
    pid_t tid() const { return tid_; }
    const std::string& name() const { return name_; }

private:
    void setDefaultName();
    bool started_;
    bool joined_;
    pthread_t pthreadId_;
    pid_t tid_;
    ThreadFunc func_;
    std::string name_;
    CountDownLatch latch_;
};

#endif //WEBSERVER_THREAD_H
