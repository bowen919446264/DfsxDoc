//
// Created by wendachuan on 2019/3/11.
//

#ifndef PROJECT_CHANGECLIPOFFSETINMEDIAACTION_H
#define PROJECT_CHANGECLIPOFFSETINMEDIAACTION_H

#include "BaseAction.h"
#include <map>
#include <string>
#include "avpub/Rational.h"
using namespace std;
using namespace libav;

namespace xedit {
    /**
     * 动作：改变切片在文件内的偏移量
     */
    class CChangeClipOffsetInMediaAction: public CBaseAction {
    public:
        CChangeClipOffsetInMediaAction(CTimeLine *pTimeLine, ID trackId, ID clipId, Rational rOffsetInMedia);
        virtual ~CChangeClipOffsetInMediaAction();

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
         * 改变切片在文件内的偏移量
         * @param rOldOffsetInMedia
         * @param rNewOffsetInMedia
         * @return
         */
        StatusCode changeClipOffsetInMedia(Rational rOldOffsetInMedia, Rational rNewOffsetInMedia);

    private:
        ID          m_trackId;
        ID          m_clipId;
        Rational    m_rOffsetInMedia;

        Rational    m_rOldOffsetInMedia;
    };
};


#endif //PROJECT_CHANGECLIPOFFSETINMEDIAACTION_H
