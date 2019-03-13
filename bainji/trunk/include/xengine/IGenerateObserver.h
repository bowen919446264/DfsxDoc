///////////////////////////////////////////////////////////
//  IGenerateObserver.h
//  Implementation of the Interface IGenerateObserver
//  Created on:      22-06-2018 16:58:43
//  Original author: wendachuan
///////////////////////////////////////////////////////////

#if !defined(EA_E79B6017_4D85_40d7_9FD7_6BC8B2DA9948__INCLUDED_)
#define EA_E79B6017_4D85_40d7_9FD7_6BC8B2DA9948__INCLUDED_

#include "GenerateSetting.h"

namespace xedit {
    /**
     * 生成观察者
     */
	class IGenerateObserver {
	public:
        virtual ~IGenerateObserver(){}

	    /**
	     * 生成结束回调处理
	     * @param param 生成参数
	     * @param code
	     */
		virtual void onFinish(const GenerateSetting& param, StatusCode code) =0;

		/**
		 * 更新生成进度回调处理
		 * @param param 生成参数
		 * @param rDuration 当前已经生成的时长(秒)
		 */
		virtual void onUpdateProcess(const GenerateSetting& param, Rational rDuration) =0;
	};
}

#endif // !defined(EA_E79B6017_4D85_40d7_9FD7_6BC8B2DA9948__INCLUDED_)
