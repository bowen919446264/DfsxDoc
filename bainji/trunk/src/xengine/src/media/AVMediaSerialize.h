//
// Created by wendachuan on 2018/11/20.
//

#ifndef PROJECT_AVMEDIASERIALIZE_H
#define PROJECT_AVMEDIASERIALIZE_H

#include "IInnerMedia.h"
#include "avpub/Define.h"
#include <libxml/tree.h>

#define BaseAVStreamPtr BaseAVStream*
#define VideoStreamPtr  VideoStream*
#define AudioStreamPtr  AudioStream*

namespace xedit {
    /**
     * AVMedia xml序列化和反序列化辅助类
     */
    class CAVMediaSerialize {
    public:
        /**
		 * 序列化对象
		 * @param pMedia 待序列化对象
		 * @param nodeName 对象序列化后的节点名称
		 * @return 序列化后的数据
		 */
        static xmlNodePtr serialize(const IInnerAVMedia *pMedia, const char* nodeName = "media");

        /**
         * 反序列化
         * @param node 待反序列化的数据
         */
        static IInnerAVMedia* deserialize(const xmlNodePtr& node);
    };

    /**
     * BaseAVStream xml序列化和反序列化辅助类
     */
    API_IMPORT_EXPORT
    class CBaseAVStreamSerialize {
    public:
        /**
		 * 序列化对象
		 * @param object 待序列化对象
		 * @param nodeName 对象序列化后的节点名称
		 * @return 序列化后的数据
		 */
        static xmlNodePtr serialize(const BaseAVStream& object, const char* nodeName);

        /**
         * 反序列化
         * @param node 待反序列化的数据
         * @param pOutStream 输出对象
         */
        static bool deserialize(const xmlNodePtr& node, BaseAVStreamPtr pOutStream);
    };

    /**
     * VideoStream xml序列化和反序列化辅助类
     */
    API_IMPORT_EXPORT
    class CVideoStreamSerialize {
    public:
        /**
		 * 序列化对象
		 * @param object 待序列化对象
         * @param pPreview 预览
		 * @param nodeName 对象序列化后的节点名称
		 * @return 序列化后的数据
		 */
        static xmlNodePtr serialize(const VideoStream& object, IPreview *pPreview = NULL, const char* nodeName = "videoStream");

        /**
         * 反序列化
         * @param node 待反序列化的数据
         * @param pOutStream 输出对象
         * @param ppOutPreview 输出预览
         */
        static bool deserialize(const xmlNodePtr& node, VideoStreamPtr pOutStream, IPreview **ppOutPreview);
    };

    /**
     * AudioStream xml序列化和反序列化辅助类
     */
    API_IMPORT_EXPORT
    class CAudioStreamSerialize {
    public:
        /**
		 * 序列化对象
		 * @param object 待序列化对象
		 * @param nodeName 对象序列化后的节点名称
		 * @return 序列化后的数据
		 */
        static xmlNodePtr serialize(const AudioStream& object, const char* nodeName = "audioStream");

        /**
         * 反序列化
         * @param node 待反序列化的数据
         * @param pOutStream 输出对象
         */
        static bool deserialize(const xmlNodePtr& node, AudioStreamPtr pOutStream);
    };
}

#endif //PROJECT_AVMEDIASERIALIZE_H
