cmake_minimum_required(VERSION 3.4.1)

include_directories(..)
include_directories(inc)
link_directories(.)

add_library(opencl-sdk SHARED
            opencl.cpp)

add_definitions(-DEXECUTE_IN_JNI)

find_library(log-lib log)
find_library(android-lib android)

target_link_libraries(opencl-sdk OpenCL ${android-lib} ${log-lib})
