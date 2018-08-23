//CPreset.h
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include <vector>
#include <map>
using std::vector; using std::map;

struct TagIS			//tag of Icon Spacing
{
	TagIS& operator =(const TagIS& tag)		//重载赋值运算符
	{
		if (this != &tag)
		{
			this->nHS = tag.nHS;
			this->nVS = tag.nVS;
		}
		return *this;
	}

	unsigned nHS;	//ICON_HORIZONTAL_SPACING
	unsigned nVS;	//ICON_VERTICAL_SPACING
};

struct tagFontInfo
{
	wchar_t* m0_strFace;	//L"FACE"
	LONG* m1_lHeight;			//L"SIZE"，在显示到CEdit、存入文件时，需将m1_lHeight转换为Size
	BYTE* m2_bCharset;			//L"CHARSET"
};

class CPreset		//class Preset
{
public:
	CPreset& operator =(const CPreset& tag)	//重载赋值运算符
	{
		if (this != &tag)
		{
			this->metrics = tag.metrics;
			this->iconFont = tag.iconFont;
			this->metricsAll = tag.metricsAll;
			this->iconFontAll = tag.iconFontAll;
			this->tagIS = tag.tagIS;
		}
		return *this;
	}

	const CString strRCN3;	//RCN3：Resource Center Name part 3
	vector<CString> vecRCN;

	//赋值操作时，需要复制以下成员
	NONCLIENTMETRICSW metrics;
	LOGFONTW iconFont;

	NONCLIENTMETRICSW metricsAll;
	LOGFONTW iconFontAll;

	TagIS tagIS;				//存放图标间距

	//C++11初始化列表。RCN1：Resource Center Name part 1
	vector<CString> vecRCN1{
		L"CAPTION",
		L"ICON",
		L"MENU",
		L"MESSAGE",
		L"SMALLCAPTION",
		L"STATUS"
	};

	//C++11初始化列表。RCN2：Resource Center Name part 2
	vector<CString> vecRCN2{
		L"FACE",
		L"SIZE",
		L"CHARSET"
	};

	//C++11初始化列表。IS：Icon Spacing
	vector<CString> vecIS{
		L"ICON_HORIZONTAL_SPACING",
		L"ICON_VERTICAL_SPACING"
	};

	//用于循环处理的关键数据结构mapRCN
	map<CString, tagFontInfo> mapRCN;

	//生成读写ini文件时的键值名
	CPreset(CString str) : strRCN3(str)	{
		//生成一种配置所包含的字符标识容器数组vecRCN
		for(unsigned j = 0; j < vecRCN2.size(); j++) {
			for (unsigned i = 0; i < vecRCN1.size(); i++) {
				vecRCN.push_back(vecRCN1[i] + L"_" + vecRCN1[j] + L"_" + strRCN3);
				if (0 == j) { mapRCN[vecRCN1[i]] = tagFontInfo(); }	//创建mapRCN元素
			}
		}
		vecRCN.push_back(vecIS[0] + L"_" + strRCN3);
		vecRCN.push_back(vecIS[1] + L"_" + strRCN3);

		//为已创建的mapRCN元素赋值
		mapRCN[vecRCN1[0]].m0_strFace = metrics.lfCaptionFont.lfFaceName;
		mapRCN[vecRCN1[0]].m1_lHeight = &metrics.lfCaptionFont.lfHeight;
		mapRCN[vecRCN1[0]].m2_bCharset = &metrics.lfCaptionFont.lfCharSet;

		mapRCN[vecRCN1[1]].m0_strFace = iconFont.lfFaceName;
		mapRCN[vecRCN1[1]].m1_lHeight = &iconFont.lfHeight;
		mapRCN[vecRCN1[1]].m2_bCharset = &iconFont.lfCharSet;

		mapRCN[vecRCN1[2]].m0_strFace = metrics.lfMenuFont.lfFaceName;
		mapRCN[vecRCN1[2]].m1_lHeight = &metrics.lfMenuFont.lfHeight;
		mapRCN[vecRCN1[2]].m2_bCharset = &metrics.lfMenuFont.lfCharSet;

		mapRCN[vecRCN1[3]].m0_strFace = metrics.lfMessageFont.lfFaceName;
		mapRCN[vecRCN1[3]].m1_lHeight = &metrics.lfMessageFont.lfHeight;
		mapRCN[vecRCN1[3]].m2_bCharset = &metrics.lfMessageFont.lfCharSet;

		mapRCN[vecRCN1[4]].m0_strFace = metrics.lfSmCaptionFont.lfFaceName;
		mapRCN[vecRCN1[4]].m1_lHeight = &metrics.lfSmCaptionFont.lfHeight;
		mapRCN[vecRCN1[4]].m2_bCharset = &metrics.lfSmCaptionFont.lfCharSet;

		mapRCN[vecRCN1[5]].m0_strFace = metrics.lfStatusFont.lfFaceName;
		mapRCN[vecRCN1[5]].m1_lHeight = &metrics.lfStatusFont.lfHeight;
		mapRCN[vecRCN1[5]].m2_bCharset = &metrics.lfStatusFont.lfCharSet;

		tagIS.nHS = tagIS.nVS = -1;		//未存入配置的标志
	}

	// 将菜单字体的信息应用于其他字体的信息。
	void SetAllFont()
	{
		metricsAll = metrics;
		iconFontAll = iconFont;
		metricsAll.lfCaptionFont = metricsAll.lfMenuFont;
		iconFontAll = metricsAll.lfMenuFont;
		metricsAll.lfMessageFont = metricsAll.lfMenuFont;
		metricsAll.lfSmCaptionFont = metricsAll.lfMenuFont;
		metricsAll.lfStatusFont = metricsAll.lfMenuFont;
	}
};
