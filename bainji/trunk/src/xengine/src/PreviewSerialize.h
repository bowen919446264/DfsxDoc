//
// Created by wendachuan on 2018/12/4.
//

#ifndef PROJECT_MEDIAPREVIEWSERIALIZE_H
#define PROJECT_MEDIAPREVIEWSERIALIZE_H

#include "IInnerPreview.h"
#include <libxml/tree.h>

namespace xedit {
    class CPreviewSerialize {
    public:
        /**
		 * 序列化对象
		 * @param pPreview 待序列化对象
		 * @param nodeName 对象序列化后的节点名称
		 * @return 序列化后的数据
		 */
        static xmlNodePtr serialize(const IPreview* pPreview, const char* nodeName = "preview");

        /**
         * 反序列化
         * @param node 待反序列化的数据
         */
        static IInnerPreview* deserialize(const xmlNodePtr& node);
    };
}



#endif //PROJECT_MEDIAPREVIEWSERIALIZE_H
