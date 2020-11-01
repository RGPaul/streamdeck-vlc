//======================================================================================================================
/**
@file       VlcStreamDeckPlugin.hpp

@copyright  (c) 2018, Corsair Memory, Inc.
			(c) 2020, Ralph-Gordon Paul
			This source code is licensed under the MIT-style license found in the LICENSE file.
**/
//======================================================================================================================

#include <StreamDeckSdk/ESDBasePlugin.h>

#include <string>
#include <mutex>
#include <set>

#include <nlohmann/json.hpp>

#include "CallBackTimer.hpp"

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

private:

	void UpdateTimer();
	
	std::mutex _visibleContextsMutex;
	std::set<std::string> _visibleContexts;
	
	CallBackTimer* _timer { nullptr };

	int _playButtonState { 0 };
};
