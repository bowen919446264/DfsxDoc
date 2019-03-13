# the whole CMakeLists.txt for libav
cmake_minimum_required(VERSION 3.6)

include(config.cmake)
include(script/pub.cmake)

message("============${CMAKE_SHARED_LINKER_FLAGS}-------------")

# include directories
include_directories(include)

foreach(lib ${LIBS})
    add_subdirectory(src/${lib})
endforeach()
