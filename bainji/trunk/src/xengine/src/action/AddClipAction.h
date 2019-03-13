//
// Created by wendachuan on 2018/11/27.
//

#ifndef PROJECT_ADDCLIPACTION_H
#define PROJECT_ADDCLIPACTION_H

#include "BaseAction.h"
#include <map>
#include <string>
#include "avpub/Rational.h"
using namespace std;
using namespace libav;

namespace xedit {
    /**
     * 动作：添加切片
     */
    class CAddClipAction: public CBaseAction {
    public:
        CAddClipAction(CTimeLine *pTimeLine, ID trackId, ID mediaId, Rational rOffsetOnTrack);
        CAddClipAction(CTimeLine *pTimeLine, ID trackId, ID mediaId, Rational rOffsetOnTrack, Rational rOffsetInMedia, Rational rDuration);
        virtual ~CAddClipAction();

        /**
         * 获取动作类型
         * @return
         */
        EActionType getActionType() const { return AT_AddClip; }

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

        /**
         * 获得切片id
         * @return
         */
        ID getClipId() const;

    private:
        ID          m_trackId;
        ID          m_mediaId;
        Rational    m_rOffsetOnTrack;
        Rational    m_rOffsetInMedia;
        Rational    m_rDuration;

        ID          m_clipId;
    };
};

#endif //PROJECT_ADDCLIPACTION_H
