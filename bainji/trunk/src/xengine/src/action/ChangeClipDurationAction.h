//
// Created by wendachuan on 2019/3/11.
//

#ifndef PROJECT_CHANGECLIPDURATIONACTION_H
#define PROJECT_CHANGECLIPDURATIONACTION_H


#include "BaseAction.h"
#include <map>
#include <string>
#include "avpub/Rational.h"
using namespace std;
using namespace libav;

namespace xedit {
    /**
     * 动作：改变切片时长
     */
    class CChangeClipDurationAction: public CBaseAction {
    public:
        CChangeClipDurationAction(CTimeLine *pTimeLine, ID trackId, ID clipId, Rational rDuration);
        virtual ~CChangeClipDurationAction();

        /**
         * 获取动作类型
         * @return
         */
        EActionType getActionType() const { return AT_ChangeClipOffsetInMedia; }

        /**
         * 执行动作
         * @return
         */
        virtual StatusCode excute();

        /**
		 * 取消执行
		 */
        virtual void cancel();

        /**
         * 回退
         * @return
         */
        virtual StatusCode unDo();

    private:
        /**
         * 改变切片时长
         * @param rOldDuration
         * @param rNewDuration
         * @return
         */
        StatusCode changeClipDuration(Rational rOldDuration, Rational rNewDuration);

    private:
        ID          m_trackId;
        ID          m_clipId;
        Rational    m_rDuration;

        Rational    m_rOldDuration;
    };
};

#endif //PROJECT_CHANGECLIPDURATIONACTION_H
