#!/bin/sh
#
# public script for useful functions
#
# Copyright (c) 2018 成都东方盛行电子有限责任公司
#

# 终止脚本
die() {
    exit 1;
}

# 检查命令是否存在
function cmd_exist() {
    which $1 > /dev/null 2>&1
    return $?
}

# 获得操作系统名称
function get_host_os() {
    # 检查是否是Unix族
    cmd_exist uname
    if [ $? == 0 ]; then
        echo `uname -s 2>&1`
        return
    else
        # 检查是否是Windows
        cmd_exist cmd
        if [ $? == 0 ]; then
            echo "Windows"
            return
        fi
    fi
}

# 获得cpu架构
function get_host_cpu_arch() {
    # 检查是否是Windows
    cmd_exist systeminfo
    if [ $? == 0 ]; then
        # TODO:
        # echo systeminfo|grep 'System Type'
        return
    fi

    # 检查是否是Unix族
    cmd_exist uname
    if [ $? == 0 ]; then
        echo `uname -m 2>&1`
        return
    fi
}

# 命令参数处理
sh_quote() {
    v=$(echo "$1" | sed "s/'/'\\\\''/g")
    test "x$v" = "x${v#*[!A-Za-z0-9_/.+-:]}" || v="$v"
    echo "$v"
}

# 将windows路径转换为unix路径
function unix_path2windows_path() {
    UNIX_PATH=$1
    UNIX_PATH=${UNIX_PATH#/cygdrive}
    MATCH_PATH=`echo ${UNIX_PATH} | grep "^/[a-z]/"`
    if [ ! ${MATCH_PATH} ]; then
        echo "${UNIX_PATH}"
        return 0
    fi

    DISK=${UNIX_PATH:1:1}
    DIR=${UNIX_PATH:2}
    echo "${DISK}:${DIR}"
}

# 获得短路径(Windows)
function windows_long_path2short_path() {
	LONG_PATH="$1"
	SHORT_PATH=$(cygpath -w -s "${LONG_PATH}")
	SHORT_PATH=${SHORT_PATH//\\//}
	echo "${SHORT_PATH}"
}

# 将windows路径转换为cygwin路径
function windows_path2cygwin_path() {
    WIN_PATH="$1"
	WIN_SHORT_PATH=$(windows_long_path2short_path "${WIN_PATH}")
    CYGWIN_PATH=$(cygpath -p ${WIN_SHORT_PATH} -u)
    if [[ "${CYGWIN_PATH}" == /cygdrive/* ]]; then
        echo "${CYGWIN_PATH}"
    else
        COLON=${WIN_PATH:1:1}
        if [ ${COLON} == ":" ]; then
            DISK=${WIN_PATH:0:1}
            DISK=$(echo ${DISK} | tr '[A-Z]' '[a-z]')
            WIN_PATH=${WIN_PATH:2}
            WIN_PATH="/cygdrive/${DISK}${WIN_PATH}"
            echo "${WIN_PATH}"
        else
            die
        fi
    fi
}

# 将cygwin路径转换为windows路径
function cygwin_path2windows_path() {
    CYGWIN_PATH="$1"
    WIN_PATH=$(cygpath -w ${CYGWIN_PATH})
    WIN_SHORT_PATH=$(windows_long_path2short_path "${WIN_PATH}")
    echo "${WIN_SHORT_PATH}"
}

# 由visual studio版本获得msbuild版本
function get_msbuild_version_of_vs_version() {
    VS_VERSION=$1
    case ${VS_VERSION} in
        2005)
            VS_INNER_VERSION="8"
            ;;
        2008)
             VS_INNER_VERSION="9"
            ;;
        2010)
             VS_INNER_VERSION="10"
            ;;
        2012)
             VS_INNER_VERSION="11"
            ;;
        2013)
             VS_INNER_VERSION="12"
            ;;
        2015)
             VS_INNER_VERSION="14"
            ;;
        2017)
             VS_INNER_VERSION="15"
            ;;
        *)
            echo "Unkonwn visual studio version: ${VS_VERSION}"
            return
    esac
    echo ${VS_INNER_VERSION}
}

# 根据android abi 查找对应工具链
function find_android_toolchain() {
    # 获得android编译工具链数组
    NDK_TOOLCHAIN=${ANDROID_NDK_HOME}/toolchains
    ANDROID_TOOLCHAINS=()
    for ELEMENT in `ls $NDK_TOOLCHAIN`
    do
        if [ -d ${NDK_TOOLCHAIN}"/"${ELEMENT} ]
        then
            case ${ELEMENT} in
                aarch64-linux-android-*)
                    ANDROID_TOOLCHAINS=( ${ANDROID_TOOLCHAINS[@]} ${ELEMENT} )
                    ;;
                arm-linux-androideabi-*)
                    ANDROID_TOOLCHAINS=( ${ANDROID_TOOLCHAINS[@]} ${ELEMENT} )
                    ;;
                x86_64-*)
                    ANDROID_TOOLCHAINS=( ${ANDROID_TOOLCHAINS[@]} ${ELEMENT} )
                    ;;
                x86-*)
                    ANDROID_TOOLCHAINS=( ${ANDROID_TOOLCHAINS[@]} ${ELEMENT} )
                    ;;
                *)
                    ;;
            esac
        fi
    done

    for item in ${ANDROID_TOOLCHAINS[@]}
    do
        if [[ $item == $1* ]]; then
            echo $item
            return
        fi
    done
}

# 由cpu架构获取Android Toolchain
function get_android_toolchain() {
    ABI=$1
    ANDROID_TOOL_CHAIN=
    case ${ABI} in
        arm64-v8a)
            ANDROID_TOOL_CHAIN=$(find_android_toolchain "aarch64-linux-android-")
            ;;
        armeabi-v7a)
            ANDROID_TOOL_CHAIN=$(find_android_toolchain "arm-linux-androideabi-")
            ;;
        x86_64)
            ANDROID_TOOL_CHAIN=$(find_android_toolchain "x86_64-")
            ;;
        x86)
            ANDROID_TOOL_CHAIN=$(find_android_toolchain "x86-")
            ;;
    esac
    echo ${ANDROID_TOOL_CHAIN}
}

# 由cpu架构获取Android Toolchain名称
function get_android_toolchain_name() {
    ABI=$1
    ANDROID_TOOL_CHAIN=
    case ${ABI} in
        arm64-v8a)
            echo "aarch64-linux-android"
            ;;
        armeabi-v7a)
            echo "arm-linux-androideabi"
            ;;
        x86_64)
            echo "x86_64-linux-android"
            ;;
        x86)
            echo "i686-linux-android"
            ;;
    esac
}

# 由cpu架构获取Android build Toolchain名称
function get_android_build_toolchain_name() {
    ABI=$1
    ANDROID_TOOL_CHAIN=
    case ${ABI} in
        arm64-v8a)
            echo "aarch64-linux-android"
            ;;
        armeabi-v7a)
            echo "arm-linux-androideabi"
            ;;
        x86_64)
            echo "x86_64"
            ;;
        x86)
            echo "x86"
            ;;
    esac
}

# 由cpu架构获取Android Toolchain的include目录
function get_android_toolchain_include_dir() {
    ABI=$1
    case ${ABI} in
        arm64-v8a)
            INCLUDE_DIR=${ANDROID_NDK_HOME}/sysroot/usr/include/aarch64-linux-android
            ;;
        armeabi-v7a)
            INCLUDE_DIR=${ANDROID_NDK_HOME}/sysroot/usr/include/arm-linux-androideabi
            ;;
        x86_64)
            INCLUDE_DIR=${ANDROID_NDK_HOME}/sysroot/usr/include/x86_64-linux-android
            ;;
        x86)
            INCLUDE_DIR=${ANDROID_NDK_HOME}/sysroot/usr/include/i686-linux-android
            ;;
    esac
    echo ${INCLUDE_DIR}
}

# 由cpu架构获取Android编译工具链目录
function get_android_toolchain_home() {
    ABI=$1
    TOOLCHAIN_PREBUILD_DIR=${ANDROID_NDK_HOME}/toolchains/$(get_android_toolchain ${ABI})/prebuilt
    for ITEM in `ls "${TOOLCHAIN_PREBUILD_DIR}"`
    do
        if [ -d "${TOOLCHAIN_PREBUILD_DIR}/${ITEM}/bin" ]; then
            TOOLCHAIN_HOME="${TOOLCHAIN_PREBUILD_DIR}/${ITEM}"
            echo ${TOOLCHAIN_HOME}
            return
        fi
    done
}

# 由cpu架构获取Android API arch目录名
function get_android_arch_dir_name() {
    ABI=$1
    case ${ABI} in
        arm64-v8a)
            echo "arch-arm64"
            ;;
        armeabi-v7a)
            echo "arch-arm"
            ;;
        x86_64)
            echo "arch-x86_64"
            ;;
        x86)
            echo "arch-x86"
            ;;
    esac
}

# 元素是否存在于数组中
function array_item_exist() {
    ITEM=$1
    ARRAY=$2
    for i in ${ARRAY[*]}
    do
        if [ $i == ${ITEM} ]; then
            return 1
        fi
    done
    return 0
}

# 合并静态库
function merge_static_library() {
    LIB_NAME=$1
    LIB_SEARCH_PATH=$2
    LIB_OUTPUT_PATH=$3

    MERGE_LIBS=()
    for ARCH in "armv7 armv7s arm64 i386 x86_64"
    do
        LIB_PATH=${LIB_SEARCH_PATH}/${ARCH}/lib${LIB_NAME}.a
        if [ -f ${LIB_PATH} ]; then
            MERGE_LIBS=( ${MERGE_LIBS[@]} ${LIB_PATH} )
        fi
    done
    lipo -create "${MERGE_LIBS[@]}" -output ${LIB_OUTPUT_PATH}/lib${LIB_NAME}.a
}

# 自动查找并合并静态库
function auto_merge_static_libraries() {

    LIB_SEARCH_PATH=$1
    LIB_OUTPUT_PATH=$2

    IOS_ARCHS="
        armv7
        armv7s
        arm64
        i386
        x86_64
    "

    # 遍历目录获得需要合并的lib数组
    LIBS=()
    for ARCH in ${IOS_ARCHS}
    do
        SUB_PATH=${LIB_SEARCH_PATH}/${ARCH}
        if [ -d ${SUB_PATH} ]; then
            OLD_PATH=`pwd`
            cd ${SUB_PATH}
            for lib in `ls lib*.a`
            do
                len=${#lib}
                count=`expr ${len} - 5`
                LIB_NAME=${lib:3:$count}
                `array_item_exist ${LIB_NAME} "${LIBS[*]}"`
                if [[ $? -eq 0 ]]; then
                    LIBS=( ${LIBS[@]} ${LIB_NAME} )
                fi
            done
            cd ${OLD_PATH}
        fi
    done

    # 合并lib
    for LIB in ${LIBS[@]}
    do
        MERGE_LIBS=()
        for ARCH in ${IOS_ARCHS[@]}
        do
            if [ -f ${LIB_SEARCH_PATH}/${ARCH}/lib$LIB.a ]; then
                MERGE_LIBS=( ${MERGE_LIBS[@]} ${LIB_SEARCH_PATH}/${ARCH}/lib$LIB.a )
            fi
        done
        lipo -create "${MERGE_LIBS[@]}" -output ${LIB_OUTPUT_PATH}/lib${LIB}.a || return $?
    done
}

# 检查Android环境
check_android() {
    # 检查ANDROID环境变量
    if [ ! ${ANDROID_HOME} ]; then
        echo "The environment ANDROID_HOME is not set!"
        return 1
    fi

    if [ ! ${ANDROID_NDK_HOME} ]; then
        if [ -d ${ANDROID_HOME}/ndk-bundle ]; then
            export ANDROID_NDK_HOME=${ANDROID_HOME}/ndk-bundle
            echo "Use default ANDROID_NDK_HOME=${ANDROID_NDK_HOME}"
        else
            echo "The environment ANDROID_NDK_HOME is not set!"
            return 1
        fi
    fi

    ANDROID_HOME=${ANDROID_HOME//\\//}
    ANDROID_NDK_HOME=${ANDROID_NDK_HOME//\\//}
    return 0
}

# 获得默认的vs安装目录
function get_default_vs_dir() {
    VS_VERSIONS="
        140
        120
        110
        100
        90
        80
    "

    for VERSION in ${VS_VERSIONS}
    do
        VS_COMNTOOLS=`eval echo '$'"VS${VERSION}COMNTOOLS"`
        if [ "${VS_COMNTOOLS}" ]; then
            VS_COMNTOOLS=${VS_COMNTOOLS//\\/\/}
            VS_COMNTOOLS=${VS_COMNTOOLS%/*}
            VS_COMNTOOLS=${VS_COMNTOOLS%/*}
            VS_COMNTOOLS=${VS_COMNTOOLS%/*}
            echo "${VS_COMNTOOLS}"
            return 0
        fi
    done
}

# 获得指定版本的vs安装目录
function get_vs_dir() {
    VS_VERSION=$1
    VS_COMNTOOLS=`eval echo '$'"VS${VS_VERSION}COMNTOOLS"`
    if [ "${VS_COMNTOOLS}" ]; then
        VS_COMNTOOLS=${VS_COMNTOOLS//\\/\/}
        VS_COMNTOOLS=${VS_COMNTOOLS%/*}
        VS_COMNTOOLS=${VS_COMNTOOLS%/*}
        VS_COMNTOOLS=${VS_COMNTOOLS%/*}
        echo "${VS_COMNTOOLS}"
        return 0
    else
        return 1
    fi
}