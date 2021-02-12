//======================================================================================================================
/**
@file       VlcStreamDeckPlugin.cpp

@copyright  (c) 2018, Corsair Memory, Inc.
			(c) 2020, Ralph-Gordon Paul
			This source code is licensed under the MIT-style license found in the LICENSE file.
**/
//======================================================================================================================

#include "VlcStreamDeckPlugin.hpp"

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
	if (mConnectionManager != nullptr &&   // we need a connection to the stream deck
	   _vlcConnectionManager != nullptr && // we need a connection to the vlc server
	   _lastUnsuccessfullCalls < 5         // don' flood the log with connection errors (just try some times)
	   && !_allVisibleContexts.empty())    // we only need to update if there is at least one context visible
	{
		updateVlcStatus();
	}
}

void VlcStreamDeckPlugin::KeyDownForAction(const std::string& inAction, const std::string& inContext,
										   const nlohmann::json &inPayload, const std::string& inDeviceID)
{
	mConnectionManager->LogMessage("key pressed: " + inAction);

	// Play
	if (inAction == "com.rgpaul.vlc.play")
	{
		keyPressedPlay(inPayload);
	}
	// Pause
	else if (inAction == "com.rgpaul.vlc.pause")
	{
		keyPressedPause(inPayload);
	}
	// Play / Pause
	else if (inAction == "com.rgpaul.vlc.playpause")
	{
		keyPressedPlayPause(inPayload);
	}
	// Next Title
	else if (inAction == "com.rgpaul.vlc.next")
	{
		keyPressedNext(inPayload);
	}
	// Previous Title
	else if (inAction == "com.rgpaul.vlc.prev")
	{
		keyPressedPrevious(inPayload);
	}
	// Volume Up
	else if (inAction == "com.rgpaul.vlc.volumeup")
	{
		keyPressedVolumeUp(inPayload);
	}
	// Volume Down
	else if (inAction == "com.rgpaul.vlc.volumedown")
	{
		keyPressedVolumeDown(inPayload);
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

	// also add all contexts to the all visible lists
	_allVisibleContexts.insert(inContext);

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

	// also remove from all visible contexts
	_allVisibleContexts.erase(inContext);

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
		bool success = _vlcConnectionManager->getStatus(payload);

		if (!success)
			_lastUnsuccessfullCalls++;
		
		processVlcResponse("update status", success, payload);
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

void VlcStreamDeckPlugin::keyPressedPlay(const nlohmann::json &inPayload)
{
	nlohmann::json payload;
	bool success = _vlcConnectionManager->sendPause(payload);

	processVlcResponse("play", success, payload);
}

void VlcStreamDeckPlugin::keyPressedPause(const nlohmann::json &inPayload)
{
	nlohmann::json payload;
	bool success = _vlcConnectionManager->sendPause(payload);

	processVlcResponse("pause", success, payload);
}

void VlcStreamDeckPlugin::keyPressedPlayPause(const nlohmann::json &inPayload)
{
	nlohmann::json payload;
	int state = EPLJSONUtils::GetIntByName(inPayload, "state");
	bool success { false };

	if (state == kKeyStatePlay) 
		success = _vlcConnectionManager->sendPlay(payload);
	else
		success = _vlcConnectionManager->sendPause(payload);

	processVlcResponse("play/pause", success, payload);
}

void VlcStreamDeckPlugin::keyPressedNext(const nlohmann::json &inPayload)
{
	nlohmann::json payload;
	bool success = _vlcConnectionManager->sendNext(payload);

	processVlcResponse("next", success, payload);
}

void VlcStreamDeckPlugin::keyPressedPrevious(const nlohmann::json &inPayload)
{
	nlohmann::json payload;
	bool success = _vlcConnectionManager->sendPrevious(payload);

	processVlcResponse("previous", success, payload);
}

void VlcStreamDeckPlugin::keyPressedVolumeUp(const nlohmann::json &inPayload)
{
	nlohmann::json payload;
	bool success = _vlcConnectionManager->sendVolumeUp(payload);

	processVlcResponse("volume up", success, payload);
}

void VlcStreamDeckPlugin::keyPressedVolumeDown(const nlohmann::json &inPayload)
{
	nlohmann::json payload;
	bool success = _vlcConnectionManager->sendVolumeDown(payload);

	processVlcResponse("volume down", success, payload);
}

void VlcStreamDeckPlugin::processVlcResponse(const std::string& functionName, bool success, 
	const nlohmann::json& payload)
{
	nlohmann::json piPayload;

	piPayload["event"] = "com.rgpaul.vlc.rpc.state";

	if (success)
	{
		updateVlcStatus(payload);
		_lastUnsuccessfullCalls = 0;

		piPayload["state"] = "Connection Successful";
		for (const auto context : _allVisibleContexts)
			mConnectionManager->SendToPropertyInspector("com.rgpaul.vlc.rpc.state", context, piPayload);
	}
	else
	{
		// log error
		std::string logMessage = EPLJSONUtils::GetStringByName(payload, "logMessage");
		mConnectionManager->LogMessage(functionName + " failed: " + logMessage);

		// update rpc state on property inspector
		std::string message = EPLJSONUtils::GetStringByName(payload, "message");
		piPayload["state"] = message;
		for (const auto context : _allVisibleContexts)
			mConnectionManager->SendToPropertyInspector("com.rgpaul.vlc.rpc.state", context, piPayload);
	}
}
