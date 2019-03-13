///////////////////////////////////////////////////////////
//  IXmlDeserializableT.h
//  Implementation of the Interface IXmlDeserializable<T>
//  Created on:      21-06-2018 10:16:36
//  Original author: wendachuan
///////////////////////////////////////////////////////////

#if !defined(EA_41C4C1EC_C85D_47fb_A8BD_386E46B31A2B__INCLUDED_)
#define EA_41C4C1EC_C85D_47fb_A8BD_386E46B31A2B__INCLUDED_

#include "IDeserializable.h"
#include <libxml/tree.h>

namespace xedit {
	/**
	 * XML反序列化接口
	 */
	template <class ObjectType>
	class IXmlDeserializable: public IDeserializable<ObjectType, xmlNodePtr> {
	public:
		virtual ~IXmlDeserializable() {}
	};
}

#endif // !defined(EA_41C4C1EC_C85D_47fb_A8BD_386E46B31A2B__INCLUDED_)
