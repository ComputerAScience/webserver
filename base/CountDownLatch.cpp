//
// Created by morass on 2022/7/1.
//

#include "CountDownLatch.h"


CountDownLatch::CountDownLatch(int count)
        : mutex_(),
          condition_(),
          count_(count) {
}

void CountDownLatch::wait() {
    std::unique_lock lk(mutex_);
    while (count_ > 0) {
        condition_.wait(lk);
    }
}

void CountDownLatch::countDown() {
    std::unique_lock lk(mutex_);
    --count_;
    if (count_ == 0) {
        condition_.notify_all();
    }
}

int CountDownLatch::getCount() const {
    std::unique_lock lk(mutex_);
    return count_;
}