///////////////////////////////////////////////////////////
//  IDeserializableT,U.h
//  Implementation of the Interface IDeserializable<T,U>
//  Created on:      21-06-2018 10:16:36
//  Original author: wendachuan
///////////////////////////////////////////////////////////

#if !defined(EA_E9BBEB67_88E8_4854_9782_21DCCDF281DE__INCLUDED_)
#define EA_E9BBEB67_88E8_4854_9782_21DCCDF281DE__INCLUDED_

namespace xedit {
	/**
	 * 反序列化接口
	 */
    template <class ObjectType, class DataType>
	class IDeserializable {
	public:
		virtual ~IDeserializable() {}

		/**
         * 反序列化
         * @param data 待反序列化的数据
         * @return 反序列化后的对象
         */
        static ObjectType deserialize(const DataType& data) {}
	};
}

#endif // !defined(EA_E9BBEB67_88E8_4854_9782_21DCCDF281DE__INCLUDED_)
