//
// Created by wendachuan on 2018/12/25.
//

#ifndef PROJECT_TIMELINEGENERATOR_H
#define PROJECT_TIMELINEGENERATOR_H

#include "avpub/Thread.h"
#include "avpub/TSmartPtr.h"
#include "xengine/GenerateSetting.h"
#include "xengine/IGenerateObserver.h"
#include "NLEHeader.h"
using namespace libav;

namespace xedit {
    class CTimeLine;

    /**
     * 时间线生成类
     */
    class CTimeLineGenerator {
    public:
        CTimeLineGenerator(CTimeLine *pTimeLine);
        ~CTimeLineGenerator();

        /**
         * 生成
         * 异步操作，调用后立即返回，并不代表生成已经完成
         * @param pTimeLine 时间线
         * @param param 生成参数
         * @param pObserver 生成观察者
         * @return 返回0表示成功；否则返回失败代码
         */
        StatusCode generate();

        /**
         * 取消生成
         */
        void cancel();

    protected:
        static HRESULT generateCallback(EDs_EngineStatus engineStatus, long operatorStatus);

    private:
        CTimeLine* m_pTimeLine;
        TDsSmartPtr<IDsComplier>    m_pCompiler;
    };
}


#endif //PROJECT_TIMELINEGENERATOR_H
