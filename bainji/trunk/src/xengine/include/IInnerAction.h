//
// Created by wendachuan on 2019/1/4.
//

#ifndef PROJECT_IINNERACTION_H
#define PROJECT_IINNERACTION_H

#include "xengine/IAction.h"

namespace xedit {
    class IInnerAction: public IAction {
    public:
        virtual ~IInnerAction() {}

        /**
         * 获取动作编号
         * @return
         */
        virtual ID getId() =0;

        /**
         * 执行动作
         * @return
         */
        virtual StatusCode excute() =0;

        /**
         * 动作是否可以取消
         * @return
         */
        virtual bool canCancel() const =0;

        /**
         * 取消执行
         */
        virtual void cancel() =0;

        /**
         * 动作是否已取消
         * @return
         */
        virtual bool isCanceled() const =0;

        /**
         * 回退
         * @return
         */
        virtual StatusCode unDo() =0;
    };
}

#endif //PROJECT_IINNERACTION_H
