//
// Created by wendachuan on 2018/12/5.
//

#ifndef PROJECT_AVCLIP_H
#define PROJECT_AVCLIP_H

#include "BaseClip.h"

namespace xedit {
    /**
     * 视音频切片
     */
    class CAVClip: public CBaseClip, public IInnerAVClip {
    public:
        CAVClip();
        virtual ~CAVClip();

        /**
         * 获得切片的视音频流序号
         * @return
         */
        virtual int getStreamIndex() const;

        /**
         * 设置切片的视音频流序号
         * @param nStrmIdx
         */
        virtual void setStreamIndex(int nStrmIdx);

        /**
         * 获取切片在媒体中的偏移时间(秒)
         * @return
         */
        //virtual Rational getOffsetInMedia() const;

        /**
         * 设置切片在媒体中的偏移时间(秒)
         * @param rOffsetInMedia
         */
        //virtual void setOffsetInMedia(const Rational& rOffsetInMedia);

        /**
         * 绑定IDsClip
         * @param pClip
         */
        //virtual void bindDsClip(IDsClip *pClip);

    protected:

        /**
         * 设置切片在媒体中的偏移量
         * @param rOffsetInMedia
         */
        //virtual void innerSetOffsetInMedia(const Rational& rOffsetInMedia);

    protected:
        int         m_nStrmIdx;
    };
}



#endif //PROJECT_AVCLIP_H
