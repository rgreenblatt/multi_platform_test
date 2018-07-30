# Project's name
PROJECT ( example C CXX )

# Specify the minimum version for CMake

cmake_minimum_required(VERSION 3.1)

add_definitions(-std=c++11)

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package (Threads REQUIRED)

# The following folder will be included
include_directories("${PROJECT_SOURCE_DIR}/include")

add_executable(test_platform_linux_socket ${PROJECT_SOURCE_DIR}/test_platform.cpp)
if (${CMAKE_LIBRARY_ARCHITECTURE} STREQUAL "arm-linux-gnueabihf") # Jetson TK1 / Pi
    target_link_libraries(test_platform_linux_socket ${CMAKE_SOURCE_DIR}/lib/raspberry/libCTRE_PhoenixPlatformLinuxSocketCan.a)
else()
    target_link_libraries(test_platform_linux_socket ${CMAKE_SOURCE_DIR}/lib/x86-64/libCTRE_PhoenixPlatformLinuxSocketCan.a)
endif()

target_link_libraries(test_platform_linux_socket Threads::Threads)

add_executable(test_platform_read_linux_socket ${PROJECT_SOURCE_DIR}/test_platform_read.cpp)
if (${CMAKE_LIBRARY_ARCHITECTURE} STREQUAL "arm-linux-gnueabihf") # Jetson TK1 / Pi
    target_link_libraries(test_platform_read_linux_socket ${CMAKE_SOURCE_DIR}/lib/raspberry/libCTRE_PhoenixPlatformLinuxSocketCan.a)
else()
    target_link_libraries(test_platform_read_linux_socket ${CMAKE_SOURCE_DIR}/lib/x86-64/libCTRE_PhoenixPlatformLinuxSocketCan.a)
endif()

target_link_libraries(test_platform_read_linux_socket Threads::Threads)
