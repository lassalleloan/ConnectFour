cmake_minimum_required(VERSION 3.8)
project(ConnectFour)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES src/main.cpp src/ConnectFour.cpp src/ConnectFour.h src/IConnectFour.h)
add_executable(ConnectFour ${SOURCE_FILES})