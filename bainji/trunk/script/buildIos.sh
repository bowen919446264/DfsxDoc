#!/bin/sh
#
# ios build script
#
# Copyright (c) 2018 成都东方盛行电子有限责任公司
#

# 当前脚本文件所在目录
CURRENT_PATH=$(cd "$(dirname "$0")";pwd)

source ${CURRENT_PATH}/pub.sh

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
--extra-lib-roots=EXTRA_LIB_ROOT        extra library root directories
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

# 判断是否是Mac OS
HOST_OS=$(get_host_os)
test ${HOST_OS} = "Darwin" || die

ARCHS="
    arm64
"

BUILD_TYPES="
    Debug
"

TARGET_OS=iOS

# 编译
for ARCH in ${ARCHS}
do
    CMAKE_TOOLCHAIN_FILE=${SOURCE_ROOT}/toolchain/iOS.cmake
    CMAKE_PARAMETERS=( -H. -DIOS_ARCH=${ARCH} -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} )

    for BUILD_TYPE in ${BUILD_TYPES}
    do
        echo "===============Building for ${TARGET_OS} ${ARCH} ${BUILD_TYPE}==============="
        BUILD_DIR="${BUILD_ROOT}/CMakeFiles/${TARGET_OS}/${BUILD_TYPE}/${ARCH}"
        if [ -d ${BUILD_DIR} ]; then
            cd ${BUILD_DIR}
            make clean > /dev/null 2>&1
        fi

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
        EXTRA_LD_FLAGS=
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
        cmake ${CMAKE_PARAMETERS[@]} -B${BUILD_DIR} || die

        # 编译
        cd ${BUILD_DIR}
        make || die
    done
done

LIB_DIR="${PREFIX}/lib"

# 合并静态库
auto_merge_static_libraries ${LIB_DIR}/iOS/Debug   ${LIB_DIR}/iOS/Debug
auto_merge_static_libraries ${LIB_DIR}/iOS/Release ${LIB_DIR}/iOS/Release

cd ${OLD_WORKING_DIR}
