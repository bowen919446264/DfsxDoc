//
// Created by wendachuan on 2018/11/27.
//

#ifndef PROJECT_REMOVECLIPACTION_H
#define PROJECT_REMOVECLIPACTION_H

#include "BaseAction.h"
#include <map>
#include <string>
#include <libxml/tree.h>
using namespace std;

namespace xedit {
    /**
     * 动作：删除切片
     */
    class CRemoveClipAction: public CBaseAction {
    public:
        CRemoveClipAction(CTimeLine *pTimeLine, ID trackId, ID clipId);
        virtual ~CRemoveClipAction();

        /**
         * 获取动作类型
         * @return
         */
        EActionType getActionType() const { return AT_RemoveClip; }

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
        ID          m_trackId;
        ID          m_clipId;
        xmlNodePtr  m_clipNode;
    };
};

#endif //PROJECT_REMOVECLIPACTION_H
