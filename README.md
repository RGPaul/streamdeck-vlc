# Stream Deck VLC Control
A Stream Deck plugin that lets you remotely control the VLC media player.

## Requirements
Installed and running [VLC media player](https://www.videolan.org/vlc/). 
To be able to remotely control the VLC media player, it is required that VLC is running with enabled Webinterface access.

## Activating the Web Interface
To activate the web interface, click the Tools menu in VLC and select Preferences. 
Click the All option under Show settings to view VLC’s advanced settings. Scroll down in the list of advanced settings and select Main interfaces under the Interface header. 
Click the Web checkbox to enable the HTTP interface. 
Click on LUA under the Main interfaces. Set a password under the LUA-HTTP settings. 
Save your settings and restart VLC. Each time you start VLC, the web server will be started in the background – Windows will prompt you to allow VLC firewall access when you restart it, indicating that the web server is running. 

## Platforms
This plugin supports macOS and Windows 10.

## Licence
Stream Deck VLC Control is licenced under [The MIT License (MIT)](LICENSE) and is not officially associated with Elgato or VideoLAN Organization.

Stream Deck and VLC are trademarks or registered trademarks of [Elgato](https://www.elgato.com/en) and [VideoLAN Organization](https://www.videolan.org/videolan/) respectively.
