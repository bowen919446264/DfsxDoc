//
// Created by wendachuan on 2018/11/22.
//

#include "BaseAction.h"
using namespace xedit;

CBaseAction::CBaseAction(CTimeLine *pTimeLine): m_pTimeLine(pTimeLine) {
    m_id = idGenerateOne();
}

CBaseAction::~CBaseAction() {

}

/**
 * 获取动作参数
 * @param key 参数key, AP_*
 * @return
 */
const char* CBaseAction::getActionParam(const char* key) const {
    map<string, string>::const_iterator it = m_mapParams.find(key);
    if (it == m_mapParams.end()) {
        return NULL;
    }
    return it->second.c_str();
}

/**
 * 获取动作编号
 * @return
 */
ID CBaseAction::getId() {
    return m_id;
}

/**
 * 动作是否可以取消
 * @return
 */
bool CBaseAction::canCancel() const {
    return false;
}

/**
 * 动作是否已取消
 * @return
 */
bool CBaseAction::isCanceled() const {
    return false;
}
