#!/bin/sh
#
# generate jni classes
#
# Copyright (c) 2018 成都东方盛行电子有限责任公司
#

# 当前脚本文件所在目录
CURRENT_PATH=`dirname $0`
CURRENT_PATH=${CURRENT_PATH/\./$(pwd)}

source ${CURRENT_PATH}/pub.sh

# 缓存选项
OPTION_CACHE_FILE="${CURRENT_PATH}/$0.cache"
if [ $# -ne 0 ]; then
    echo -n "$@" > ${OPTION_CACHE_FILE}
fi
CACHED_OPTIONS="$(cat ${OPTION_CACHE_FILE})"

# 显示帮助信息
show_help() {
    cat <<EOF
Usage: configure [options]
Options:
-h|--help                       print this message
--package-name=PACKAGE_NAME     generated package name
--swig-file=SWIG_FILE           swig interface file
--swig-inc-dir=SWIG_INC_DIR     swig include directory
--out-dir=OUT_DIR               output directory
EOF
    exit 0
}

# 解析参数
for var in ${CACHED_OPTIONS}; do
    value=${var#*=}
    name=${var%"$value"}
    if test -z ${name}; then
        name=${value}
    else
        value=$(sh_quote "$value")
    fi

    case ${name} in
        -h|--help)
            show_help
            ;;
        --package-name=)
            PACKAGE_NAME=${value}
            ;;
        --swig-file=)
            SWIG_FILE=${value}
            ;;
        --swig-inc-dir=)
            SWIG_INC_DIRS=( ${SWIG_INC_DIRS[@]} ${value} )
            ;;
        --out-dir=)
            OUT_DIR=${value}
            ;;
        *)
            echo "Unknown option '${name}'. Use -h option to see help information"
            die
    esac
done

test -z ${PACKAGE_NAME} && {
    echo "Package name not set.Please set package name with option --package-name."
    die
}

test -z ${SWIG_FILE} && {
    echo "swig interface file not set.Please set the swig interface file with option --swig-file."
    die
}

test -z ${OUT_DIR} && {
    OUT_DIR="${CURRENT_PATH}/generated"
}

# 检查swig是否存在
cmd_exist swig || {
    echo "swig not found"
    exit 1
}

# 包路径
PACKAGE_PATH="${OUT_DIR}/java/${PACKAGE_NAME//.//}"

# 移除包目录
if [ -d "${PACKAGE_PATH}" ]; then
    rm -r "${PACKAGE_PATH}"
fi

# 创建包目录
if [ ! -d "${PACKAGE_PATH}" ]; then
    mkdir -p "${PACKAGE_PATH}" || exit 1
fi

# jni目录
JNI_DIR="${OUT_DIR}/jni"

# 移除jni目录
if [ -d "${JNI_DIR}" ]; then
    rm -r "${JNI_DIR}"
fi

# 创建jni目录
if [ ! -d "${JNI_DIR}" ]; then
   mkdir -p "${JNI_DIR}" || exit 1
fi

SWIG_PARAMS=( -c++ -cpperraswarn -java -package "${PACKAGE_NAME}" -outdir "${PACKAGE_PATH}" -o "${JNI_DIR}/${PACKAGE_NAME}.cpp" )
for INC_DIR in ${SWIG_INC_DIRS[@]}
do
    SWIG_PARAMS=( ${SWIG_PARAMS[@]} -I${INC_DIR} )
done

# 生成java接口和cpp包装
swig ${SWIG_PARAMS[@]} "${SWIG_FILE}"