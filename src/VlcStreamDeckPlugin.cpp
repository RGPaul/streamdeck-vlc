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

#include <StreamDeckSdk/EPLJSONUtils.h>
#include <StreamDeckSdk/ESDConnectionManager.h>

static const int kKeyStatePlay = 0;
static const int kKeyStatePause = 1;

static const char* const kActionIdPlay = "com.rgpaul.vlc.play";
static const char* const kActionIdTitle = "com.rgpaul.vlc.title";

// ---------------------------------------------------------------------------------------------------------------------
// Constructor / Destructor
// ---------------------------------------------------------------------------------------------------------------------

VlcStreamDeckPlugin::VlcStreamDeckPlugin()
{
	_timer = new CallBackTimer();
	
	_timer->start(3000, [this]()
	{
		this->UpdateTimer();
	});
	
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

// ---------------------------------------------------------------------------------------------------------------------
// Public
// ---------------------------------------------------------------------------------------------------------------------

void VlcStreamDeckPlugin::UpdateTimer()
{
	//
	// Warning: UpdateTimer() is running in the timer thread
	//
	if(mConnectionManager != nullptr && _lastUnsuccessfulUpdates < 5)
	{
		updateVlcStatus();
	}
}

void VlcStreamDeckPlugin::KeyDownForAction(const std::string& inAction, const std::string& inContext,
										   const nlohmann::json &inPayload, const std::string& inDeviceID)
{
	mConnectionManager->LogMessage("key pressed: " + inAction);

	// Play / Pause
	if (inAction == "com.rgpaul.vlc.play")
	{
		nlohmann::json payload;
		int state = EPLJSONUtils::GetIntByName(inPayload, "state");
		bool success { false };

		if (state == kKeyStatePlay) 
			success = _vlcConnectionManager->sendPlay(payload);
		else
			success = _vlcConnectionManager->sendPause(payload);

		if (success)
		{
			mConnectionManager->LogMessage("play/pause success");
		}
		else
		{
			std::string logMessage = EPLJSONUtils::GetStringByName(payload, "logMessage");
			mConnectionManager->LogMessage("play/pause failed: " + logMessage);
		}
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
	mConnectionManager->LogMessage("will appear for action: " + inAction);

	// if there is no password set, we might have to read in the global settings
	if (!_vlcConnectionManager->hasPasswordSet())
		mConnectionManager->RequestGlobalSettings(inDeviceID);

	// remember the context
	_visibleContextsMutex.lock();

	if (inAction == kActionIdPlay)
	{
		_visiblePlayContexts.insert(inContext);
	}
	else if (inAction == kActionIdTitle)
	{
		_visibleTitleContexts.insert(inContext);
	}

	_visibleContextsMutex.unlock();
}

void VlcStreamDeckPlugin::WillDisappearForAction(const std::string& inAction, const std::string& inContext,
												 const nlohmann::json &inPayload, const std::string& inDeviceID)
{
	// remove the context
	_visibleContextsMutex.lock();

	if (inAction == kActionIdPlay)
		_visiblePlayContexts.erase(inContext);
	else if (inAction == kActionIdTitle)
		_visibleTitleContexts.erase(inContext);

	_visibleContextsMutex.unlock();
}

void VlcStreamDeckPlugin::DeviceDidConnect(const std::string& inDeviceID, const nlohmann::json &inDeviceInfo)
{
	// Nothing to do
	mConnectionManager->LogMessage("device did connect: " + inDeviceID);
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
	mConnectionManager->LogMessage("received global settings");

	nlohmann::json settings;
  	EPLJSONUtils::GetObjectByName(inPayload, "settings", settings);

	std::string host = EPLJSONUtils::GetStringByName(settings, "vlcHost");
	std::string port = EPLJSONUtils::GetStringByName(settings, "vlcPort");
	std::string password = EPLJSONUtils::GetStringByName(settings, "vlcPassword");

	_vlcConnectionManager->setHost(host);
	_vlcConnectionManager->setPort(port);
	_vlcConnectionManager->setPassword(password);

	// connection details changed - so we might be able to update the vlc server state now
	updateVlcStatus();
}

// ---------------------------------------------------------------------------------------------------------------------
// Private
// ---------------------------------------------------------------------------------------------------------------------

void VlcStreamDeckPlugin::updateVlcStatus()
{
	nlohmann::json payload;
	// only try connecting if there is a password
	if (_vlcConnectionManager->hasPasswordSet())
	{
		// get's the status.json from vlc
		if (_vlcConnectionManager->getStatus(payload))
		{
			_lastUnsuccessfulUpdates = 0;
			updateVlcStatus(payload);
		}
		else
		{
			_lastUnsuccessfulUpdates++;
			std::string logMessage = EPLJSONUtils::GetStringByName(payload, "logMessage");
			mConnectionManager->LogMessage("update status failed: " + logMessage);
		}
	}
}

void VlcStreamDeckPlugin::updateVlcStatus(const nlohmann::json &payload)
{
	_currentStatus = VlcStatus(payload);

	_visibleContextsMutex.lock();
	
	if (_currentStatus.playState() == VlcStatus::PlayState::Playing)
	{
		for (const std::string& context : _visiblePlayContexts)
			mConnectionManager->SetState(kKeyStatePause, context);
	}
	else
	{
		for (const std::string& context : _visiblePlayContexts)
			mConnectionManager->SetState(kKeyStatePlay, context);
	}

	for (const std::string& context : _visibleTitleContexts)
		mConnectionManager->SetTitle(_currentStatus.songTitle(), context, kESDSDKTarget_HardwareAndSoftware);
	
	_visibleContextsMutex.unlock();
}
