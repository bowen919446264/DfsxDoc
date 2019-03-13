//
// Created by wendachuan on 2018/12/7.
//

#ifndef PROJECT_SVGCLIPSERIALIZE_H
#define PROJECT_SVGCLIPSERIALIZE_H

#include "IInnerClip.h"
#include <libxml/tree.h>

namespace xedit {
    class CSVGClipSerialize {
    public:
        /**
		 * 序列化对象
		 * @param pClip 待序列化对象
		 * @param nodeName 对象序列化后的节点名称
		 * @return 序列化后的数据
		 */
        static xmlNodePtr serialize(const ISVGClip* pClip, const char* nodeName = "clip");

        /**
         * 反序列化
         * @param node 待反序列化的数据
         * @return
         */
        static IInnerClip* deserialize(const xmlNodePtr& node);
    };
}

#endif //PROJECT_SVGCLIPSERIALIZE_H
