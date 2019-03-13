//
// Created by wendachuan on 2018/11/27.
//

#ifndef PROJECT_REMOVEMEDIAACTION_H
#define PROJECT_REMOVEMEDIAACTION_H

#include "BaseAction.h"
#include <map>
#include <string>
using namespace std;

namespace xedit {
    /**
     * 动作：删除媒体
     */
    class CRemoveMediaAction: public CBaseAction {
    public:
        CRemoveMediaAction(CTimeLine *pTimeLine, ID mediaId);
        virtual ~CRemoveMediaAction();

        /**
         * 获取动作类型
         * @return
         */
        EActionType getActionType() const { return AT_RemoveMedia; }

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
        ID  m_mediaId;
    };
};

#endif //PROJECT_REMOVEMEDIAACTION_H
