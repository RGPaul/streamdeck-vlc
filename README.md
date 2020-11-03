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
