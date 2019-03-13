#pragma  once
//#include "DsNleDef.h"
//#include "DsUtil.h"
//using namespace DSNleLib;
#include <string>
#include <vector>
//using namespace std;
namespace PlayEngine
{
#define  MAX_GROUP_TITLE 50
#ifndef MAX_PATH
#define MAX_PATH 256
#endif // !MAX_PATH


	enum EDS_DATA_TYPE
	{
		eDs_EU_FLOAT = 0,
		eDs_EU_INTEGAR,
		eDs_EU_COLORREF,
		eDs_EU_TEXT,
		eDs_EU_IMAGE,
		eDs_EU_CHECKBOX,
		eDs_EU_WNDHOOK_CHECKBOX,
		eDs_EU_HSVCOLOR,
		eDs_EU_COMBOX,
		eDs_EU_CONTROLPOINT,
		eDs_EU_GROUP_ITEM,
		eDs_EU_CHILD_GROUP_CONTAINER,
		eDs_EU_YUVCOLOR = 0x8000
	};

	enum EDS_CONTROL_TYPE
	{
		eDs_CONTROL_INVALID = -1,
		eDs_CHROMAKEY = 0,
		eDs_LUMAKEY,
		eDs_WITH_LOCK
	};

	enum EDS_GROUP_TYPE
	{
		eDs_NONE_TRANSLATOR_GROUP = 0,
		eDs_TRANSLATOR_GROUP,
		eDsNONE_TRANSLATOR_WITH_LOCK_GROUP
	};

	enum EDS_TRANSLATOR_TYPE
	{
		eDs_TRANSLATOR_INVALID = -1,
		eDs_TRANSLATOR_TRANSFORM_PAN = 0,  // 平移
		eDs_TRANSLATOR_TRANSFORM_ROTATE,   // 旋转
		eDs_TRANSLATOR_TRANSFORM_SCALE,    // 缩放
		eDs_TRANSLATOR_TRANSFORM_CROP,     // 裁剪
		eDs_TRANSLATOR_MASK_PAN,		   // 蒙板平移
		eDs_TRANSLATOR_MASK_ROTATE,		   // 蒙板旋转
		eDs_TRANSLATOR_MASK_SCALE,		   // 蒙板缩放
		eDs_TRANSLATOR_MASK_CROP,		   // 蒙板裁剪
		eDs_TRANSLATOR_TRANSFORM_CENTER,   // 中心
		eDs_TRANSLATOR_MASK_CENTER,		   // 蒙板中心
		eDs_TRANSLATOR_MASK_SETTING		   // 蒙板的 “设置”分组
	};

	// 两个特技关键帧之间的特技如何计算：默认为线性计算
	enum EDS_FX_CALCULATE_TYPE
	{
		eDs_FX_CALCULATE_LINE  = 1,      // 线性计算
		eDs_FX_CALCULATE_HOLD  = 2,     // 保持不变
		eDs_FX_CALCULATE_CURVE = 4      // 曲线计算
	};

	enum EDS_FX_GROUP_RESTRICT_TYPE
	{
		eDs_FX_GROUP_NONAL = 0,
		eDs_FX_GROUP_SUPPORT_CONTROLPOINT = 1  // 支持控制点
	};

	enum  EDS_FX_TYPE
	{
		eDs_FX_TYPE_INVALID = 0,
		eDs_FX_TYPE_NORMAL = 1,
		eDs_FX_TYPE_TRANSLATION = 2,
	};

	struct SFxBasicItem
	{
		virtual ~SFxBasicItem()
		{
		}
	};

	struct SDsBoolItem : public SFxBasicItem
	{
		BOOL m_bCheckStatus ;
	};

	struct SDsTextItem : public SFxBasicItem
	{
		SDsTextItem()
		{
			memset(m_textName, 0, sizeof(TCHAR) * MAX_PATH);
		}
		TCHAR m_textName[MAX_PATH];
	};

	struct SDsFloatItem : public SFxBasicItem
	{
		void operator= (const SDsFloatItem& item)
		{
			m_fValue = item.m_fValue;
			m_fMinValue = item.m_fMinValue;
			m_fMaxValue = item.m_fMaxValue;
		}
		float m_fValue;
		float m_fMinValue;
		float m_fMaxValue;
	};

	struct SDsColorItem : public SFxBasicItem
	{
		BOOL     m_bUsingAlpha;
		DWORD m_color;
	};

	struct SDsImageItem : public SFxBasicItem
	{
		SDsImageItem()
		{
			m_bAlphaImage = 0;
			memset(m_wszWorkFolder, 0,sizeof(TCHAR)*MAX_PATH);
			memset(m_wszImageFile, 0,sizeof(TCHAR)*MAX_PATH);

		}
		BOOL    m_bAlphaImage;
		TCHAR m_wszWorkFolder[MAX_PATH];
		TCHAR m_wszImageFile[MAX_PATH];
	};

#define  MAX_OF_ITEM_NAME 50
#define  MAX_COMBOX_ITEM_COUNT 15

	struct SDsComboBoxItem : public SFxBasicItem
	{
		std::vector<std::string> m_ListStrings;
		int			  m_ListCount;
		int			  m_CurrentIndex;
	};

	struct SDsControlPoint : public SFxBasicItem
	{
		float m_fXPosition;
		float m_fYPosition;
	};

	struct SDsBasicItemInfo;
	typedef std::vector<SDsBasicItemInfo> SDsBasicItemList;
	struct SDsFxGroupSettings : public SFxBasicItem
	{
		EDS_GROUP_TYPE m_eGroupType;
		EDS_TRANSLATOR_TYPE m_eTraslatorType;
		EDS_FX_CALCULATE_TYPE m_eFxCalculateType; // 特技计算
		EDS_FX_GROUP_RESTRICT_TYPE m_eFxGroupRestrictType;
		TCHAR m_swzGroupName[MAX_GROUP_TITLE];
		SDsBasicItemList m_BasicItemInfoList;

		SDsFxGroupSettings()
		{
			m_eGroupType = eDs_NONE_TRANSLATOR_GROUP;
			m_eTraslatorType = eDs_TRANSLATOR_INVALID;
			m_eFxCalculateType = eDs_FX_CALCULATE_LINE;
			m_eFxGroupRestrictType = eDs_FX_GROUP_NONAL;
			memset(m_swzGroupName, 0,sizeof(TCHAR) * MAX_GROUP_TITLE);
		}
	};
	typedef std::vector<SDsFxGroupSettings> SDsFxGroupList;

	struct SDsChildGroupItemContainer: public SFxBasicItem
	{
		SDsFxGroupList m_ChildGroupList;
	};

	struct SDsBasicItemInfo
	{
		SDsBasicItemInfo(EDS_DATA_TYPE eDataType)
		: m_eParameterType(eDataType)
		, m_eControlType(eDs_CONTROL_INVALID)
		, m_pBasicInfo(NULL)
		{
			InitItem(eDataType);
			memset(m_swzTitle, 0,sizeof(TCHAR) * MAX_GROUP_TITLE);
		}

		virtual ~SDsBasicItemInfo()
		{
			if (m_pBasicInfo != NULL)
			{
				delete(m_pBasicInfo);
				m_pBasicInfo = NULL;
			}
		}

		// 拷贝构造函数
		SDsBasicItemInfo(const SDsBasicItemInfo &item)
		{
			InitItem(item.m_eParameterType);
			m_eParameterType = item.m_eParameterType;
			*this = item;
		}

		SDsBasicItemInfo & operator= (const SDsBasicItemInfo& item)
		{
			//assert(m_ePrameterType == item.m_ePrameterType);
			// 类型不同，需要与目标类型相同 
			if (m_eParameterType != item.m_eParameterType)
			{
				SAFE_DELETE(m_pBasicInfo);
				InitItem(item.m_eParameterType);
			}
			m_eParameterType = item.m_eParameterType;
			m_eControlType = item.m_eControlType;
			_tcscpy(m_swzTitle, item.m_swzTitle);
			if (m_pBasicInfo && item.m_pBasicInfo)
			{
				assert(m_pBasicInfo != item.m_pBasicInfo);
				if (m_eParameterType == eDs_EU_FLOAT ||
					m_eParameterType == eDs_EU_INTEGAR)
				{
					*((SDsFloatItem*)m_pBasicInfo) = *((SDsFloatItem *)item.m_pBasicInfo);
				}
				else if (m_eParameterType == eDs_EU_COLORREF)
				{
					*((SDsColorItem*)m_pBasicInfo) = *((SDsColorItem *)item.m_pBasicInfo);
				}
				else if (m_eParameterType == eDs_EU_IMAGE)
				{
					*((SDsImageItem*)m_pBasicInfo) = *((SDsImageItem *)item.m_pBasicInfo);
				}
				else if (m_eParameterType == eDs_EU_CHECKBOX)
				{
					*((SDsBoolItem*)m_pBasicInfo) = *((SDsBoolItem *)item.m_pBasicInfo);
				}
				else if (m_eParameterType == eDs_EU_WNDHOOK_CHECKBOX)
				{
					*((SDsBoolItem*)m_pBasicInfo) = *((SDsBoolItem *)item.m_pBasicInfo);
				}
				else if (m_eParameterType == eDs_EU_COMBOX)
				{
					*((SDsComboBoxItem*)m_pBasicInfo) = *((SDsComboBoxItem *)item.m_pBasicInfo);
				}
				else if (m_eParameterType == eDs_EU_CONTROLPOINT)
				{
					*((SDsControlPoint*)m_pBasicInfo) = *((SDsControlPoint *)item.m_pBasicInfo);
				}
				else if (m_eParameterType == eDs_EU_GROUP_ITEM)
				{
					*((SDsFxGroupSettings *)m_pBasicInfo) = *((SDsFxGroupSettings *)item.m_pBasicInfo);
				}
				else if (m_eParameterType == eDs_EU_CHILD_GROUP_CONTAINER)
				{
					*((SDsChildGroupItemContainer *)m_pBasicInfo) = *((SDsChildGroupItemContainer *)item.m_pBasicInfo);
				}
				else if (m_eParameterType == eDs_EU_TEXT)
				{
					*((SDsTextItem *)m_pBasicInfo) = *((SDsTextItem *)item.m_pBasicInfo);
				}
				else if (m_eParameterType == eDs_EU_HSVCOLOR)
				{
				}
				else if (m_eParameterType == eDs_EU_YUVCOLOR)
				{
					*((SDsColorItem*)m_pBasicInfo) = *((SDsColorItem *)item.m_pBasicInfo);
				}
				else 
				{
					assert(FALSE);
				}
			}

			return *this;
		}

		TCHAR m_swzTitle[MAX_GROUP_TITLE];
		EDS_DATA_TYPE m_eParameterType;
		EDS_CONTROL_TYPE m_eControlType;
		SFxBasicItem  *m_pBasicInfo;

	private:
		void InitItem(EDS_DATA_TYPE eDataType)
		{
			m_pBasicInfo = NULL;
			if (eDataType == eDs_EU_FLOAT ||
				eDataType == eDs_EU_INTEGAR)
			{
				m_pBasicInfo = new SDsFloatItem();
			}
			else if (eDataType == eDs_EU_COLORREF)
			{
				m_pBasicInfo = new SDsColorItem;
			}
			else if (eDataType == eDs_EU_IMAGE)
			{
				m_pBasicInfo = new SDsImageItem;
			}
			else if (eDataType == eDs_EU_CHECKBOX)
			{
				m_pBasicInfo = new SDsBoolItem;
			}
			else if (eDataType == eDs_EU_WNDHOOK_CHECKBOX)
			{
				m_pBasicInfo = new SDsBoolItem;
			}
			
			else if (eDataType == eDs_EU_COMBOX)
			{
				m_pBasicInfo = new SDsComboBoxItem;
			}
			else if (eDataType == eDs_EU_CONTROLPOINT)
			{
				m_pBasicInfo = new SDsControlPoint;
			}
			else if (eDataType == eDs_EU_GROUP_ITEM)
			{
				m_pBasicInfo = new SDsFxGroupSettings;
			}
			else if (eDataType == eDs_EU_CHILD_GROUP_CONTAINER)
			{
				m_pBasicInfo = new SDsChildGroupItemContainer;
			}
			else if (eDataType == eDs_EU_TEXT)
			{
				m_pBasicInfo = new SDsTextItem;
			}
			else if (eDataType == eDs_EU_HSVCOLOR)
			{
			}
			else if (eDataType == eDs_EU_YUVCOLOR)
			{
				m_pBasicInfo = new SDsColorItem;
			}
			else
			{
				assert(FALSE);
			}
		}
		
	};

	// 关键帧组配置
	struct SDsKeyFrameFxGroupSettings
	{
		float m_fPercentOfDuration;        // 关键帧位置
		SDsFxGroupSettings m_sFxGroupItem;   // 组参数
	};

	typedef std::vector<SDsKeyFrameFxGroupSettings> SDsKeyFrameGroupSettingsList;  // 分组的关键帧列表

	typedef std::vector<SDsKeyFrameGroupSettingsList> SDsListOfKeyFrameGroupSettingsList;  // 分组列表

	struct SDsFxSettings
	{
		// 有多个设置分组
		// 每个分组可以有多个关键帧配置
		SDsListOfKeyFrameGroupSettingsList m_GroupList;  
	};

	// 一帧对应的特技配置
	struct SDsSingleKeyFrameFxSettings
	{
		SDsFxGroupList m_GroupItemList;
	};


	// 过渡特技方向
	enum EDsTransitionDirection
	{
		eDsTrackAToTrackB,
		eDsTrackBToTrackA
	};

	// 过渡特技的控制关键帧
	struct SDsTransitionFxControlKeyFrame
	{
		float m_fKFPosition;    // 关键帧位置
		float m_fFxValuePosition;	     // 特技位置
	};

	typedef std::vector<SDsTransitionFxControlKeyFrame> SDsTransitionFxControlKFList; //过渡特技的控制关键帧列表


	struct SDsFxInfo
	{
		TCHAR m_wszFxName[50];
		GUID  m_fxGuid;
		EDS_FX_TYPE  m_eSupportFxTypes; // 支持过渡特技
	};

	typedef std::vector<SDsFxInfo> SDsFxInfoList;
}