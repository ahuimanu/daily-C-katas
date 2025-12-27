cmake_minimum_required(VERSION 3.23)
project(daily_kata LANGUAGES C CXX)

add_executable(daily_kata ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
target_compile_features(daily_kata PRIVATE cxx_std_23)
set_target_properties(daily_kata PROPERTIES
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
