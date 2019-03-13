#!/bin/sh
#
# windows build script
#
# Copyright (c) 2018 成都东方盛行电子有限责任公司
#

# 当前脚本文件所在目录
CURRENT_PATH=$(cd "$(dirname "$0")";pwd)
source "${CURRENT_PATH}/pub.sh"
CURRENT_PATH=$(unix_path2windows_path "${CURRENT_PATH}")

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
        --vs-version=)
            VS_VERSION=${value}
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

# 判断是否是Windows
HOST_OS=$(get_host_os)
case ${HOST_OS} in
    CYGWIN*)
        ;;
    *)
        echo "Unsupported host os ${HOST_OS}"
        die
esac

if [ -z "${VS_VERSION}" ]; then
    echo "Visual studio version not set!"
    die
else
    VS_INNER_VERSION=$(get_msbuild_version_of_vs_version ${VS_VERSION})
fi

MS_BUILD_ROOT="C:/Program Files (x86)/MSBuild/${VS_INNER_VERSION}.0"
if [[ ! -d ${MS_BUILD_ROOT} ]]; then
    echo "MSBuild ${VS_INNER_VERSION} not found!"
    die
fi

ARCHS="
    x86
    x86_64
"

BUILD_TYPES="
    Debug
    Release
"

TARGET_OS=Windows

CMAKE_COMMON_PARAMETERS=(
    -DCMAKE_EXE_LINKER_FLAGS="/SAFESEH:NO"
    -DCMAKE_SHARED_LINKER_FLAGS="/SAFESEH:NO"
    -DCMAKE_MODULE_LINKER_FLAGS="/SAFESEH:NO"
)

WINDOWS_OUTPUT_DIR="${CURRENT_PATH}/../lib/Windows"

# 编译
for ARCH in ${ARCHS}
do
    case ${ARCH} in
        x86)
            MSBUILD="${MS_BUILD_ROOT}/Bin/MSBuild.exe"
            CMAKE_GENERATOR="Visual Studio ${VS_INNER_VERSION} ${VS_VERSION}"
            PLATFORM=Win32
            ;;
        x86_64)
            MSBUILD="${MS_BUILD_ROOT}/Bin/amd64/MSBuild.exe"
            CMAKE_GENERATOR="Visual Studio ${VS_INNER_VERSION} ${VS_VERSION} Win64"
            PLATFORM=x64
            ;;
        *)
            echo "Unsupported architecture ${ARCH} for ${TARGET_OS}"
            die
    esac

    for BUILD_TYPE in ${BUILD_TYPES}
    do
        echo "===============Building for ${TARGET_OS} ${ARCH} ${BUILD_TYPE}==============="
        BUILD_DIR="${BUILD_ROOT}/CMakeFiles/${TARGET_OS}/${BUILD_TYPE}/${ARCH}"

        CMAKE_PARAMETERS=(
            ${SOURCE_ROOT}
            ${CMAKE_COMMON_PARAMETERS[@]}
            -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG=\"${WINDOWS_OUTPUT_DIR}/Debug/${ARCH}\"
            -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG=\"${WINDOWS_OUTPUT_DIR}/Debug/${ARCH}\"
            -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE=\"${WINDOWS_OUTPUT_DIR}/Release/${ARCH}\"
            -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE=\"${WINDOWS_OUTPUT_DIR}/Release/${ARCH}\"
            -B${BUILD_DIR}
            -DCMAKE_GENERATOR=\"${CMAKE_GENERATOR}\"
        )

        # 将CMAKE_PARAMETERS保存在文件中（因为含有空格时，直接作为参数无法处理）
        echo ${CMAKE_PARAMETERS[@]} > ${SOURCE_ROOT}/CMAKE_PARAMETERS.txt

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
            EXTRA_LD_FLAGS="${EXTRA_LD_FLAGS} /LIBPATH:${EXTRA_LIB_ROOT}/${TARGET_OS}/${BUILD_TYPE}/${ARCH}"
        done
        CONFIGURE_PARAMETERS=( ${CONFIGURE_PARAMETERS[@]} --extra-ldflags=\"${EXTRA_LD_FLAGS}\" )

        # 配置
        cd ${SOURCE_ROOT}
        if [ ! -d ${BUILD_DIR} ]; then
            mkdir -p ${BUILD_DIR} || die
        fi
        echo "${CONFIGURE_PARAMETERS[@]}" > ${BUILD_DIR}/CONFIGURE_PARAMETERS.txt
        xargs sh configure < ${BUILD_DIR}/CONFIGURE_PARAMETERS.txt || die

        # 执行cmake，生成vc解决方案
        xargs cmake < ${SOURCE_ROOT}/CMAKE_PARAMETERS.txt || die

        # 编译
        "${MSBUILD}" ${BUILD_DIR}/Project.sln /p:Configuration=${BUILD_TYPE} /p:Platform=${PLATFORM} || die
    done
done