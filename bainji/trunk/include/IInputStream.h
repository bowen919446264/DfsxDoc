///////////////////////////////////////////////////////////
//  IInputStream.h
//  Implementation of the Interface IInputStream
//  Created on:      21-06-2018 10:16:36
//  Original author: wendachuan
///////////////////////////////////////////////////////////

#if !defined(EA_4BDEFFA0_A334_400e_A90A_EA58C2E49DB5__INCLUDED_)
#define EA_4BDEFFA0_A334_400e_A90A_EA58C2E49DB5__INCLUDED_

#include "avpub/Define.h"

namespace xedit {
	/**
	 * 输入流接口
	 */
	class IInputStream {

	public:
		virtual ~IInputStream() {}

		/**
         * 从流中读取数据
         * @param buffer 目标buffer
         * @param count 需要读取的字节数
         * @return 返回读取到的字节数，如果小于0则读取失败
         */
		virtual int read(uint8_t* buffer, int count) =0;

		/**
         * 定位
         * @param pos 目标位置
         * @return 返回定位后的位置，如果小于0则表示失败
         */
		virtual int64_t seek(int64_t pos) =0;

		/**
         * 剩下可用字节数
         * @return
         */
		virtual int64_t available() =0;

		/**
         * 跳过指定字节数
         * @param n 指定的字节数
         * @return 新位置
         */
		virtual int64_t skip(int64_t n) =0;

		/**
		 * 关闭流
		 * @return 返回0表示成功；否则返回失败代码
		 */
		virtual StatusCode close() =0;
	};
}

#endif // !defined(EA_4BDEFFA0_A334_400e_A90A_EA58C2E49DB5__INCLUDED_)
