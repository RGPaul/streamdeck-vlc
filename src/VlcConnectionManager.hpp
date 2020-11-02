/*
 -----------------------------------------------------------------------------------------------------------------------
 The MIT License (MIT)

 Copyright (c) 2020 Ralph-Gordon Paul. All rights reserved.

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
 rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
 persons to whom the Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
 Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 -----------------------------------------------------------------------------------------------------------------------
*/

#pragma once

#include <string>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <nlohmann/json.hpp>

class VlcConnectionManager
{
public:
	
	VlcConnectionManager();
	virtual ~VlcConnectionManager();
	
	// Accessors
	void setHost(const std::string& host);
	std::string host() const;
	void setPort(const std::string& port);
	std::string port() const;
	void setPassword(const std::string& password);

	bool hasPasswordSet() const;

	/*!
	 * @brief request status from vlc server
	 * @param outPayload output parameter for status response payload on success and error payload on failure
	 * 
	 * @return true on success and and false on failure
	 */
	bool getStatus(nlohmann::json& outPayload) const;

	/*!
	 * @brief send play command to vlc server
	 * @param outPayload output parameter for status response payload on success and error payload on failure
	 * 
	 * @return true on success and and false on failure
	 */
	bool sendPlay(nlohmann::json& outPayload) const;

	/*!
	 * @brief send pause command to vlc server
	 * @param outPayload output parameter for status response payload on success and error payload on failure
	 * 
	 * @return true on success and and false on failure
	 */
	bool sendPause(nlohmann::json& outPayload) const;

private:

    std::string _host { "127.0.0.1" };
    std::string _port { "8080" };
	std::string _password;

	int _httpVersion { 11 }; // HTTP 1.1

	// sends a get request to the given target and writes the output to outPayload, on error the error will be written
	// into the payload
	bool sendGetRequest(const std::string& target, nlohmann::json& outPayload) const;
};
