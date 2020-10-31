//==============================================================================
/**
@file       ESDLocalizer.h

@brief      Utility functions to handle localization

@copyright  (c) 2018, Corsair Memory, Inc.
			(c) 2020, Ralph-Gordon Paul
			This source code is licensed under the MIT-style license found in the LICENSE file.

**/
//==============================================================================

#pragma once

#include <string>
#include <nlohmann/json.hpp>

class ESDLocalizer
{
public:
	
	static void Initialize(const std::string &inLanguageCode);
	
	static std::string GetLocalizedString(const std::string &inDefaultString);

private:
	ESDLocalizer(const std::string &inLanguageCode);
	std::string GetLocalizedStringIntern(const std::string &inDefaultString);

	nlohmann::json mLocalizationData;
};

