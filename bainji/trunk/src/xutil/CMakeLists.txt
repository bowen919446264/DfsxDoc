# the whole CMakeLists.txt for project xengine
cmake_minimum_required(VERSION 3.6)
project(xutil)

add_compile_options(-std=c++11)

# include directories
include_directories(.)
include_directories(${CMAKE_SOURCE_DIR}/third-part/include)
include_directories(${CMAKE_SOURCE_DIR}/third-part/include/libxml2)

# add library search directory
link_directories(${CMAKE_SOURCE_DIR}/third-part/lib/${TARGET_OS}/${CMAKE_BUILD_TYPE}/${ARCH})

link_libraries(avpub xml2 freeimage)

file (GLOB_RECURSE source_files src/*.*)

set_platform (${PROJECT_NAME} "${source_files}")

