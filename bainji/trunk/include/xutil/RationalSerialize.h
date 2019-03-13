//
// Created by wendachuan on 2018/11/19.
//

#ifndef PROJECT_RATIONALSERIALIZE_H
#define PROJECT_RATIONALSERIALIZE_H

#include "avpub/Define.h"
#include "avpub/Rational.h"
#include <libxml/tree.h>

using namespace libav;

#define RationalPtr Rational*

namespace xedit {
    /**
     * Rational xml序列化和反序列化辅助类
     */
    API_IMPORT_EXPORT
    class CRationalSerializeHelper {
    public:
        /**
		 * 序列化对象
		 * @param object 待序列化对象
		 * @param nodeName 对象序列化后的节点名称
		 * @return 序列化后的数据
		 */
        static xmlNodePtr serialize(const Rational& object, const char* nodeName);

        /**
         * 反序列化
         * @param node 待反序列化的数据
         * @param pOutValue 输出对象
         */
        static bool deserialize(const xmlNodePtr& node, RationalPtr pOutValue);
    };
}

#endif //PROJECT_RATIONALSERIALIZE_H
