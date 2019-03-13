//
// Created by wendachuan on 2018/12/6.
//

#ifndef PROJECT_IINNERTRACK_H
#define PROJECT_IINNERTRACK_H

#include "xengine/ITrack.h"
#include "NLEHeader.h"
#include "IInnerClip.h"

namespace xedit {
    class IInnerTrack: public ITrack {
    public:
        virtual ~IInnerTrack() {}

        /**
         * 设置轨道id
         * @param id
         */
        virtual void setId(ID id) =0;

        /**
         * 设置轨道类型
         * @param trackType
         */
        virtual void setTrackType(ETrackType trackType) =0;

        /**
         * 启用轨道
         */
        virtual void enable() =0;

        /**
         * 禁用轨道
         */
        virtual void disable() =0;

        /**
         * 添加切片
         * @param pClip
         */
        virtual StatusCode addClip(IInnerClip *pClip) =0;

        /**
         * 移除指定序号的切片并返回(不)
         * @param nIndex
         * @return
         */
        virtual IInnerClip* popClip(int nIndex) =0;

        /**
         * 绑定IDsTrack
         * @param pDsTrack
         */
        virtual void bindDsTrack(IDsTrack *pDsTrack) =0;

        /**
         * 获得绑定的IDsTrack
         * @return
         */
        virtual IDsTrack* getBindDsTrack() const =0;

        /**
         * 清空轨道
         */
        virtual void clear() =0;
    };
}

#endif //PROJECT_IINNERTRACK_H
