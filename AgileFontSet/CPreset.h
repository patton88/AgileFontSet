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
	wchar_t const* m0_strFace;	//L"FACE"
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
			this->tagIS = tag.tagIS;
		}
		return *this;
	}

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


	//map<RCN2, map<RCN1, tagFontInfo>> mapRCN2;
	//map<CString, pair<CString, tagFontInfo>> mapRCNb;

	//map<RCN1, tagFontInfo>> mapRCN;
	map<CString, tagFontInfo> mapRCN;

	//生成读写ini文件时的键值名
	CPreset(CString str) : strRCN3(str)	{

		for(unsigned j = 0; j < vecRCN2.size(); j++) {
			for (unsigned i = 0; i < vecRCN1.size(); i++) {
				vecRCN.push_back(vecRCN1[i] + L"_" + vecRCN1[j] + L"_" + strRCN3);
				if (0 == j) { mapRCN[vecRCN1[i]] = tagFontInfo(); }	//创建mapRCN元素
			}
		}
		vecRCN.push_back(vecIS[0] + L"_" + strRCN3);
		vecRCN.push_back(vecIS[1] + L"_" + strRCN3);

		//mapRCN[vecRCN1[0] + L"_" + vecRCN2[0] + L"_" + strRCN3] = &metrics.lfCaptionFont.lfFaceName;

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
	}

	const CString strRCN3;	//RCN3：Resource Center Name part 3
	vector<CString> vecRCN;
	//map<CString, LOGFONTW*> mapRCN;
 	NONCLIENTMETRICSW metrics;
	LOGFONTW iconFont, theIS;

	//为便于循环处理，用tagLOGFONTW的lfHeight存放nHS、用lfWidth存放nVS
	//typedef struct tagLOGFONTW
	//{
	//	LONG      lfHeight;
	//	LONG      lfWidth;

	TagIS tagIS;				//存放图标间距





};
