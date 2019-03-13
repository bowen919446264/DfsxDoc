//
// Created by wendachuan on 2018/11/22.
//

#ifndef PROJECT_ADDMEDIAACTION_H
#define PROJECT_ADDMEDIAACTION_H

#include "BaseAction.h"
#include "NLEHeader.h"
#include <map>
#include <string>
#include "avpub/Rational.h"
using namespace std;
using namespace libav;

namespace xedit {
    /**
     * 动作：添加媒体
     */
    class CAddMediaAction: public CBaseAction {
    public:
        CAddMediaAction(CTimeLine *pTimeLine, const string& mediaPath);
        virtual ~CAddMediaAction();

        /**
         * 获取动作类型
         * @return
         */
        EActionType getActionType() const { return AT_AddMedia; }

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
         * 返回媒体id
         */
        ID getMediaId() const;

    private:
        string      m_mediaPath;
        ID          m_mediaId;
    };
}

#endif //PROJECT_ADDMEDIAACTION_H
