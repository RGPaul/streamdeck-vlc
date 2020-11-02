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

#include <nlohmann/json.hpp>

struct VlcStatus
{
    enum PlayState {
        Unknown = 0,
        Playing = 1,
        Paused = 2,
        Stopped = 3
    };

public:
	
    VlcStatus();
	explicit VlcStatus(const nlohmann::json &payload);
	
    // Accessors
    PlayState playState() const;
    int volume() const;

    std::string albumArtist() const;
    std::string albumTitle() const;
    std::string artworkUrl() const;
    std::string songTitle() const;

private:
    PlayState _playState { PlayState::Unknown };
    int _volume { 0 };
    std::string _albumArtist;
    std::string _albumTitle;
    std::string _artworkUrl;
    std::string _songTitle;

    void readJsonPayload(const nlohmann::json &payload);
};
