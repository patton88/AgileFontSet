//CPreset.h
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include <vector>
using std::vector;

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

	//生成读写ini文件时的键值名
	CPreset(CString str) : strRCN3(str)	{
		for (auto& rcn2 : vecRCN2) {
			for (auto& rcn1 : vecRCN1) {
				vecRCN.push_back(rcn1 + L"_" + rcn2 + L"_" + strRCN3);
			}
		}
		vecRCN.push_back(vecIS[0] + L"_" + strRCN3);
		vecRCN.push_back(vecIS[1] + L"_" + strRCN3);
	}

	const CString strRCN3;	//RCN3：Resource Center Name part 3
	vector<CString> vecRCN;
	NONCLIENTMETRICSW metrics;
	LOGFONTW iconFont;
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
};
