//
// Created by wendachuan on 2018/12/5.
//

#ifndef PROJECT_CTRACK_H
#define PROJECT_CTRACK_H

#include "IInnerTrack.h"
#include <vector>
#include "avpub/TSmartPtr.h"

using namespace std;

namespace xedit {
    class CTimeLine;

    class CTrack: public IInnerTrack {
//        friend class CRemoveClipAction;
//        friend class CAddClipAction;
//        friend class CMoveClipAction;
//        friend class CChangeClipDuration;

    public:
        CTrack(CTimeLine *pTimeLine);
        virtual ~CTrack();

    public: // ITrack接口实现

        /**
         * 获得轨道id
         * @return
         */
        virtual ID getId() const;

        /**
         * 获得轨道类型
         * @return
         */
        virtual ETrackType getTrackType() const;

        /**
         * 轨道当前是否已禁用
         * @return
         */
        virtual bool isDisabled() const;

        /**
         * 获取轨道上切片数量
         * @return
         */
        virtual int getClipCount() const;

        /**
         * 获取指定序号的切片
         * @param nIndex
         * @return
         */
        virtual IClip* getClip(int nIndex) const;

        /**
         * 获取指定id的切片
         */
        virtual IClip* getClipById(ID clipId) const;

        /**
         * 获得指定位置的切片
         * @param rOffsetOnTrack
         * @return
         */
        virtual IClip* getClipByOffset(Rational rOffsetOnTrack) const;

        /**
         * 根据时间，查找切片
         * @param rTime 时间
         * @return
         */
        virtual IClip* findClip(Rational rTime) const;

        /**
         * 判断在指定偏移量处是否有切片存在
         * @param rOffsetOnTrack
         * @return
         */
        virtual bool hasClipOnOffset(Rational rOffsetOnTrack) const;

        /**
         * 添加切片(指定的位置必须没有切片存在)
         * @param mediaId 媒体id
         * @param rOffsetOnTrack 切片在时间线上的偏移量(秒)
         * @return
         */
        virtual IClip* addClip(ID mediaId, Rational rOffsetOnTrack);

        /**
         * 添加流切片
         * @param mediaId 媒体id
         * @param rOffsetOnTrack 切片在时间线上的偏移量(秒)
         * @param nStreamIndex 视/音频流序号
         * @return
         */
        //virtual IClip* addAVStreamClip(ID mediaId, Rational rOffsetOnTrack, int nStreamIndex);

        /**
         * 拆分切片
         * @param clipId 待拆分的切片id
         * @param rSplitOffsetInClip 拆分位置(在切片中的偏移量)
         * @param ppOutLeftClip 拆分后的左侧切片
         * @param ppOutRightClip 拆分后的右侧切片
         * @return
         */
        virtual StatusCode splitClip(ID clipId, Rational rSplitOffsetInClip, IClip **ppOutLeftClip, IClip **ppOutRightClip);

        /**
         * 移动切片 - 移动后其他切片的位置和长度可能发生改变，需要遍历切片，更新切片UI
         * 如果目标位置处于其它切片中间，则会拆分该切片，然后按下述流程处理
         * 定义: 移动前，切片在轨道上的序号为a; 移动后，切片在轨道上的序号为b; 切片时长为d; 插入的目标位置为p
         * 有下面几种情况：
         * 1）a == b, 即移动后，此切片在轨道上的序号未改变，此时只改变切片在时间线上的偏移量即可
         * 2) b < a, 即移动后，此切片在轨道上的序号变小，首先将切片删除，然后将序号范围[b, a-1]的切片向右移动d，最后将切片插入到p位置
         * 3) a < b, 即移动后，此切片在轨道上的序号变大，首先将切片删除，然后将序号范围[a, b-1]的切片向左移动d, 最后将切片插入到p位置
         * @param clipId 待移动的切片id
         * @param rOffsetOnTrack 目标位置
         * @param pOutSplitClipId 如果切片被拆分，则返回被拆分的切片id
         * @param ppOutLeftClip 如果切片被拆分，则返回被拆分后的左侧切片
         * @param ppOutRightClip 如果切片被拆分，则返回被拆分后的右侧切片
         * @return
         */
        virtual StatusCode moveClip(ID clipId, Rational rOffsetOnTrack, ID *pOutSplitClipId, IClip **ppOutLeftClip, IClip **ppOutRightClip);

        /**
         * 改变切片时长(秒) - 会改变在此切片右侧所有切片在时间线上的偏移量, 需要遍历切片，更新切片UI
         * @param clipId 切片id
         * @param rDuration
         * @return
         */
        virtual StatusCode changeClipDuration(ID clipId, const Rational& rDuration);

        /**
         * 改变切片在媒体中的偏移时间(秒) - 会同时改变此切片在时间线上的偏移量，以及其右侧所有切片在时间线上的偏移量, 需要遍历切片，更新切片UI
         * @param clipId 切片id
         * @param rOffsetInMedia
         * @return
         */
        virtual StatusCode changeClipOffsetInMedia(ID clipId, const Rational& rOffsetInMedia);

        /**
         * 将指定切片与其左侧切片交换
         * @param clipId
         * @return
         */
        //virtual IClip* swapWithLeftClip(ID clipId);

        /**
         * 将指定切片与其右侧切片交换
         * @param clipId
         * @return
         */
        //virtual IClip* swapWithRightClip(ID clipId);

        /**
         * 删除指定序号的切片
         * @param nIndex
         */
        virtual StatusCode removeClip(int nIndex);

        /**
         * 删除指定切片
         * @param clipId
         */
        virtual StatusCode removeClipById(ID clipId);

        /**
         * 删除所有切片
         */
        virtual void removeAllClips();

    public: // IInnerTrack 接口实现

        /**
         * 设置轨道id
         * @param id
         */
        virtual void setId(ID id);

        /**
         * 设置轨道类型
         * @param trackType
         */
        virtual void setTrackType(ETrackType trackType);

        /**
         * 启用轨道
         */
        virtual void enable();

        /**
         * 禁用轨道
         */
        virtual void disable();

        /**
         * 添加切片
         * @param pClip
         */
        virtual StatusCode addClip(IInnerClip *pClip);

        /**
         * 移除指定序号的切片并返回(不)
         * @param nIndex
         * @return
         */
        virtual IInnerClip* popClip(int nIndex);

        /**
         * 绑定IDsTrack
         * @param pDsTrack
         */
        virtual void bindDsTrack(IDsTrack *pDsTrack);

        /**
         * 获得绑定的IDsTrack
         * @return
         */
        virtual IDsTrack* getBindDsTrack() const;

        /**
         * 清空轨道
         */
        virtual void clear();

    public:

        vector<IInnerClip*>& getClips() { return m_clips; }

        /**
         * 判断给定的偏移量是否处于切片中间(不含切片首尾两端)
         * @param rOffsetOnTrack
         * @return
         */
        bool offsetInMiddleOfClip(Rational rOffsetOnTrack) const;

        /**
         * 更新切片(在修改了切片信息后，需要调用此函数)
         * @param clipId
         */
        StatusCode updateClip(ID clipId);

        /**
         * 创建切片原子操作
         * @param mediaId
         * @return
         */
        IInnerClip* atomCreateClip(ID mediaId);

        /**
         * 向轨道添加切片原子操作
         * @param pClip
         * @return
         */
        StatusCode atomAddClip(IInnerClip *pClip);

        /**
         * 从轨道中删除切片原子操作
         * @param clipId
         * @return
         */
        StatusCode atomRemoveClip(ID clipId);

        /**
         * 移动切片原子操作
         * @param clipId
         * @param rOffsetOnTrack
         * @return
         */
        StatusCode atomMoveClip(ID clipId, Rational rOffsetOnTrack);

        /**
         * 添加切片
         * @param mediaId 媒体id
         * @param rOffsetOnTrack 切片在时间线上的偏移量(秒)
         * @return
         */
        //IClip* innerAddClip(ID mediaId, Rational rOffsetOnTrack);

        /**
         * 删除切片
         * @param clipId 切片id
         */
        //StatusCode innerRemoveClip(ID clipId);

    public:
        static StatusCode clipSetOffsetOnTrackWrapper(IInnerClip *pClip, const Rational& rOffsetOnTrack, void *pPriv);
        static StatusCode clipSetOffsetInMediaWrapper(IInnerClip *pClip, const Rational& rOffsetInMedia, void *pPriv);
        static StatusCode clipSetDurationWrapper(IInnerClip *pClip, const Rational& rDuration, void *pPriv);

    private:
        CTimeLine*      m_pTimeLine;
        ID              m_id;
        ETrackType      m_trackType;
        bool            m_isDisabled;
        vector<IInnerClip*>  m_clips;
        TSmartPtr<IDsTrack>  m_spDsTrack;
    };

}


#endif //PROJECT_CTRACK_H
