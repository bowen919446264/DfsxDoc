//
// Created by wendachuan on 2018/12/5.
//

#ifndef PROJECT_BASECLIPSERIALIZE_H
#define PROJECT_BASECLIPSERIALIZE_H

#include "IInnerClip.h"
#include <libxml/tree.h>

namespace xedit {
    class CBaseClipSerialize {
    public:
        /**
		 * 序列化对象
		 * @param pClip 待序列化对象
		 * @param nodeName 对象序列化后的节点名称
		 * @return 序列化后的数据
		 */
        static xmlNodePtr serialize(const IClip *pClip, const char* nodeName = "clip");

        /**
         * 反序列化
         * @param node 待反序列化的数据
         */
        static IInnerClip* deserialize(const xmlNodePtr& node);
    };
}

#endif //PROJECT_BASECLIPSERIALIZE_H
