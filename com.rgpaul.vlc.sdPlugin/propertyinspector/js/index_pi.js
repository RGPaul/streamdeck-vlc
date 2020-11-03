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

var uuid, settings;

$SD.on('connected', (jsonObj) => 
{
    uuid = jsonObj.uuid;
    $SD.api.getGlobalSettings(uuid);
});

$SD.on('didReceiveGlobalSettings', (jsonObj) => 
{
    settings = jsonObj.payload.settings;
    document.getElementById('vlc_host').value = settings.vlcHost || 'localhost';
    document.getElementById('vlc_port').value = settings.vlcPort || '8080';
    document.getElementById('vlc_password').value = settings.vlcPassword || '';
    document.getElementById('mainWrapper').classList.remove('hidden');
});

$SD.on('sendToPropertyInspector', (jsonObj) => 
{
    const payload = jsonObj.payload;
    const event = payload.event;
    
    if (event != "com.rgpaul.vlc.rpc.state")
        return;
    
    document.getElementById('rpc-state').innerText = payload.state;
    document.getElementById('rpc-state-date').innerText = (new Date()).toLocaleString();
});

function saveSettings()
{
    const vlcHost = document.getElementById('vlc_host').value;
    const vlcPort = document.getElementById('vlc_port').value;
    const vlcPassword = document.getElementById('vlc_password').value;
    
    // only save if something changed
    if (vlcHost == settings.vlcHost && vlcPort == settings.vlcPort && vlcPassword == settings.vlcPassword)
        return;
    
    settings.vlcHost = vlcHost;
    settings.vlcPort = vlcPort;
    settings.vlcPassword = vlcPassword;
    $SD.api.setGlobalSettings(uuid, settings);
};
