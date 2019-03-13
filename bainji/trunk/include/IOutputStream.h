///////////////////////////////////////////////////////////
//  IOutputStream.h
//  Implementation of the Interface IOutputStream
//  Created on:      21-06-2018 10:16:36
//  Original author: wendachuan
///////////////////////////////////////////////////////////

#if !defined(EA_75A75BF3_A1F7_468c_A2E7_FC0B4AAE3E41__INCLUDED_)
#define EA_75A75BF3_A1F7_468c_A2E7_FC0B4AAE3E41__INCLUDED_

#include "avpub/Define.h"

namespace xedit {
	/**
	 * 输出流接口
	 */
	class IOutputStream
	{

	public:
		virtual ~IOutputStream() {}

		/**
         * 向流写入数据
         * @param buffer 待写入的数据
         * @param count 待写入的字节数
         * @return 写入成功的字节数，如果小于0则表示失败
         */
		virtual int write(const uint8_t* buffer, int count) =0;

		/**
         * 刷新输出流
         * @return 返回0表示成功；否则返回失败代码
         */
		virtual StatusCode flush() =0;

		/**
         * 关闭输出流
         * @return 返回0表示成功；否则返回失败代码
         */
		virtual StatusCode close() =0;

		/**
         * 定位到指定位置
         * @param pos 目标位置
         * @return 定位后的位置，如果小于0则表示失败
         */
		virtual int64_t seek(int64_t pos) =0;
	};
}

#endif // !defined(EA_75A75BF3_A1F7_468c_A2E7_FC0B4AAE3E41__INCLUDED_)
