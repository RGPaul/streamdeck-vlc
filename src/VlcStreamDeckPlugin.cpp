//======================================================================================================================
/**
@file       VlcStreamDeckPlugin.cpp

@copyright  (c) 2018, Corsair Memory, Inc.
			(c) 2020, Ralph-Gordon Paul
			This source code is licensed under the MIT-style license found in the LICENSE file.
**/
//======================================================================================================================

#include "VlcStreamDeckPlugin.hpp"

#include <atomic>

#include <StreamDeckSdk/ESDConnectionManager.h>

class CallBackTimer
{
public:
    CallBackTimer() :_execute(false) { }

    ~CallBackTimer()
    {
        if( _execute.load(std::memory_order_acquire) )
        {
            stop();
        };
    }

    void stop()
    {
        _execute.store(false, std::memory_order_release);
        if(_thd.joinable())
            _thd.join();
    }

    void start(int interval, std::function<void(void)> func)
    {
        if(_execute.load(std::memory_order_acquire))
        {
            stop();
        };
        _execute.store(true, std::memory_order_release);
        _thd = std::thread([this, interval, func]()
        {
            while (_execute.load(std::memory_order_acquire))
            {
                func();
                std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            }
        });
    }

    bool is_running() const noexcept
    {
        return (_execute.load(std::memory_order_acquire) && _thd.joinable());
    }

private:
    std::atomic<bool> _execute;
    std::thread _thd;
};

VlcStreamDeckPlugin::VlcStreamDeckPlugin()
{
	_timer = new CallBackTimer();
	_timer->start(1000, [this]()
	{
		this->UpdateTimer();
	});
}

VlcStreamDeckPlugin::~VlcStreamDeckPlugin()
{
	if(_timer != nullptr)
	{
		_timer->stop();
		
		delete _timer;
		_timer = nullptr;
	}
}

void VlcStreamDeckPlugin::UpdateTimer()
{
	//
	// Warning: UpdateTimer() is running in the timer thread
	//
	if(mConnectionManager != nullptr)
	{
		_visibleContextsMutex.lock();
		for (const std::string& context : _visibleContexts)
		{
			mConnectionManager->SetTitle("Test", context, kESDSDKTarget_HardwareAndSoftware);
		}
		_visibleContextsMutex.unlock();
	}
}

void VlcStreamDeckPlugin::KeyDownForAction(const std::string& inAction, const std::string& inContext,
										   const nlohmann::json &inPayload, const std::string& inDeviceID)
{
	// Nothing to do
}

void VlcStreamDeckPlugin::KeyUpForAction(const std::string& inAction, const std::string& inContext,
             							 const nlohmann::json &inPayload, const std::string& inDeviceID)
{
	// Nothing to do
}

void VlcStreamDeckPlugin::WillAppearForAction(const std::string& inAction, const std::string& inContext,
											  const nlohmann::json &inPayload, const std::string& inDeviceID)
{
	// Remember the context
	_visibleContextsMutex.lock();
	_visibleContexts.insert(inContext);
	_visibleContextsMutex.unlock();
}

void VlcStreamDeckPlugin::WillDisappearForAction(const std::string& inAction, const std::string& inContext,
												 const nlohmann::json &inPayload, const std::string& inDeviceID)
{
	// Remove the context
	_visibleContextsMutex.lock();
	_visibleContexts.erase(inContext);
	_visibleContextsMutex.unlock();
}

void VlcStreamDeckPlugin::DeviceDidConnect(const std::string& inDeviceID, const nlohmann::json &inDeviceInfo)
{
	// Nothing to do
}

void VlcStreamDeckPlugin::DeviceDidDisconnect(const std::string& inDeviceID)
{
	// Nothing to do
}

void VlcStreamDeckPlugin::SendToPlugin(const std::string& inAction, const std::string& inContext,
									   const nlohmann::json &inPayload, const std::string& inDeviceID)
{
	// Nothing to do
}
