//
// Created by wendachuan on 2019/2/22.
//

#include "GroupAction.h"
#include "xutil/StdUtil.h"
#include "avpub/StatusCode.h"

using namespace xedit;

CGroupAction::CGroupAction(CTimeLine *pTimeLine): CBaseAction(pTimeLine) {

}

CGroupAction::CGroupAction(CTimeLine *pTimeLine, const vector<IInnerAction*>& actions): CBaseAction(pTimeLine) {
    m_actions = actions;
}

CGroupAction::~CGroupAction() {
    destoryVector(&m_actions);
}

/**
 * 执行动作
 * @return
 */
StatusCode CGroupAction::excute() {
    for (vector<IInnerAction*>::iterator it = m_actions.begin(); it != m_actions.end(); it++) {
        IInnerAction *pAction = *it;
        StatusCode code = pAction->excute();
        if (FAILED(code)) return code;
    }
    return AV_OK;
}

/**
 * 取消执行
 */
void CGroupAction::cancel() {

}

/**
 * 回退
 * @return
 */
StatusCode CGroupAction::unDo() {
    for (int i = m_actions.size() - 1; i >= 0; i--) {
        IInnerAction *pAction = m_actions[i];
        StatusCode code = pAction->unDo();
        if (FAILED(code)) return code;
    }
    return AV_OK;
}

/**
 * 添加动作
 * @param pAction
 */
void CGroupAction::addAction(IInnerAction *pAction) {
    m_actions.push_back(pAction);
}