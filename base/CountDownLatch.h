//
// Created by morass on 2022/7/1.
//

#ifndef WEBSERVER_COUNTDOWNLATCH_H
#define WEBSERVER_COUNTDOWNLATCH_H

#include <mutex>
#include <condition_variable>


class CountDownLatch {
public:

    explicit CountDownLatch(int count);

    void wait();

    void countDown();

    int getCount() const;

private:
    mutable std::mutex mutex_;
    std::condition_variable condition_;
    int count_;
};


#endif //WEBSERVER_COUNTDOWNLATCH_H
