//
// Created by wendachuan on 2018/11/6.
//

#ifndef XEDIT_OUTPUTFILESTREAM_H
#define XEDIT_OUTPUTFILESTREAM_H

#include "IOutputStream.h"
#include <stdio.h>

namespace xedit {
    /**
     * output file stream
     */
    API_IMPORT_EXPORT
    class COutputFileStream: public IOutputStream {
    public:
        COutputFileStream();
        virtual ~COutputFileStream();

        /**
         * 打开一个文件
         * @param filename
         * @return 返回0表示成功；否则返回失败代码
         */
        StatusCode open(const char* filename);

        /**
         * 向流写入数据
         * @param inBuffer 待写入的数据
         * @param nBufferCount 待写入的字节数
         * @return 写入成功的字节数，如果小于0则表示失败
         */
        virtual int write(const uint8_t* inBuffer, int nBufferCount);

        /**
         * 刷新输出流
         * @return 返回0表示成功；否则返回失败代码
         */
        virtual StatusCode flush();

        /**
         * 关闭输出流
         * @return 返回0表示成功；否则返回失败代码
         */
        virtual StatusCode close();

        /**
         * 定位到指定位置
         * @param nPos 目标位置
         * @return 定位后的位置，如果小于0则表示失败
         */
        virtual int64_t seek(int64_t nPos);

    private:
        FILE*   m_pFile;
        char    m_strFilePath[AV_MAX_PATH];
    };
}

#endif //XEDIT_OUTPUTFILESTREAM_H
