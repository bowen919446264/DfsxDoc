//
// Created by wendachuan on 2018/11/16.
//

#include "xutil/InputFileStream.h"
#include "avpub/Log.h"
#include "avpub/StatusCode.h"
#include <memory.h>
#include <errno.h>

using namespace xedit;
using namespace libav;

CInputStream::CInputStream() : m_file(NULL) {
    memset(m_filePath, 0, sizeof(m_filePath));
    m_fileSize = 0;
}

CInputStream::~CInputStream() {
    close();
}

/**
 * 打开一个文件
 * @param filename
 * @return 返回0表示成功；否则返回失败代码
 */
StatusCode CInputStream::open(const char* filename) {
    close();

    FILE *file = fopen(filename, "rb");
    if (!file) {
        StatusCode code = errno;
        AVLOG(ELOG_LEVEL_ERROR, "Open file [%s] to read FAILED! Code: %x, message: %s", filename, code, strerror(code));
        return -code;
    }

    fseeko(file, 0, SEEK_END);
    m_fileSize = ftello(file);
    fseeko(file, 0, SEEK_SET);

    m_file = file;
    strcpy(m_filePath, filename);
    return AV_OK;
}

/**
 * 从流中读取数据
 * @param outBuffer 目标buffer
 * @param nReadCount 需要读取的字节数
 * @return 返回读取到的字节数，如果小于0则读取失败
 */
int CInputStream::read(uint8_t* outBuffer, int nReadCount) {
    int nItem = fread(outBuffer, nReadCount, 1, m_file);
    if (nItem == 1) {
        return nReadCount;
    } else {
        AVLOG(ELOG_LEVEL_ERROR, "Read file [%s] FAILED! Code: %d, message: %s", m_filePath, errno, strerror(errno));
        return -errno;
    }
}

/**
 * 定位
 * @param nPos 目标位置
 * @return 返回定位后的位置，如果小于0则表示失败
 */
int64_t CInputStream::seek(int64_t nPos) {
    return fseeko(m_file, nPos, SEEK_SET);
}

/**
 * 剩下可用字节数
 * @return
 */
int64_t CInputStream::available() {
    return m_fileSize - ftello(m_file);
}

/**
 * 跳过指定字节数
 * @param nSkipBytes 指定的字节数
 * @return 新位置
 */
int64_t CInputStream::skip(int64_t nSkipBytes) {
    return fseeko(m_file, nSkipBytes, SEEK_CUR);
}

/**
 * 关闭流
 * @return 返回0表示成功；否则返回失败代码
 */
StatusCode CInputStream::close() {
    if (m_file) {
        int nCode = fclose(m_file);
        m_file = NULL;

        return nCode == 0 ? AV_OK : -errno;
    }
    return AV_OK;
}