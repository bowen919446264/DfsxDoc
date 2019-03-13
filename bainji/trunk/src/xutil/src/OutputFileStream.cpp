//
// Created by wendachuan on 2018/11/6.
//

#include "xutil/OutputFileStream.h"
#include "avpub/Log.h"
#include "avpub/StatusCode.h"
#include <memory.h>
#include <errno.h>

using namespace xedit;
using namespace libav;

COutputFileStream::COutputFileStream() : m_pFile(NULL) {
    memset(m_strFilePath, 0, sizeof(m_strFilePath));
}

COutputFileStream::~COutputFileStream() {
    close();
}

/**
 * 打开一个文件
 * @param filename
 * @return 返回0表示成功；否则返回失败代码
 */
StatusCode COutputFileStream::open(const char* filename) {
    close();

    FILE *file = fopen(filename, "wb");
    if (!file) {
        AVLOG(ELOG_LEVEL_ERROR, "Open file [%s] to write FAILED! Code: %x, message: %s", filename, errno, strerror(errno));
        return -errno;
    }

    m_pFile = file;
    strcpy(m_strFilePath, filename);
    return AV_OK;
}

/**
 * 向流写入数据
 * @param inBuffer 待写入的数据
 * @param nBufferCount 待写入的字节数
 * @return 写入成功的字节数，如果小于0则表示失败
 */
int COutputFileStream::write(const uint8_t* inBuffer, int nBufferCount) {
    int nItems = fwrite(inBuffer, nBufferCount, 1, m_pFile);
    if (nItems == 1) {
        return nBufferCount;
    } else {
        AVLOG(ELOG_LEVEL_ERROR, "Write to file [%s] FAILED! Code: %d, message: %s", m_strFilePath, errno, strerror(errno));
        return -errno;
    }
}

/**
 * 刷新输出流
 * @return 返回0表示成功；否则返回失败代码
 */
StatusCode COutputFileStream::flush() {
    fflush(m_pFile);
    return AV_OK;
}

/**
 * 关闭输出流
 * @return 返回0表示成功；否则返回失败代码
 */
StatusCode COutputFileStream::close() {
    if (m_pFile) {
        int nCode = fclose(m_pFile);
        m_pFile = NULL;

        return nCode == 0 ? AV_OK : -errno;
    }
    return AV_OK;
}

/**
 * 定位到指定位置
 * @param nPos 目标位置
 * @return 定位后的位置，如果小于0则表示失败
 */
int64_t COutputFileStream::seek(int64_t nPos) {
    int nCode = FSEEK64(m_pFile, nPos, SEEK_SET);
    if (!nCode)
        return AV_OK;
    else {
        AVLOG(ELOG_LEVEL_ERROR, "Seek file [%s] to pos [%I64d] FAILED! Code: %x", m_strFilePath, nPos, errno);
        return -errno;
    }
}