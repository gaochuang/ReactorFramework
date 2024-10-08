#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <memory>

#include "plugin/PluginServices.hpp"
#include "timer/timerQueue.hpp"
#include "event/eventCallbackQueue.hpp"
#include "signalMonitor/signalMonitor.hpp"
#include "plugin/FdMonitor.hpp"
#include "ComAPI.hpp"
#include "engine/sharedLibraryMap.hpp"
#include "plugin/PluginMap.hpp"
#include "plugin/PluginDataStorage.hpp"

namespace commonApi
{

class EventLoop;

class Engine : public std::enable_shared_from_this<Engine>, 
               public ComAPI,
               public FdMonitor, 
               public PluginServices,
               public PluginDataStorage   
               
{
public:
    Engine();
    Engine(const Callbacks& callbacks);
    ~Engine();


    void addFd(int fd, unsigned int events, const FdEventHandler& handler) override;
    void removeFd(int fd) override;
    void modifyFd(int fd, unsigned int events) override;

    void run() override;
    void stop() override;

    int getEpollFd() const override;
    void postCallback(const Callback& callback) override;
    void postCallback(Callback&& callback) override;
    void waitAndHandleEvents() override;
    bool waitAndHandleEvents(const std::chrono::steady_clock::duration& timeout) override;
   
    std::shared_ptr<ComAPI> getComAPI() override;
    std::shared_ptr<ComAPIEventLoop> getComAPIEventLoop() override;

    CallbackQueueService& getEventCallbackQueue() override;
    TimerService& getTimerService() override;
    SignalMonitorService& getSignalMonitor() override;
    FdMonitor& getFdMonitor() override;

    PluginList getExistingPlugins() override;
    PluginDataStorage& getPluginDataStorage() override;

    void addSharedLibrary(const SharedLibraryMap::key_type& key, const std::shared_ptr<SharedLibrary>& sharedLibrary);
    std::shared_ptr<SharedLibrary> getSharedLibrary(const SharedLibraryMap::key_type& key) const;

    void updatePluginMap(const PluginMap::key_type& key, const std::shared_ptr<Plugin>& plugin);

    void save(Key key, Data data) override;
    Data get(Key key) const override;
    void remove(Key key) override;

private:
    Callbacks callbacks;

    std::unordered_map<int, FdMonitor::FdEventHandler> handlers;
    PluginMap pluginMap;
    SharedLibraryMap sharedLibraryMap;

    std::map<Key, Data> pluginDataMap;
    std::unique_ptr<TimerQueue> timerQueue;
    std::shared_ptr<EventLoop> loop;
    std::unique_ptr<CallbackQueueService> callbackQueue;
    std::unique_ptr<SignalMonitorService> signalMonitor;
};

}

#endif
