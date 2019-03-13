cmake_minimum_required(VERSION 3.6)

# 检查是否设置了编译目标平台.可选项：iOS, Android or Windows
if (NOT DEFINED TARGET_OS)
    message(FATAL_ERROR "Target os not set. Please excute configure script first.")
endif ()

# 检查是否设置了编译目标CPU架构
if (NOT DEFINED ARCH)
    message(FATAL_ERROR "Architecture not set. Please excute configure script first.")
endif ()

if (NOT DEFINED LIBRARY_OUTPUT_PATH)
    set(LIBRARY_OUTPUT_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/../lib/${TARGET_OS}/${CMAKE_BUILD_TYPE}/${ARCH})
endif()

# 设置工程的编译平台
macro (set_platform project_name source_files)
    if ("${TARGET_OS}" STREQUAL "iOS")
        add_library (${project_name} STATIC ${source_files})
    elseif ("${TARGET_OS}" STREQUAL "Android")
        add_library (${project_name} SHARED ${source_files})
    elseif ("${TARGET_OS}" STREQUAL "Windows")
        add_library (${project_name} SHARED ${source_files})
    else ()
        message (FATAL_ERROR "Unsupported target OS: ${TARGET_OS}. Valid options: iOS, Android, Windows")
    endif ()
endmacro (set_platform)

# 添加依赖库
macro(add_dependency_lib lib_dir lib_name)
    find_library(TARGET_LIBRARY
            ${lib_name}
            PATHS ${lib_dir}
            NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
    if(NOT TARGET_LIBRARY)
        message(FATAL_ERROR "library ${lib_name} not found")
    endif()
    link_libraries(${TARGET_LIBRARY})
endmacro(add_dependency_lib)