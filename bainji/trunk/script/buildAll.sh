#!/bin/sh
#
# build all script
#
# Copyright (c) 2018 成都东方盛行电子有限责任公司
#

# 当前脚本文件所在目录
CURRENT_PATH=$(cd "$(dirname "$0")";pwd)

source ${CURRENT_PATH}/pub.sh || die

# 显示帮助信息
show_help() {
    cat <<EOF
Usage: configure [options]
Options:
-h|--help                               print this message
--include-dirs=INCLUDE_DIRS             include directories
--lib-dirs=LIB_DIRS                     lib directories
--vs-version=VS_VERSION                 set visual studio version, such as: 2010, 2015, 2017
EOF
    exit 0
}

# 缓存选项
OPTION_CACHE_FILE=${CURRENT_PATH}/$0.cache

if [ -f "${OPTION_CACHE_FILE}" ]; then
    xargs sh "$0" < ${OPTION_CACHE_FILE} || die
    exit 0
else
    CACHE_OPTIONS=

    # 解析参数
    for var in "$@"; do
        value=${var#*=}
        name=${var%"$value"}
        if test -z ${name}; then
            name=${value}
        #else
        #    value=$(sh_quote "$value")
        fi

        case ${name} in
            -h|--help)
                show_help
                ;;
            --include-dirs=)
                INCLUDE_DIRS="${value}"
                CACHE_OPTIONS="${CACHE_OPTIONS} --include-dirs=\"${value}\" "
                ;;
            --lib-dirs=)
                LIB_DIRS="${value}"
                CACHE_OPTIONS="${CACHE_OPTIONS} --lib-dirs=\"${value}\" "
                ;;
            --vs-version=)
                VS_VERSION="${value}"
                CACHE_OPTIONS="${CACHE_OPTIONS} --vs-version=\"${value}\" "
                ;;
            *)
                OPTIONS="${OPTIONS} ${var} "
                ;;
        esac
        CACHE_OPTIONS="${CACHE_OPTIONS} ${OPTIONS}"
        echo "${CACHE_OPTIONS}" > ${OPTION_CACHE_FILE}
    done
fi

HOST_OS=$(get_host_os)

THIRD_PART_INCLUDE_DIR=$(cygwin_path2windows_path "${CURRENT_PATH}/../third-part/include")
INCLUDE_DIRS="${INCLUDE_DIRS} ${THIRD_PART_INCLUDE_DIR}/libav ${THIRD_PART_INCLUDE_DIR}/DsNleLib/linux/dsnlengine ${THIRD_PART_INCLUDE_DIR}/DsNleLib/inc ${THIRD_PART_INCLUDE_DIR}/DsNleLib/android/dsnlecore/include"

# 解析参数
EXTRA_CXXFLAGS=
for INC_DIR in ${INCLUDE_DIRS}
do
    EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS} -I${INC_DIR}"
done

# 编译iOS
if [ ${HOST_OS} = "Darwin" ]; then
    bash ${CURRENT_PATH}/buildIos.sh --extra-cxxflags="${EXTRA_CXXFLAGS}" --extra-lib-roots="${LIB_DIRS}"
    if [ $? -ne 0 ]; then
        echo "Build for ios failed!"
        die
    fi
fi

# 编译Android
bash ${CURRENT_PATH}/buildAndroid.sh --extra-cxxflags="${EXTRA_CXXFLAGS}" --extra-lib-roots="${LIB_DIRS}"
if [ $? -ne 0 ]; then
    echo "Build for android failed!"
    die
fi

# 编译Windows


echo "All done."
