#
#  CMakeLists.txt
#
#  Created by Ralph-Gordon Paul on @date
#  Copyright (c) 2020 Ralph-Gordon Paul. All rights reserved.
#

cmake_minimum_required (VERSION 3.18)
project (vlc-remote)
set(CMAKE_CXX_STANDARD 17)

# For Visual Studio Code
include(CMakeToolsHelpers OPTIONAL)

# we prefer libraries over frameworks
set(CMAKE_FIND_FRAMEWORK LAST)

# add local cmake modules to module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Conan
if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(TARGETS)
endif()

# on unix systems, the dependencies should be installed under /usr/local/lib /usr/local/include
if(UNIX)
  set(CMAKE_PREFIX_PATH "/usr/local/")
  set(CMAKE_INCLUDE_PATH "/usr/local/include/" CACHE FILEPATH "Path to headers (include) for third party libraries")
  set(CMAKE_LIBRARY_PATH "/usr/local/lib/" CACHE FILEPATH "Path to third party libraries")
  include_directories(BEFORE ${CMAKE_INCLUDE_PATH})
  link_directories(${CMAKE_LIBRARY_PATH})
endif()

# on windows systems, the dependencies should be installed under C:/lib and C:/include
if (WIN32)
  set(CMAKE_INCLUDE_PATH "C:/include" CACHE FILEPATH "Path to headers (include) for third party libraries")
  set(CMAKE_LIBRARY_PATH "C:/lib" CACHE FILEPATH "Path to third party libraries")
  include_directories(BEFORE ${CMAKE_INCLUDE_PATH})
  link_directories(${CMAKE_LIBRARY_PATH})
endif()

# setup clang compiler options
if((CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
  message("Setting clang options")
  set (CMAKE_CXX_FLAGS "-std=c++17 -stdlib=libc++")
endif()

### link external libraries ###

# Boost - https://www.boost.org
set(Boost_USE_STATIC_LIBS ON )
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost 1.73.0 COMPONENTS system random)

# nlohmann json - https://github.com/nlohmann/json
set(NlohmannJson_INCLUDE    "${CMAKE_SOURCE_DIR}/deps/nlohmann_json/include")

# Websocket++ - https://github.com/zaphoyd/websocketpp
set(Websocketpp_INCLUDE "${CMAKE_SOURCE_DIR}/deps/websocketpp/include")

# StreamDeckSdk - taken and modified from Elgato sample: https://github.com/elgatosf/streamdeck-cpu
set(StreamDeckSdk_INCLUDE   "${CMAKE_SOURCE_DIR}/deps/StreamDeckSdk/include")
set(StreamDeckSdk_SRC   "${CMAKE_SOURCE_DIR}/deps/StreamDeckSdk/ESDConnectionManager.cpp"
                        "${CMAKE_SOURCE_DIR}/deps/StreamDeckSdk/ESDLocalizer.cpp")

### create executable ###

add_executable(vlc-remote
                ${StreamDeckSdk_SRC}
                src/VlcConnectionManager.cpp
                src/VlcStatus.cpp
                src/VlcStreamDeckPlugin.cpp
                src/main.cpp)

if(APPLE)
    # add macos specific sources
    target_sources(vlc-remote PRIVATE "${CMAKE_SOURCE_DIR}/deps/StreamDeckSdk/ESDUtilitiesMac.cpp")
endif()

if (WIN32)
    # add windows specific sources
    target_sources(vlc-remote PRIVATE "${CMAKE_SOURCE_DIR}/deps/StreamDeckSdk/ESDUtilitiesWindows.cpp")

    # set msvc runtime property
    set_property(TARGET vlc-remote PROPERTY
                    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

### link libraries ###

target_include_directories(vlc-remote PRIVATE 
                            ${Boost_INCLUDE_DIRS}
                            ${NlohmannJson_INCLUDE}
                            ${Websocketpp_INCLUDE}
                            ${StreamDeckSdk_INCLUDE}
                            )

target_link_libraries(vlc-remote
                        ${Boost_LIBRARIES})

if(APPLE)
    target_link_libraries(vlc-remote
                            "-framework Foundation")
endif()

### PCH ###

if(APPLE)
    target_precompile_headers(vlc-remote PUBLIC "${CMAKE_SOURCE_DIR}/src/macos_pch.hpp")
endif()

if (WIN32)
    target_precompile_headers(vlc-remote PUBLIC "${CMAKE_SOURCE_DIR}/src/windows_pch.hpp")
endif()

# add posix thread library for Linux, BSD, Solaris, Minix, ...
if(UNIX AND NOT APPLE)
    target_link_libraries(vlc-remote dl pthread)
endif()

# installation 
install(TARGETS vlc-remote
    RUNTIME DESTINATION bin)
