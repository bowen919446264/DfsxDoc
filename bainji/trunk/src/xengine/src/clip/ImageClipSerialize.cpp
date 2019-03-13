//
// Created by wendachuan on 2018/12/5.
//

#include "ImageClipSerialize.h"
#include "BaseClipSerialize.h"

using namespace xedit;

/**
 * 序列化对象
 * @param pClip 待序列化对象
 * @param nodeName 对象序列化后的节点名称
 * @return 序列化后的数据
 */
xmlNodePtr CImageClipSerialize::serialize(const IImageClip *pClip, const char* nodeName) {
    return CBaseClipSerialize::serialize(pClip, nodeName);
}

/**
 * 反序列化
 * @param node 待反序列化的数据
 * @return
 */
IInnerClip* CImageClipSerialize::deserialize(const xmlNodePtr& node) {
    return CBaseClipSerialize::deserialize(node);
}