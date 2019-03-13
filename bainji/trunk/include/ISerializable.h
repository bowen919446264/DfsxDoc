///////////////////////////////////////////////////////////
//  ISerializableT,U.h
//  Implementation of the Interface ISerializable<T,U>
//  Created on:      21-06-2018 10:16:36
//  Original author: wendachuan
///////////////////////////////////////////////////////////

#if !defined(EA_200F81E7_E7B9_421d_8EB3_61EC86BB61FB__INCLUDED_)
#define EA_200F81E7_E7B9_421d_8EB3_61EC86BB61FB__INCLUDED_

namespace xedit {
	/**
	 * 序列化接口
	 */
	template <class DataType>
	class ISerializable {
	public:
		virtual ~ISerializable() {}

		/**
		 * 序列化对象
		 * @return 序列化后的数据
		 */
		virtual DataType serialize() =0;
	};
}

#endif // !defined(EA_200F81E7_E7B9_421d_8EB3_61EC86BB61FB__INCLUDED_)
