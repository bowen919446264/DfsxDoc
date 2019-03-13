//
// Created by wendachuan on 2018/12/4.
//

#ifndef PROJECT_BASEMEDIASERIALIZE_H
#define PROJECT_BASEMEDIASERIALIZE_H

#include <libxml/tree.h>
#include "IInnerMedia.h"

namespace xedit {
    class CBaseMediaSerialize {
    public:

        /**
		 * 序列化对象
		 * @param pMedia 待序列化对象
		 * @param nodeName 对象序列化后的节点名称
		 * @return 序列化后的数据
		 */
        static xmlNodePtr serialize(const IInnerMedia *pMedia, const char* nodeName = "media");

        /**
         * 反序列化
         * @param node 待反序列化的数据
         */
        static IInnerMedia* deserialize(const xmlNodePtr& node);
    };
}

#endif //PROJECT_BASEMEDIASERIALIZE_H
