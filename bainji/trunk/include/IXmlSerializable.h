///////////////////////////////////////////////////////////
//  IXmlSerializableT.h
//  Implementation of the Interface IXmlSerializable<T>
//  Created on:      21-06-2018 10:16:37
//  Original author: wendachuan
///////////////////////////////////////////////////////////

#if !defined(EA_F6D0FE68_9A6C_4f48_AEEA_7B747F16A4C6__INCLUDED_)
#define EA_F6D0FE68_9A6C_4f48_AEEA_7B747F16A4C6__INCLUDED_

#include <libxml/tree.h>

namespace xedit {
	/**
	 * xml序列化协议
	 */
	template <class ObjectType>
	class IXmlSerializable {
	public:
		virtual ~IXmlSerializable() {}

		/**
		 * 序列化对象
		 * @param object 待序列化对象
		 * @param nodeName 对象序列化后的节点名称
		 * @return 序列化后的数据
		 */
		static xmlNodePtr serialize(const ObjectType& object, const char* nodeName) {}
	};
}


#endif // !defined(EA_F6D0FE68_9A6C_4f48_AEEA_7B747F16A4C6__INCLUDED_)
