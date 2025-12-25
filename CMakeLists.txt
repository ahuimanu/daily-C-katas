cmake_minimum_required(VERSION 3.23)
project (daily_kata LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

add_executable(daily_kata ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
