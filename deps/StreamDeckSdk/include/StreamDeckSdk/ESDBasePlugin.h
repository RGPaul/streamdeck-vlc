//==============================================================================
/**
@file       ESDBasePlugin.h

@brief      Plugin base class

@copyright  (c) 2018, Corsair Memory, Inc.
            (c) 2020, Ralph-Gordon Paul
            This source code is licensed under the MIT-style license found in the LICENSE file.

**/
//==============================================================================

#pragma once

#include <string>

#include <nlohmann/json.hpp>

class ESDConnectionManager;

class ESDBasePlugin
{
  public:
    ESDBasePlugin() {}
    virtual ~ESDBasePlugin() {}

    void SetConnectionManager(ESDConnectionManager* inConnectionManager) { mConnectionManager = inConnectionManager; }

    virtual void KeyDownForAction(const std::string& inAction, const std::string& inContext,
								  const nlohmann::json& inPayload, const std::string& inDeviceID) = 0;
    virtual void KeyUpForAction(const std::string& inAction, const std::string& inContext,
                                const nlohmann::json& inPayload, const std::string& inDeviceID) = 0;

    virtual void WillAppearForAction(const std::string& inAction, const std::string& inContext,
                                     const nlohmann::json& inPayload, const std::string& inDeviceID) = 0;
    virtual void WillDisappearForAction(const std::string& inAction, const std::string& inContext,
                                        const nlohmann::json& inPayload, const std::string& inDeviceID) = 0;

    virtual void DeviceDidConnect(const std::string& inDeviceID, const nlohmann::json& inDeviceInfo) = 0;
    virtual void DeviceDidDisconnect(const std::string& inDeviceID) = 0;

    virtual void SendToPlugin(const std::string& inAction, const std::string& inContext,
                              const nlohmann::json& inPayload, const std::string& inDeviceID) = 0;
    virtual void DidReceiveGlobalSettings(const nlohmann::json& inPayload) = 0;

  protected:
    ESDConnectionManager* mConnectionManager = nullptr;
};
