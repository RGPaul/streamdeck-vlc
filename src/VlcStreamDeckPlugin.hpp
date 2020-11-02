//======================================================================================================================
/**
@file       VlcStreamDeckPlugin.hpp

@copyright  (c) 2018, Corsair Memory, Inc.
			(c) 2020, Ralph-Gordon Paul
			This source code is licensed under the MIT-style license found in the LICENSE file.
**/
//======================================================================================================================

#include <StreamDeckSdk/ESDBasePlugin.h>

#include <stdint.h>

#include <string>
#include <mutex>
#include <set>

#include <nlohmann/json.hpp>

#include "CallBackTimer.hpp"
#include "VlcConnectionManager.hpp"
#include "VlcStatus.hpp"

class VlcStreamDeckPlugin : public ESDBasePlugin
{
public:
	
	VlcStreamDeckPlugin();
	virtual ~VlcStreamDeckPlugin();
	
	void KeyDownForAction(const std::string& inAction, const std::string& inContext, const nlohmann::json &inPayload,
						  const std::string& inDeviceID) override;
	void KeyUpForAction(const std::string& inAction, const std::string& inContext, const nlohmann::json &inPayload, 
					    const std::string& inDeviceID) override;
	
	void WillAppearForAction(const std::string& inAction, const std::string& inContext, const nlohmann::json &inPayload,
						     const std::string& inDeviceID) override;
	void WillDisappearForAction(const std::string& inAction, const std::string& inContext, 
								const nlohmann::json &inPayload, const std::string& inDeviceID) override;
	
	void DeviceDidConnect(const std::string& inDeviceID, const nlohmann::json &inDeviceInfo) override;
	void DeviceDidDisconnect(const std::string& inDeviceID) override;
	
	void SendToPlugin(const std::string& inAction, const std::string& inContext, const nlohmann::json &inPayload,
				      const std::string& inDeviceID) override;

	void DidReceiveGlobalSettings(const nlohmann::json& inPayload) override;

private:

	void UpdateTimer();
	
	std::mutex _visibleContextsMutex;
	std::set<std::string> _visiblePlayContexts; //!< contains all play/pause button contexts
	std::set<std::string> _visibleTitleContexts; //!< contains all title button contexts
	
	CallBackTimer* _timer { nullptr };
	VlcConnectionManager* _vlcConnectionManager { nullptr };

	// we store the number of failed update calls - if the last 5 calls failed, we will stop so we don't drawn
	// the log file
	uint8_t _lastUnsuccessfulUpdates { 0 };

	VlcStatus _currentStatus;

	// send an update request to vlc server and then updates the buttons with the newly received data
	void updateVlcStatus();
	
	// updates the stream deck buttons with the given payload (must be from status.json)
	void updateVlcStatus(const nlohmann::json &payload);

};
