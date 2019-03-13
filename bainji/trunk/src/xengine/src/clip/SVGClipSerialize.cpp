//
// Created by wendachuan on 2018/12/7.
//

#include "SVGClipSerialize.h"
#include "BaseClipSerialize.h"

using namespace xedit;

/**
 * 序列化对象
 * @param pClip 待序列化对象
 * @param nodeName 对象序列化后的节点名称
 * @return 序列化后的数据
 */
xmlNodePtr CSVGClipSerialize::serialize(const ISVGClip *pClip, const char* nodeName) {
    return CBaseClipSerialize::serialize(pClip, nodeName);
}

/**
 * 反序列化
 * @param node 待反序列化的数据
 * @return
 */
IInnerClip* CSVGClipSerialize::deserialize(const xmlNodePtr& node) {
    return CBaseClipSerialize::deserialize(node);
}