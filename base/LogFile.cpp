//
// Created by morass on 2022/7/1.
//

#include "LogFile.h"
#include "FileUtil.h"
#include <memory>


using namespace std;

LogFile::LogFile(const string &basename, int flushEveryN)
        : basename_(basename),
          flushEveryN_(flushEveryN),
          count_(0),
          mutex_(std::make_unique<std::mutex>()),
          file_(make_unique<AppendFile>(basename)) {
    // assert(basename.find('/') >= 0);
}

LogFile::~LogFile() {}

void LogFile::append(const char *logline, int len) {
    lock_guard lk(*mutex_);
    append_unlocked(logline, len);
}

void LogFile::flush() {
    lock_guard lk(*mutex_);
    file_->flush();
}

void LogFile::append_unlocked(const char *logline, int len) {
    file_->append(logline, len);
    ++count_;
    if (count_ >= flushEveryN_) {
        count_ = 0;
        file_->flush();
    }
}