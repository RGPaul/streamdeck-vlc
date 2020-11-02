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

static const int kKeyStatePlay = 0;
static const int kKeyStatePause = 1;

VlcStreamDeckPlugin::VlcStreamDeckPlugin()
{
	_timer = new CallBackTimer();
	/*
	_timer->start(1000, [this]()
	{
		this->UpdateTimer();
	});
	*/

	_vlcConnectionManager = new VlcConnectionManager();
}

VlcStreamDeckPlugin::~VlcStreamDeckPlugin()
{
	if (_timer != nullptr)
	{
		_timer->stop();
		
		delete _timer;
		_timer = nullptr;
	}

	if (_vlcConnectionManager != nullptr)
	{
		delete _vlcConnectionManager;
		_vlcConnectionManager = nullptr;
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
	// Play / Pause
	if (inAction == "com.rgpaul.vlc.play")
	{
		std::string s = _vlcConnectionManager->sendTest();
		mConnectionManager->LogMessage(s);
		mConnectionManager->SetTitle("Test", inContext, kESDSDKTarget_HardwareAndSoftware);
	}
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

void VlcStreamDeckPlugin::DidReceiveGlobalSettings(const nlohmann::json& inPayload)
{

}
