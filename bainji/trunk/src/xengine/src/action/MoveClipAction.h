//
// Created by wendachuan on 2018/11/27.
//

#ifndef PROJECT_MOVECLIPACTION_H
#define PROJECT_MOVECLIPACTION_H

#include "BaseAction.h"
#include <map>
#include <string>
#include "avpub/Rational.h"
#include "GroupAction.h"
using namespace std;
using namespace libav;

namespace xedit {
    /**
     * 动作：移动切片
     */
    class CMoveClipAction: public CBaseAction {
    public:
        CMoveClipAction(CTimeLine *pTimeLine, ID trackId, ID clipId, Rational rOffsetOnTrack);
        virtual ~CMoveClipAction();

        /**
         * 获取动作类型
         * @return
         */
        virtual EActionType getActionType() const { return AT_MoveClip; }

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
        StatusCode splitIfNeeded();

    private:
        ID              m_trackId;
        ID              m_clipId;
        Rational        m_rOffsetOnTrack;
        //Rational        m_rOldOffsetOnTrack;
        CGroupAction    m_splitAction;
        CGroupAction    m_moveAction;
    };
};

#endif //PROJECT_MOVECLIPACTION_H
