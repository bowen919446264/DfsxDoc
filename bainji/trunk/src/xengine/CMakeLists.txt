# the whole CMakeLists.txt for project xengine
cmake_minimum_required(VERSION 3.6)
project(xengine)

add_compile_options(-std=c++11)

# include directories
include_directories(.)
include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/third-part/include)
include_directories(${CMAKE_SOURCE_DIR}/third-part/include/libxml2)
include_directories(${CMAKE_SOURCE_DIR}/third-part/include/ffmpeg)

# add library search directory
link_directories(${CMAKE_SOURCE_DIR}/third-part/lib/${TARGET_OS}/${CMAKE_BUILD_TYPE}/${ARCH})

link_libraries(xutil dsnlengine log4cpp boost_filesystem avformat avcodec avutil swscale swresample)

file (GLOB_RECURSE source_files
        src/*.*
        src/action/*.*
        src/clip/*.*
        src/media/*.*
        src/project/*.*)

set_platform (${PROJECT_NAME} "${source_files}")

