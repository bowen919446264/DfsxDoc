//
// Created by wendachuan on 2018/12/5.
//

#ifndef PROJECT_BASECLIP_H
#define PROJECT_BASECLIP_H

#include "IInnerClip.h"
#include "avpub/TSmartPtr.h"
#include "IInnerTrack.h"
#include "IInnerPreview.h"

namespace xedit {
    /**
     * 切片基类
     */
    class CBaseClip: virtual public IInnerClip {
    public:
        CBaseClip();
        virtual ~CBaseClip();

    public: // IClip接口实现

        /**
         * 获得切片id
         * @return
         */
        virtual ID getId() const;

        /**
         * 获得切片引用的媒体id
         * @return
         */
        virtual ID getRefMediaId() const;

        /**
         * 获得切片在时间线上的偏移量
         * @return
         */
        virtual Rational getOffsetOnTrack() const;

        /**
         * 获取切片时长(秒)
         * @return
         */
        virtual Rational getDuration() const;

        /**
         * 获取切片在媒体中的偏移时间(秒)
         * @return
         */
        virtual Rational getOffsetInMedia() const;

    public: // IInnerClip接口实现

        /**
         * 设置切片id
         * @param id
         */
        virtual void setId(ID id);

        /**
         * 设置切片引用的媒体id
         * @param id
         */
        virtual void setRefMediaId(ID id);

        /**
         * 设置切片时长(秒)(并不会立即生效，需要调用ITrack->updateClip来更新)
         * @param rDuration
         */
        virtual void setDuration(const Rational& rDuration);

        /**
         * 设置切片在时间线上的偏移量(并不会立即生效，需要调用ITrack->updateClip来更新)
         * @param rOffsetOnTrack
         * @return
         */
        virtual void setOffsetOnTrack(Rational rOffsetOnTrack);

        /**
         * 设置切片在媒体中的偏移时间(秒)(并不会立即生效，需要调用ITrack->updateClip来更新)
         * @param rOffsetInMedia
         */
        virtual void setOffsetInMedia(const Rational& rOffsetInMedia);

        /**
         * 绑定IDsClip
         * @param pClip
         */
        virtual void bindDsClip(IDsClip *pClip);

        /**
         * 返回绑定的IDsClip
         * @return
         */
        virtual IDsClip* getBindDsClip() const;

    public:

        static const Rational& getOffsetOnTrackWrapper(const IInnerClip *pClip);
//        static StatusCode setOffsetOnTrackWrapper(IInnerClip *pClip, const Rational& rOffsetOnTrack);
//
        static const Rational& getOffsetInMediaWrapper(const IInnerClip *pClip);
//        static StatusCode setOffsetInMediaWrapper(IInnerClip *pClip, const Rational& rOffsetInMedia);
//
        static const Rational& getDurationWrapper(const IInnerClip *pClip);
//        static StatusCode setDurationWrapper(IInnerClip *pClip, const Rational& rDuration);

    protected:
        ID                  m_id;
        ID                  m_refMediaId;
        Rational            m_rOffsetOnTrack;
        Rational            m_rOffsetInMedia;
        Rational            m_rDuration;
        TSmartPtr<IDsClip>  m_spDsClip;
    };

}


#endif //PROJECT_BASECLIP_H
