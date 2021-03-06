# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# https://cmake.org/cmake/help/latest/command/set.html
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

# https://cmake.org/cmake/help/latest/command/add_library.html
add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/native-lib.cpp )
add_library(app-glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
add_library(native-activity SHARED src/main/cpp/main.cpp)

# https://cmake.org/cmake/help/latest/command/include_directories.html
include_directories(${ANDROID_NDK}/sources/android/native_app_glue/)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

# https://cmake.org/cmake/help/latest/command/find_library.html
find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )
find_library(android-lib android)

find_library(gles3-lib GLESv3)
find_library(egl-lib EGL)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

# https://cmake.org/cmake/help/latest/command/target_link_libraries.html
target_link_libraries( # Specifies the target library.
                       native-lib

                       # Links the target library to the log library
                       # included in the NDK.
                       ${gles3-lib}
                       ${android-lib}
                       ${log-lib} )
target_link_libraries(native-activity app-glue ${gles3-lib} ${egl-lib} ${android-lib} ${log-lib})
