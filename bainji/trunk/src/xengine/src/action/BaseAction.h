//
// Created by wendachuan on 2018/11/22.
//

#ifndef PROJECT_BASEACTION_H
#define PROJECT_BASEACTION_H

#include "IInnerAction.h"
#include <vector>
#include <map>
#include <string>
#include <inttypes.h>
#include <type_traits>
#include "avpub/Rational.h"
using namespace std;
using namespace libav;

namespace xedit {

    /**
     * 将指定类型的值转化为字符串
     * @tparam T
     * @param value
     * @param pOutStr
     * @return
     */
    template <typename T>
    bool toString(const T& value, string *pOutStr) {
        if (std::is_same<T, int>::value) {
            int nValue = *((int *)(const void*)&value);
            char tmp[32] = {0};
            sprintf(tmp, "%" PRId32, nValue);
            *pOutStr = tmp;
            return true;
        } else if (std::is_same<T, ID>::value) {
            ID nValue = *((ID *)(const void*)&value);
            char tmp[32] = {0};
            sprintf(tmp, "%" PRId64, nValue);
            *pOutStr = tmp;
            return true;
        } else if (std::is_same<T, Rational>::value) {
            const Rational& r = *((const Rational*)(const void*)&value);
            char tmp[64] = {0};
            sprintf(tmp, "%" PRId64 "/%" PRId64, r.nNum, r.nDen);
            *pOutStr = tmp;
            return true;
        } else if (std::is_same<T, char*>::value) {
            const char* str = *((const char**)(const void*)&value);
            *pOutStr = str;
            return true;
        } else if (std::is_same<T, string>::value) {
            string str = *((const string*)(const void*)&value);
            *pOutStr = str;
            return true;
        } else {
            return false;
        }
    }

    class CTimeLine;

    /**
     * 动作基类
     */
    class CBaseAction: public IInnerAction {
    protected:
        CBaseAction(CTimeLine *pTimeLine);
        virtual ~CBaseAction();

    public:

        /**
         * 获取动作参数
         * @param key 参数key, AP_*
         * @return
         */
        virtual const char* getActionParam(const char* key) const;

        /**
         * 获取动作编号
         * @return
         */
        virtual ID getId();

        /**
		 * 动作是否可以取消
		 * @return
		 */
        virtual bool canCancel() const;

        /**
		 * 动作是否已取消
		 * @return
		 */
        virtual bool isCanceled() const;

    protected:

        /**
         * 添加参数
         * @tparam T
         * @param key
         * @param value
         */
        template <typename T>
        void addParam(const char* key, T value) {
            string tmp;
            assert(toString(value, &tmp));
            m_mapParams[key] = tmp;
        }

    protected:
        CTimeLine*          m_pTimeLine;
        ID                  m_id;
        map<string, string> m_mapParams;
    };

}

#endif //PROJECT_BASEACTION_H
