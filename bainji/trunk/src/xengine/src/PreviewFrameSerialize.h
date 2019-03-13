//
// Created by wendachuan on 2018/12/5.
//

#ifndef PROJECT_PREVIEWFRAMESERIALIZE_H
#define PROJECT_PREVIEWFRAMESERIALIZE_H

#include "xengine/PreviewFrame.h"
#include <libxml/tree.h>

namespace xedit {
    /**
     *
     */
    class CPreviewFrameSerialize {
    public:
        /**
		 * 序列化对象
		 * @param previewFrame 待序列化对象
		 * @param nodeName 对象序列化后的节点名称
		 * @return 序列化后的数据
		 */
        static xmlNodePtr serialize(const PreviewFrame& previewFrame, const char* nodeName = "previewFrame");

        /**
         * 反序列化
         * @param node 待反序列化的数据
         * @param pOutValue 输出对象
         */
        static bool deserialize(const xmlNodePtr& node, PreviewFrame *pOutValue);
    };
}



#endif //PROJECT_PREVIEWFRAMESERIALIZE_H
