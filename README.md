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

## Installing on macOS

### Activating VLC Web Interface
1. Go to VLC > Preferences > Interface. There is a section named "HTTP web interface", enable the checkbox for the interface, and set a password. The password is required. 
2. Restart VLC player
3. Ensure you have enabled the player by going to http://localhost:8080 in your web browser. Leave the username blank, and use the password you set. If you see a VLC player menu come up, you have successfully enabled the VLC Web interface.
 
### Configuring Stream Deck VLC Control
1. In your stream deck configuration manager, add a VLC Control button.
2. Click on this button, and set the following parameters, replacing password with the password you previously set.
    + Host: localhost
    + Port: 8080
    + Password: {PASSWORD SET IN VLC}

## Compiling on Windows 10
For compiling on Windows 10 you have to install [Visual Studio 2019](https://visualstudio.microsoft.com) and [CMake](https://cmake.org/).  
You also have to install [Boost](https://www.boost.org/) 1.73.0 or later under `C:\include` and `C:\lib` or pass the `Boost_ROOT` option.  
```
cmake -G "Visual Studio 16 2019" -A x64 -S . -B "build64"
cmake --build build64 --config Release
```

## Compiling on macOS
For compiling on macOS you have to install [Xcode](https://developer.apple.com/xcode/) and [CMake](https://cmake.org/).  
You also have to install [Boost](https://www.boost.org/) 1.73.0 or later under `/usr/local/include` and `/usr/local/lib` or pass the `Boost_ROOT` option.  
```
cmake -S . -B build
cmake --build build --config Release
```

## Licence
Stream Deck VLC Control is licenced under [The MIT License (MIT)](LICENSE) and is not officially associated with Elgato or the VideoLAN Organization.

Stream Deck and VLC are trademarks or registered trademarks of [Elgato](https://www.elgato.com/en) and the [VideoLAN Organization](https://www.videolan.org/videolan/) respectively.
