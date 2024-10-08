#include "thread/eventThreadPool.hpp"

#include<iostream>

namespace commonApi
{
  
EventThreadPool::EventThreadPool(EventLoop* loop):mainLoop(loop),started(false),threadNum(0),threadIndex(0)
{

}

EventThreadPool::~EventThreadPool()
{

}

void EventThreadPool::init()
{
    int i(0);
    while( i < threadNum )
    {
        std::shared_ptr<EventLoopThread> thread(new EventLoopThread());
        thread->start();
        threadPoll.push_back(thread);
        eventLoopPoll.push_back(thread->getEventLoopInThread());
        i++;
    }

    started = true;
}

void EventThreadPool::init(int num)
{
    threadNum = num;
    init();
}

bool EventThreadPool::isStarted() const
{
    return started;

}

void EventThreadPool::setThreadNum(uint16_t num)
{
    threadNum = num;
}

EventLoop* EventThreadPool::getOneLoopFromPool()
{
    if(eventLoopPoll.empty())
    {
        return mainLoop;
    }

    threadIndex++;
    if(threadIndex >= eventLoopPoll.size())
    {
        threadIndex = 0;
    }

    return eventLoopPoll[threadIndex];
}

EventLoop* EventThreadPool::getOneLoopWithHash(int hash)
{
    if(eventLoopPoll.empty())
    {
        return mainLoop;
    }

    auto index = hash % threadNum;
    return eventLoopPoll[index];
}

}
