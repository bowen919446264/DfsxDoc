#!/bin/sh
#
# android build script
#
# Copyright (c) 2018 成都东方盛行电子有限责任公司
#

# 当前脚本文件所在目录
CURRENT_PATH=$(cd "$(dirname "$0")";pwd)

source ${CURRENT_PATH}/pub.sh
CURRENT_PATH=$(unix_path2windows_path "${CURRENT_PATH}")

OLD_WORKING_DIR=`pwd`

SOURCE_ROOT="${CURRENT_PATH}/.."
BUILD_ROOT="${SOURCE_ROOT}"
PREFIX="${SOURCE_ROOT}"

# 显示帮助信息
show_help() {
    cat <<EOF
Usage: configure [options]
Options:
-h|--help                               print this message
--extra-lib-roots=EXTRA_LIB_ROOTS       extra library root directories
EOF
    exit 0
}

OPTION_NAMES=()
OPTION_VALUES=()
OPTIONS=()

# 解析参数
for var in "$@"; do
    value=${var#*=}
    name=${var%"$value"}
    if test -z ${name}; then
        name=${value}
    fi

    case ${name} in
        -h|--help)
            show_help
            ;;
        --extra-lib-roots=)
            EXTRA_LIB_ROOTS="${value}"
            ;;
        *=)
            OPTION_NAMES=( ${OPTION_NAMES[@]} "${name}" )
            OPTION_VALUES=( ${OPTION_VALUES[@]} "${value}" )
            ;;
        *)
            value=$(sh_quote "$var")
            OPTIONS=( ${OPTIONS[@]} "${value}" )
            ;;
    esac
done

HOST_OS=$(get_host_os)
case ${HOST_OS} in
    Darwin)
        ;;
    CYGWIN*)
        CURRENT_PATH=$(cygwin_path2windows_path "${CURRENT_PATH}")
        OLD_WORKING_DIR=$(cygwin_path2windows_path "${OLD_WORKING_DIR}")
        ;;
    *)
        echo "Unsupported host os ${HOST_OS}"
        die
esac

#ARCHS="
#    arm64-v8a
#    armeabi-v7a
#    x86
#    x86_64
#"

ARCHS="
    arm64-v8a
	armeabi-v7a
"

BUILD_TYPES="
    Debug
"

TARGET_OS=Android

# 检查安卓环境
check_android
test $? == 0 || die

ANDROID_HOME=${ANDROID_HOME//\\//}

# 查找cmake目录
for CMAKE_ELEMENT in `ls ${ANDROID_HOME}/cmake`
do
    if [ -d ${ANDROID_HOME}/cmake"/"${CMAKE_ELEMENT} ]; then
        CMAKE_DIR="${ANDROID_HOME}/cmake/${CMAKE_ELEMENT}"
        break
    fi
done
if [ ! ${CMAKE_DIR} ]; then
    echo "cmake directory not found."
    die
fi

MAKE="${CMAKE_DIR}/bin/ninja"

# 清理输出文件(因为每次执行ninja clean默认会清理掉所有CPU架构下的文件，因此编译前，先清理一次)
for ARCH in ${ARCHS}
do
    for BUILD_TYPE in ${BUILD_TYPES}
    do
        BUILD_DIR="${BUILD_ROOT}/CMakeFiles/${TARGET_OS}/${BUILD_TYPE}/${ARCH}"
        if [ -d ${BUILD_DIR} ]; then
            cd ${BUILD_DIR}
            "${MAKE}" clean > /dev/null 2>&1
        fi
    done
done

# 编译
for ARCH in ${ARCHS}
do
    ANDROID_TOOL_CHAIN=$(get_android_toolchain ${ARCH})
    if [ ! ${ANDROID_TOOL_CHAIN} ]; then
        echo "Can not find android toolchain for architecture ${ARCH}."
        die
    fi

    # libxml 需要api level 为24
    CMAKE_PARAMETERS=(-H.
        -DCMAKE_TOOLCHAIN_FILE="${ANDROID_NDK_HOME}/build/cmake/android.toolchain.cmake"
        -DANDROID_TOOLCHAIN_NAME="${ANDROID_TOOL_CHAIN}"
        -DCMAKE_GENERATOR=Ninja
        -DCMAKE_MAKE_PROGRAM="${MAKE}"
        -DANDROID_NATIVE_API_LEVEL=24
        )

    for BUILD_TYPE in ${BUILD_TYPES}
    do
        echo "===============Building for ${TARGET_OS} ${ARCH} ${BUILD_TYPE}==============="
        BUILD_DIR="${BUILD_ROOT}/CMakeFiles/${TARGET_OS}/${BUILD_TYPE}/${ARCH}"

        # 配置参数
        CONFIGURE_PARAMETERS=( --target-os=${TARGET_OS} --arch=${ARCH} --build-dir=${BUILD_DIR} --prefix=${PREFIX} ${OPTIONS[@]} )
        if [[ ${BUILD_TYPE} = "Debug" ]]; then
            CONFIGURE_PARAMETERS=( ${CONFIGURE_PARAMETERS[@]} --enable-debug )
        fi

        for i in "${!OPTION_NAMES[@]}"
        do
            NAME="${OPTION_NAMES[$i]}"
            VALUE="${OPTION_VALUES[$i]}"
            CONFIGURE_PARAMETERS=( ${CONFIGURE_PARAMETERS[@]} ${NAME}\"${VALUE}\" )
        done

        # 添加链接参数
        EXTRA_LD_FLAGS="-lz -lm"
        for EXTRA_LIB_ROOT in ${EXTRA_LIB_ROOTS}
        do
            EXTRA_LD_FLAGS="${EXTRA_LD_FLAGS} -L${EXTRA_LIB_ROOT}/${TARGET_OS}/${BUILD_TYPE}/${ARCH}"
        done
        CONFIGURE_PARAMETERS=( ${CONFIGURE_PARAMETERS[@]} --extra-ldflags=\"${EXTRA_LD_FLAGS}\" )

        # 配置
        cd ${SOURCE_ROOT}
        if [ ! -d ${BUILD_DIR} ]; then
            mkdir -p ${BUILD_DIR} || die
        fi
        echo "${CONFIGURE_PARAMETERS[@]}" > ${BUILD_DIR}/CONFIGURE_PARAMETERS.txt
        xargs sh configure < ${BUILD_DIR}/CONFIGURE_PARAMETERS.txt || die

        # 执行cmake
        cmake ${CMAKE_PARAMETERS[@]} -B${BUILD_DIR}  || die

        # 编译
        cd ${BUILD_DIR}
        "${MAKE}" || die
    done
done
