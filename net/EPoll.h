//
// Created by morass on 2022/7/4.
//

#ifndef WEBSERVER_EPOLL_H
#define WEBSERVER_EPOLL_H

#include <map>
#include <vector>
#include"../base/noncopyable.h"
#include"../base/TimeSinceGMT.h"

class EventLoop;

class Channel;

struct epoll_event;

class EPoll : noncopyable {
public:
    EPoll(EventLoop *loop);

    ~EPoll();

    typedef std::vector<Channel *> ChannelList;

    int64_t poll(int timeoutMs, ChannelList *activeChannels);//开启I/O复用
    void updateChannel(Channel *channel);

    void removeChannel(Channel *channel);

private:

    static const int kInitEventListSize = 16;//初始时最多监听事件个数
    void fillActiveChannels(int numEvents, ChannelList *activeChannels) const;//填充就绪的channel
    void update(int operation, Channel *channel);

    typedef std::map<int, Channel *> ChannelMap;
    ChannelMap channels_;
    typedef std::vector<struct epoll_event> EventList;
    EventLoop *ownerLoop_;    // Poller所属EventLoop
    int epollfd_;                //表示epoll的文件描述符
    EventList events_;        //就绪事件集合
};

#endif //WEBSERVER_EPOLL_H
