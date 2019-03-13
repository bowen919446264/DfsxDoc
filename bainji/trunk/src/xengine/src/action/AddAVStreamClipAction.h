//
// Created by wendachuan on 2018/12/18.
//

#ifndef PROJECT_ADDAVSTREAMCLIPACTION_H
#define PROJECT_ADDAVSTREAMCLIPACTION_H

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
    class CAddAVStreamClipAction: public CBaseAction {
    public:
        CAddAVStreamClipAction(CTimeLine *pTimeLine, ID trackId, ID mediaId, Rational rOffsetOnTrack, int nStreamIndex);
        virtual ~CAddAVStreamClipAction();

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
        int         m_nStreamIndex;

        ID          m_clipId;
    };
};

#endif //PROJECT_ADDAVSTREAMCLIPACTION_H
