//
// Created by wendachuan on 2019/2/22.
//

#ifndef PROJECT_GROUPACTION_H
#define PROJECT_GROUPACTION_H

#include "BaseAction.h"
#include <vector>

using namespace std;
using namespace libav;

namespace xedit {
    /**
     * 组合动作
     */
    class CGroupAction: public CBaseAction {
    public:
        CGroupAction(CTimeLine *pTimeLine);
        CGroupAction(CTimeLine *pTimeLine, const vector<IInnerAction*>& actions);
        virtual ~CGroupAction();

        /**
         * 获取动作类型
         * @return
         */
        EActionType getActionType() const { return AT_GroupAction; }

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
         * 添加动作
         * @param pAction
         */
        void addAction(IInnerAction *pAction);

    private:
        vector<IInnerAction*>    m_actions;
    };
};

#endif //PROJECT_GROUPACTION_H
