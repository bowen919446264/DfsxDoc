//
// Created by wendachuan on 2018/12/11.
//

#ifndef PROJECT_DSFILEINFOSERIALIZE_H
#define PROJECT_DSFILEINFOSERIALIZE_H

#include <libxml/tree.h>
#include "NLEHeader.h"

namespace xedit {
    class CDsFileInfoSerialize {
    public:

        /**
		 * 序列化对象
		 * @param fileInfo 待序列化对象
		 * @param nodeName 对象序列化后的节点名称
		 * @return 序列化后的数据
		 */
        static xmlNodePtr serialize(const SDsFileInfo& fileInfo, const char* nodeName = "dsFileInfo");

        /**
         * 反序列化
         * @param node 待反序列化的数据
         * @param pOutFileInfo
         * @return
         */
        static bool deserialize(const xmlNodePtr& node, SDsFileInfo *pOutFileInfo);
    };

    class CSDsVideoFileInfoSerialize {
    public:

        /**
		 * 序列化对象
		 * @param fileInfo 待序列化对象
		 * @param nodeName 对象序列化后的节点名称
		 * @return 序列化后的数据
		 */
        static xmlNodePtr serialize(const SDsVideoFileInfo& fileInfo, const char* nodeName = "dsVideoFileInfo");

        /**
         * 反序列化
         * @param node 待反序列化的数据
         * @param pOutFileInfo
         * @return
         */
        static bool deserialize(const xmlNodePtr& node, SDsVideoFileInfo *pOutFileInfo);
    };

    class CSDsAudioFileInfoSerialize {
    public:

        /**
		 * 序列化对象
		 * @param fileInfo 待序列化对象
		 * @param nodeName 对象序列化后的节点名称
		 * @return 序列化后的数据
		 */
        static xmlNodePtr serialize(const SDsAudioFileInfo& fileInfo, const char* nodeName = "dsAudioFileInfo");

        /**
         * 反序列化
         * @param node 待反序列化的数据
         * @param pOutFileInfo
         * @return
         */
        static bool deserialize(const xmlNodePtr& node, SDsAudioFileInfo *pOutFileInfo);
    };
}

#endif //PROJECT_DSFILEINFOSERIALIZE_H
