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

#include "VlcConnectionManager.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <websocketpp/base64/base64.hpp>

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>

// ---------------------------------------------------------------------------------------------------------------------
// Constructor / Destructor
// ---------------------------------------------------------------------------------------------------------------------

VlcConnectionManager::VlcConnectionManager() { }

VlcConnectionManager::~VlcConnectionManager() { }

// ---------------------------------------------------------------------------------------------------------------------
// Accessors
// ---------------------------------------------------------------------------------------------------------------------

void VlcConnectionManager::setHost(const std::string& host)
{
    _host = host;
}

std::string VlcConnectionManager::host() const
{
    return _host;
}

void VlcConnectionManager::setPort(const std::string& port)
{
    _port = port;
}

std::string VlcConnectionManager::port() const
{
    return _port;
}

void VlcConnectionManager::setPassword(const std::string& password)
{
    _password = password;
}

// ---------------------------------------------------------------------------------------------------------------------
// Public
// ---------------------------------------------------------------------------------------------------------------------

bool VlcConnectionManager::hasPasswordSet() const
{
    return !_password.empty();
}

bool VlcConnectionManager::getStatus(nlohmann::json& outPayload) const
{
    auto const target = "/requests/status.json";

    return sendGetRequest(target, outPayload);
}

bool VlcConnectionManager::sendPlay(nlohmann::json& outPayload) const
{
    auto const target = "/requests/status.json?command=pl_play";
    
    return sendGetRequest(target, outPayload);
}

bool VlcConnectionManager::sendPause(nlohmann::json& outPayload) const
{
    auto const target = "/requests/status.json?command=pl_pause";
    
    return sendGetRequest(target, outPayload);
}

// ---------------------------------------------------------------------------------------------------------------------
// Private
// ---------------------------------------------------------------------------------------------------------------------

bool VlcConnectionManager::sendGetRequest(const std::string& target, nlohmann::json& outPayload) const
{
    bool gotValidResponse { false };
    std::string authHeader = std::string("Basic ") + websocketpp::base64_encode(":" + _password);

    // io_context is required for input / output
    net::io_context ioc;

    // these objects perform the input / output
    tcp::resolver resolver(ioc);
    beast::tcp_stream stream(ioc);

    try
    {
        // domain lookup
        auto const results = resolver.resolve(_host, _port);

        // create connection to the server
        stream.connect(results);

        // create an http get request
        http::request<http::string_body> req { http::verb::get, target, _httpVersion };
        req.set(http::field::host, _host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req.set(http::field::authorization, authHeader);
            
        // send the http request
        http::write(stream, req);

        // a buffer for reading beast messages
        beast::flat_buffer buffer;

        // container for the response
        http::response<http::string_body> res;

        // receive the http response
        http::read(stream, buffer, res);

        if (res.result() == http::status::ok)
        {
            outPayload = nlohmann::json::parse(res.body());
            gotValidResponse = true;
        }
        else if (res.result() == http::status::unauthorized)
        {
            std::string message = "Authentication to VLC Server failed.";
            std::string logMessage = "Authentication to VLC Server failed.";
            
            nlohmann::json jsonObject;
            jsonObject["error"] = true;
            jsonObject["message"] = message;
            jsonObject["logMessage"] = logMessage;

            outPayload = jsonObject;
        }
        else
        {
            std::string message = "Received Unknown Error from Server.";
            std::string logMessage = std::string("Received Unknown Error from Server: ") 
                + std::to_string(res.result_int());
            
            nlohmann::json jsonObject;
            jsonObject["error"] = true;
            jsonObject["message"] = message;
            jsonObject["logMessage"] = logMessage;

            outPayload = jsonObject;
        }

        // close the socket
        beast::error_code ec;
        stream.socket().shutdown(tcp::socket::shutdown_both, ec);

        // not_connected happens sometimes, throw for all other errors
        if(ec && ec != beast::errc::not_connected) 
        {
            // beast::system_error { ec }
        }
    }
    catch (beast::system_error& e)
    {
        gotValidResponse = false;

        std::string message = "Error connecting to VLC Server.";
        std::string logMessage = std::string("Error connecting to VLC Server: ") + e.what();
        
        nlohmann::json jsonObject;
        jsonObject["error"] = true;
        jsonObject["message"] = message;
        jsonObject["logMessage"] = logMessage;

        outPayload = jsonObject;
    }

    return gotValidResponse;
}