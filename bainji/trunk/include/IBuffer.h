///////////////////////////////////////////////////////////
//  IBuffer.h
//  Implementation of the Interface IBuffer
//  Created on:      21-06-2018 10:16:36
//  Original author: wendachuan
///////////////////////////////////////////////////////////

#if !defined(EA_E253F003_0862_41e8_AF9C_98BFFC20B26E__INCLUDED_)
#define EA_E253F003_0862_41e8_AF9C_98BFFC20B26E__INCLUDED_

#include "GSize.h"
#include "avpub/EPixFormat.h"
#include "avpub/Define.h"

using namespace libav;

namespace xedit {

	/**
     * buffer类型
     */
	enum EBufferType {
		EBufferType_Invalid,
		EBufferType_Video,
		EBufferType_Audio,
		EBufferType_Title
	};

	/**
     * buffer位置
     */
	enum EBufferLocation {
		EBufferLocation_Invalid,
		EBufferLocation_Memory,
		EBufferLocation_GPU
	};

    /**
     * buffer接口
     */
	class IBuffer {
	public:
	    virtual ~IBuffer() {}

	    /**
	     * 获得buffer类型
	     * @return
	     */
		virtual EBufferType getType() const =0;
	};

	/**
	 * 视频buffer
	 */
	class IVideoBuffer: public IBuffer {
	public:
		virtual ~IVideoBuffer() {}

		/**
		 * 获得视频矩形
		 * @return
		 */
		virtual GSize getVideoRect() const =0;

		/**
		 * 获得像素格式
		 * @return
		 */
		virtual EPixFormat getPixelFormat() const =0;

		/**
		 * 获得buffer位置
		 * @return
		 */
		virtual EBufferLocation getLocation() const =0;

		/**
		 * 获得平面数(YUV或者RGB平面)
		 * @return
		 */
		virtual int getPlaneCount() const =0;

		/**
		 * 获得平面数据指针
		 * @param nPlane
		 * @return
		 */
		virtual uint8_t* getPlanePointer(int nPlane) const =0;

		/**
		 * 获得平面一行数据大小
		 * @param nPlane
		 * @return
		 */
		virtual int getPlaneLineSize(int nPlane) const =0;

		/**
		 * 获得平面数据大小
		 * @param nPlane
		 * @return
		 */
		virtual int getSize(int nPlane) const =0;
	};
}

#endif // !defined(EA_E253F003_0862_41e8_AF9C_98BFFC20B26E__INCLUDED_)
