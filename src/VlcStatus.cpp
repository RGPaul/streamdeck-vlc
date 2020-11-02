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

#include "VlcStatus.hpp"

#include <StreamDeckSdk/EPLJSONUtils.h>

// ---------------------------------------------------------------------------------------------------------------------
// Constructor / Destructor
// ---------------------------------------------------------------------------------------------------------------------

VlcStatus::VlcStatus() {}

VlcStatus::VlcStatus(const nlohmann::json &payload)
{
    readJsonPayload(payload);
}

// ---------------------------------------------------------------------------------------------------------------------
// Accessors
// ---------------------------------------------------------------------------------------------------------------------

VlcStatus::PlayState VlcStatus::playState() const
{
    return _playState;
}

int VlcStatus::volume() const
{
    return _volume;
}

std::string VlcStatus::albumArtist() const
{
    return _albumArtist;
}

std::string VlcStatus::albumTitle() const
{
    return _albumTitle;
}

std::string VlcStatus::artworkUrl() const
{
    return _artworkUrl;
}

std::string VlcStatus::songTitle() const
{
    return _songTitle;
}

// ---------------------------------------------------------------------------------------------------------------------
// Private
// ---------------------------------------------------------------------------------------------------------------------

void VlcStatus::readJsonPayload(const nlohmann::json &payload)
{
    // play state
    std::string playstate = EPLJSONUtils::GetStringByName(payload, "state");
    if (playstate == "playing")
        _playState = PlayState::Playing;
    else if (playstate == "paused")
        _playState = PlayState::Paused;
    else if (playstate == "stopped")
        _playState = PlayState::Stopped;

    // volume
    _volume = EPLJSONUtils::GetIntByName(payload, "volume");

    // read album meta
    nlohmann::json information;
    EPLJSONUtils::GetObjectByName(payload, "information", information);
    nlohmann::json category;
    EPLJSONUtils::GetObjectByName(information, "category", category);
    nlohmann::json meta;
    EPLJSONUtils::GetObjectByName(category, "meta", meta);

    _albumArtist = EPLJSONUtils::GetStringByName(meta, "artist");
    _albumTitle = EPLJSONUtils::GetStringByName(meta, "album");
    _artworkUrl = EPLJSONUtils::GetStringByName(meta, "artwork_url");
    _songTitle = EPLJSONUtils::GetStringByName(meta, "title");
}
