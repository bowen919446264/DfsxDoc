//
// Created by wendachuan on 2018/12/5.
//

#ifndef PROJECT_IMAGESERIALIZE_H
#define PROJECT_IMAGESERIALIZE_H

#include "xutil/Image.h"
#include <libxml/tree.h>

namespace xedit {
    class CImageSerialize {
    public:
        /**
		 * 序列化对象
		 * @param media 待序列化对象
		 * @param nodeName 对象序列化后的节点名称
		 * @return 序列化后的数据
		 */
        static xmlNodePtr serialize(const Image& image, const char* nodeName);

        /**
         * 反序列化
         * @param node 待反序列化的数据
         * @param pOutImage 输出对象
         */
        static bool deserialize(const xmlNodePtr& node, Image *pOutImage);
    };
}

#endif //PROJECT_IMAGESERIALIZE_H
