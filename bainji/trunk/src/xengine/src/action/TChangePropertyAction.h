//
// Created by wendachuan on 2019/1/2.
//

#ifndef PROJECT_TCHANGEPROPERTYACTION_H
#define PROJECT_TCHANGEPROPERTYACTION_H

#include "BaseAction.h"
#include <map>
#include <string>
#include <stdarg.h>
#include "avpub/Rational.h"
using namespace std;
using namespace libav;

namespace xedit {

    /**
     * 改变媒体偏移量
     */
    template <typename ObjectType, typename PropertyType>
    class TChangePropertyAction: public CBaseAction {

        /**
         * 获取对象属性函数模板指针定义
         */
        typedef const PropertyType& (*GetPropertyFunc)(const ObjectType*);

        /**
         * 设置对象属性函数模板指针定义
         */
        typedef StatusCode (*SetPropertyFunc)(ObjectType*, const PropertyType&, void*);

    public:
        /**
         * 构造函数
         * @param pTimeLine 时间线
         * @param actionType 动作类型
         * @param pObject 对象
         * @param pPriv 私有数据
         * @param getPropertyFunc 获取属性接口
         * @param setPropertyFunc 设置属性接口
         * @param newPropertyValue 新属性值
         * @param ... 可变参数，只接受指针
         */
        TChangePropertyAction(CTimeLine *pTimeLine,
                              EActionType actionType,
                              ObjectType *pObject,
                              void* pPriv,
                              GetPropertyFunc getPropertyFunc,
                              SetPropertyFunc setPropertyFunc,
                              const PropertyType& newPropertyValue): CBaseAction(pTimeLine) {
            m_eActionType = actionType;
            m_pObject = pObject;
            m_pPriv = pPriv;
            m_getPropertyFunc = getPropertyFunc;
            m_setPropertyFunc = setPropertyFunc;
            m_newPropertyValue = newPropertyValue;
        }

        virtual ~TChangePropertyAction() {
            m_pObject = NULL;
            m_getPropertyFunc = NULL;
            m_setPropertyFunc = NULL;
        }

        /**
         * 获取动作类型
         * @return
         */
        virtual EActionType getActionType() const {
            return m_eActionType;
        }

        /**
         * 执行动作
         * @return
         */
        virtual StatusCode excute() {
            m_oldPropertyValue = m_getPropertyFunc(m_pObject);
            return m_setPropertyFunc(m_pObject, m_newPropertyValue, m_pPriv);
        }

        /**
		 * 取消执行
		 */
        virtual void cancel() {

        }

        /**
         * 回退
         * @return
         */
        virtual StatusCode unDo() {
            return m_setPropertyFunc(m_pObject, m_oldPropertyValue, m_pPriv);
        }

        /**
         * 添加参数
         * @tparam T
         * @param key
         * @param value
         */
        template <typename T>
        void addParam(const char* key, T value) {
            CBaseAction::addParam(key, value);
        }

    private:
        EActionType     m_eActionType;
        ObjectType*     m_pObject;
        void*           m_pPriv;
        PropertyType    m_oldPropertyValue;
        PropertyType    m_newPropertyValue;

        GetPropertyFunc m_getPropertyFunc;
        SetPropertyFunc m_setPropertyFunc;
    };
}


#endif //PROJECT_TCHANGEPROPERTYACTION_H
