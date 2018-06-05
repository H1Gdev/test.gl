cmake_minimum_required(VERSION 3.4.1)

include_directories(..)
include_directories(inc src)
link_directories(.)

set(COMMON_SOURCE_FILES
    src/util/util.h
    src/util/util.cpp
    src/util/half_float.h
    src/util/half_float.cpp
    src/util/cl_wrapper.h
    src/util/cl_wrapper.cpp
)

add_library(opencl-sdk SHARED
            ${COMMON_SOURCE_FILES}
            src/examples/basic/hello_world.cpp
            src/examples/basic/qcom_block_match_sad.cpp
            opencl.cpp)

add_definitions(-DEXECUTE_IN_JNI)

find_library(log-lib log)
find_library(android-lib android)

target_link_libraries(opencl-sdk OpenCL ${android-lib} ${log-lib})
