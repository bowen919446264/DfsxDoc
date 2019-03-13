//
// Created by wendachuan on 2018/12/5.
//

#ifndef PROJECT_TRACKSERIALIZE_H
#define PROJECT_TRACKSERIALIZE_H

#include "IInnerTrack.h"
#include <libxml/tree.h>

namespace xedit {
    class CTrackSerialize {
    public:
        /**
		 * 序列化对象
		 * @param pTrack 待序列化对象
		 * @param nodeName 对象序列化后的节点名称
		 * @return 序列化后的数据
		 */
        static xmlNodePtr serialize(const ITrack *pTrack, const char* nodeName = "track");

        /**
         * 反序列化
         * @param node 待反序列化的数据
         */
        static IInnerTrack* deserialize(const xmlNodePtr& node);

        /**
         * 创建切片节点
         * @param pClip
         * @return
         */
        static xmlNodePtr xmlCreateClipNode(const IClip *pClip);

        /**
         * 创建一个clips节点
         * @param pTrack
         * @return
         */
        static xmlNodePtr xmlCreateClipsNode(const ITrack *pTrack);

        /**
         * 载入切片
         * @param clipNode
         * @param ppOutClip
         * @return
         */
        static StatusCode xmlLoadClip(const xmlNodePtr clipNode, IInnerClip **ppOutClip);
    };

}


#endif //PROJECT_TRACKSERIALIZE_H
