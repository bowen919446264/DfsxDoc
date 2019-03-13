///////////////////////////////////////////////////////////
//  IObservable.h
//  Implementation of the Interface IObservable
//  Created on:      21-06-2018 10:16:36
//  Original author: wendachuan
///////////////////////////////////////////////////////////

#if !defined(EA_83EB15C6_4CCA_4627_82FA_5006ABCA5803__INCLUDED_)
#define EA_83EB15C6_4CCA_4627_82FA_5006ABCA5803__INCLUDED_

namespace xedit {
	/**
	 * 被观察者接口
	 */
	class IObservable
	{

	public:
		virtual ~IObservable() {}

		/**
         * 添加一个观察者
         * @param pObserver
         */
		virtual void addObserver(IObserver* pObserver) =0;

		/**
         * 删除指定观察者
         * @param pObserver
         */
		virtual void deleteObserver(IObserver* pObserver) =0;

		/**
         * 删除所有的观察者
         */
		virtual void deleteAllObservers() =0;

		/**
         * 获得观察者数量
         * @return
         */
		virtual int getObserverCount() =0;

		/**
         * 获得指定序号的观察者
         * @param index
         * @return
         */
		virtual IObserver* getObserver(int index) =0;

		/**
         * 唤醒所有的观察者
         */
		virtual void notifyAllObservers() =0;
	};
}

#endif // !defined(EA_83EB15C6_4CCA_4627_82FA_5006ABCA5803__INCLUDED_)
