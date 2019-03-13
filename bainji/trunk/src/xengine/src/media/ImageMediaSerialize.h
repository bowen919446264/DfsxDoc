//
// Created by wendachuan on 2018/11/20.
//

#ifndef PROJECT_IMAGEMEDIASERIALIZE_H
#define PROJECT_IMAGEMEDIASERIALIZE_H

#include "IInnerMedia.h"
#include "avpub/Define.h"
#include "IXmlSerializable.h"
#include "IXmlDeserializable.h"

namespace xedit {
    /**
     * IInnerImageMedia xml序列化和反序列化辅助类
     */
    class CImageMediaSerialize: public IXmlSerializable<IInnerImageMedia>, IXmlDeserializable<IImageMediaPtr> {
    public:
        /**
		 * 序列化对象
		 * @param pMedia 待序列化对象
		 * @param nodeName 对象序列化后的节点名称
		 * @return 序列化后的数据
		 */
        static xmlNodePtr serialize(const IInnerImageMedia *pMedia, const char* nodeName = "media");

        /**
         * 反序列化
         * @param node 待反序列化的数据
         */
        static IInnerImageMedia* deserialize(const xmlNodePtr& node);
    };
}


#endif //PROJECT_IMAGEMEDIASERIALIZE_H
