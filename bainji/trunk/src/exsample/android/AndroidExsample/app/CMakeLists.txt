cmake_minimum_required(VERSION 3.4.1)
project(native-lib)

include(../config.cmake)

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# add header search directory
include_directories(${XEDIT_INC_DIR})
include_directories(${LIBAV_INC_DIR})

# add library search directory
#link_directories(${XEDIT_LIB_DIR}/third-part/lib/Android/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}
#        ${XEDIT_LIB_DIR}/Android/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}
#        ${XEDIT_LIB_DIR}/../third-part/lib/Android/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}
#        ${LIBAV_LIB_DIR}/Android/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}
#        ${LIBAV_LIB_DIR}/../third-part/lib/Android/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}
#        ${NLE_LIB_DIR}/${ANDROID_ABI})
link_directories(${XEDIT_LIB_DIR}/Android/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}
        ${XEDIT_LIB_DIR}/../third-part/lib/Android/${CMAKE_BUILD_TYPE}/${ANDROID_ABI})

link_libraries(avpub xutil xengine android)

file(GLOB_RECURSE source_files ${XEDIT_GEN_DIR}/jni/*.cpp src/main/cpp/*.cpp)
add_library( ${PROJECT_NAME} SHARED ${source_files} )