//
// Created by wendachuan on 2018/11/16.
//

#ifndef XEDIT_INPUTFILESTREAM_H
#define XEDIT_INPUTFILESTREAM_H

#include "IInputStream.h"
#include <stdio.h>

namespace xedit {
    /**
     * input file stream
     */
    API_IMPORT_EXPORT
    class CInputStream: public IInputStream {
    public:
        CInputStream();
        virtual ~CInputStream();

        /**
         * 打开一个文件
         * @param filename
         * @return 返回0表示成功；否则返回失败代码
         */
        StatusCode open(const char* filename);

        /**
         * 从流中读取数据
         * @param outBuffer 目标buffer
         * @param nReadCount 需要读取的字节数
         * @return 返回读取到的字节数，如果小于0则读取失败
         */
        virtual int read(uint8_t* outBuffer, int nReadCount);

        /**
         * 定位
         * @param nPos 目标位置
         * @return 返回定位后的位置，如果小于0则表示失败
         */
        virtual int64_t seek(int64_t nPos);

        /**
         * 剩下可用字节数
         * @return
         */
        virtual int64_t available();

        /**
         * 跳过指定字节数
         * @param nSkipBytes 指定的字节数
         * @return 新位置
         */
        virtual int64_t skip(int64_t nSkipBytes);

        /**
         * 关闭流
         * @return 返回0表示成功；否则返回失败代码
         */
        virtual StatusCode close();

    private:
        FILE*   m_file;
        char    m_filePath[AV_MAX_PATH];
        int64_t m_fileSize;
    };
}

#endif //XEDIT_INPUTFILESTREAM_H
