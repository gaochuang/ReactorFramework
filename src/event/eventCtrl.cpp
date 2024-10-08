#include "event/eventCtrl.hpp"

#include <iostream>
namespace commonApi
{

EventCtrl::EventCtrl(EventLoop* el) : eventLoop(el)
{

}

EventCtrl::~EventCtrl()
{
    for(const auto& pair : eventPool)
    {
        int fd = pair.first;
        epoll.removeEvent(fd);
    }
    eventPool.clear();
}

int EventCtrl::getEpollFd() const noexcept
{
    return epoll.getEpollFd();
}

void EventCtrl::addEvent(std::shared_ptr<Event> event)
{
    //添加到event注册表，再加入到epoll中
    eventPool.insert(std::pair<int, std::weak_ptr<Event>>(event->getFd(), event));
    epoll.addEvent(event.get());
}

void EventCtrl::deleteEvent(std::shared_ptr<Event> event)
{
    /*
    bug: remove fd epoll -> close fd
    */
   //先从epoll中移除，再从注册事件表中移除
    epoll.removeEvent(event.get());
    eventPool.erase(event->getFd());
}

void EventCtrl::modifyEvent(std::shared_ptr<Event> event)
{
    auto it = eventPool.find(event->getFd());
    if (it != eventPool.end())
    {
        epoll.modifyEvent(event.get());
    }
}

void EventCtrl::modifyEvent(int fd)
{
    auto it = eventPool.find(fd);
    if (it != eventPool.end())
    {
        std::shared_ptr<Event> eventS = it->second;
        if (eventS)
        {
            epoll.modifyEvent(eventS.get());
        }
    }
}

void EventCtrl::deleteEvent(int fd)
{
    if (eventPool.find(fd) != eventPool.end())
    {
        /*
        出现先close,后从epoll中移除问题
        */
        epoll.removeEvent(fd);
        eventPool.erase(fd);
    }
}

/*
完成事件分发功能 
*/
void EventCtrl::waitAndRunHandler(int timeMs)
{
    activeEvents.resize(eventPool.size());
    const auto num = epoll.waitEvent(activeEvents.data(), static_cast<int>(activeEvents.size()), timeMs);
    if (num < 0)
    {
        return;
    }


    activeEvents.resize(num);
    for(const auto& event : activeEvents)
    {
        auto fd = event.data.fd;
        std::shared_ptr<Event> eventPtr = eventPool[fd];
        if (eventPtr)
        {
            eventPtr->handle(event.events);
        }
        else
        {
            deleteEvent(fd);
        }
    }
}

}
