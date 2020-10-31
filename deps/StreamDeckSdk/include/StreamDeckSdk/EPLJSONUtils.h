//==============================================================================
/**
@file       EPLJSONUtils.h

@brief		Utility methods for JSON parser from N.Lohmann
			https://github.com/nlohmann/json

@copyright  (c) 2018, Corsair Memory, Inc.
			(c) 2020, Ralph-Gordon Paul
			This source code is licensed under the MIT-style license found in the LICENSE file.

**/
//==============================================================================

#pragma once

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <nlohmann/json.hpp>

class EPLJSONUtils
{

public:
	
	//! Get object by name
	static bool GetObjectByName(const nlohmann::json& inJSON, const std::string& inName, nlohmann::json& outObject)
	{
		// Check desired value exists
		nlohmann::json::const_iterator iter(inJSON.find(inName));
		if (iter == inJSON.end())
			return false;

		// Check value is an array
		if (!iter->is_object())
			return false;

		// Assign value
		outObject = *iter;

		return true;
	}
	
	//! Get array by name
	static bool GetArrayByName(const nlohmann::json& inJSON, const std::string& inName, nlohmann::json& outArray)
	{
		// Check desired value exists
		nlohmann::json::const_iterator iter(inJSON.find(inName));
		if (iter == inJSON.end())
			return false;

		// Check value is an array
		if (!iter->is_array())
			return false;

		// Assign value
		outArray = *iter;

		return true;
	}

	//! Get string by name
	static std::string GetStringByName(const nlohmann::json& inJSON, const std::string& inName, 
									   const std::string& defaultValue = "")
	{
		// Check desired value exists
		nlohmann::json::const_iterator iter(inJSON.find(inName));
		if (iter == inJSON.end())
			return defaultValue;

		// Check value is a string
		if (!iter->is_string())
			return defaultValue;

		// Return value
		return *iter;
	}

	//! Get string
	static std::string GetString(const nlohmann::json& j, const std::string& defaultString = "")
	{
		// Check value is a string
		if (!j.is_string())
			return defaultString;

		return j;
	}
	
	//! Get bool by name
	static bool GetBoolByName(const nlohmann::json& inJSON, const std::string& inName, bool defaultValue = false)
	{
		// Check desired value exists
		nlohmann::json::const_iterator iter(inJSON.find(inName));
		if (iter == inJSON.end())
			return defaultValue;

		// Check value is a bool
		if (!iter->is_boolean())
			return defaultValue;

		// Return value
		return *iter;
	}
	
	//! Get integer by name
	static int GetIntByName(const nlohmann::json& inJSON, const std::string& inName, int defaultValue = 0)
	{
		// Check desired value exists
		nlohmann::json::const_iterator iter(inJSON.find(inName));
		if (iter == inJSON.end())
			return defaultValue;

		// Check value is an integer
		if (!iter->is_number_integer())
			return defaultValue;

		// Return value
		return *iter;
	}
	
	//! Get unsigned integer by name
	static unsigned int GetUnsignedIntByName(const nlohmann::json& inJSON, const std::string& inName, 
											 unsigned int defaultValue = 0)
	{
		// Check desired value exists
		nlohmann::json::const_iterator iter(inJSON.find(inName));
		if (iter == inJSON.end())
			return defaultValue;

		// Check value is an unsigned integer
		if (!iter->is_number_unsigned())
			return defaultValue;

		// Return value
		return *iter;
	}

	//! Get float by name
	static float GetFloatByName(const nlohmann::json& inJSON, const std::string& inName, float defaultValue = 0.0)
	{
		// Check desired value exists
		nlohmann::json::const_iterator iter(inJSON.find(inName));
		if (iter == inJSON.end())
			return defaultValue;

		// Check value is an integer
		if (!iter->is_number_float() && !iter->is_number_integer())
			return defaultValue;

		// Return value
		return *iter;
	}
};
