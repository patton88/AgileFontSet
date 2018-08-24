//CPreset.h
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include <vector>
#include <map>
using std::vector; using std::map;

extern HWND hwndPP1;

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

//数组指针的定义
//int(*PArrayType)[5];
//int c[5] = { 1,2,3,4,5 };
////PArrayType pArray;
//PArrayType = &c;

struct tagFontInfo
{
	//wchar_t* m0_strFace;	//L"FACE"。这样定义有问题
	wchar_t (*m0_strFace) [32];	//L"FACE"。数组指针的定义
	LONG* m1_lHeight;		//L"SIZE"，在显示到CEdit、存入文件时，需将m1_lHeight转换为Size
	BYTE* m2_bCharset;		//L"CHARSET"
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

	vector<CString> vecWin8xPreSet{
		"Segoe UI",
		"Segoe UI",
		"Segoe UI",
		"Segoe UI",
		"Segoe UI",
		"Segoe UI",
		"11",
		"9",
		"9",
		"9",
		"11",
		"9",
		"1",
		"1",
		"1",
		"1",
		"1",
		"1",
		"77",
		"47"
	};

	vector<CString> vecWin10PreSet{
		"Segoe UI",
		"Segoe UI",
		"Segoe UI",
		"Segoe UI",
		"Segoe UI",
		"Segoe UI",
		"9",
		"9",
		"9",
		"9",
		"9",
		"9",
		"1",
		"1",
		"1",
		"1",
		"1",
		"1",
		"75",
		"45"
	};

	//用于循环处理的关键数据结构mapRCN
	map<CString, tagFontInfo> mapRCN;

	//生成读写ini文件时的键值名
	CPreset(CString str) : strRCN3(str)	{
		//生成一种配置所包含的字符标识容器数组vecRCN
		for(unsigned j = 0; j < vecRCN2.size(); j++) {
			for (unsigned i = 0; i < vecRCN1.size(); i++) {
				vecRCN.push_back(vecRCN1[i] + L"_" + vecRCN2[j] + L"_" + strRCN3);
				if (0 == j) { mapRCN[vecRCN1[i]] = tagFontInfo(); }	//创建mapRCN元素
			}
		}
		vecRCN.push_back(vecIS[0] + L"_" + strRCN3);
		vecRCN.push_back(vecIS[1] + L"_" + strRCN3);

		//为已创建的mapRCN元素赋值
		//error C2440: '=': cannot convert from 'WCHAR (*)[32]' to 'wchar_t *'
		//WCHAR(*p)[32];
		//p = &metrics.lfCaptionFont.lfFaceName;

		//m_vecTagSetUser[i].initmapRCN();		//CPreset对象必须在创建后进行初始化，否则地址不对
		//估计此时，变量的内存还未最终分配确定，所以此时取变量地址赋值不对。此时初始化地址不对
		initmapRCN();
	}

	// 将菜单字体的信息应用于其他字体的信息。
	void initmapRCN()
	{
		FillMemory(&metrics, sizeof(NONCLIENTMETRICSW), 0x00);
		FillMemory(&metricsAll, sizeof(NONCLIENTMETRICSW), 0x00);
		FillMemory(&iconFont, sizeof(LOGFONTW), 0x00);
		FillMemory(&iconFontAll, sizeof(LOGFONTW), 0x00);
		tagIS.nHS = tagIS.nVS = -1;		//未存入配置的标志

		mapRCN[vecRCN1[0]].m0_strFace = &metrics.lfCaptionFont.lfFaceName;
		mapRCN[vecRCN1[0]].m1_lHeight = &metrics.lfCaptionFont.lfHeight;
		mapRCN[vecRCN1[0]].m2_bCharset = &metrics.lfCaptionFont.lfCharSet;

		mapRCN[vecRCN1[1]].m0_strFace = &iconFont.lfFaceName;
		mapRCN[vecRCN1[1]].m1_lHeight = &iconFont.lfHeight;
		mapRCN[vecRCN1[1]].m2_bCharset = &iconFont.lfCharSet;

		mapRCN[vecRCN1[2]].m0_strFace = &metrics.lfMenuFont.lfFaceName;
		mapRCN[vecRCN1[2]].m1_lHeight = &metrics.lfMenuFont.lfHeight;
		mapRCN[vecRCN1[2]].m2_bCharset = &metrics.lfMenuFont.lfCharSet;

		mapRCN[vecRCN1[3]].m0_strFace = &metrics.lfMessageFont.lfFaceName;
		mapRCN[vecRCN1[3]].m1_lHeight = &metrics.lfMessageFont.lfHeight;
		mapRCN[vecRCN1[3]].m2_bCharset = &metrics.lfMessageFont.lfCharSet;

		mapRCN[vecRCN1[4]].m0_strFace = &metrics.lfSmCaptionFont.lfFaceName;
		mapRCN[vecRCN1[4]].m1_lHeight = &metrics.lfSmCaptionFont.lfHeight;
		mapRCN[vecRCN1[4]].m2_bCharset = &metrics.lfSmCaptionFont.lfCharSet;

		mapRCN[vecRCN1[5]].m0_strFace = &metrics.lfStatusFont.lfFaceName;
		mapRCN[vecRCN1[5]].m1_lHeight = &metrics.lfStatusFont.lfHeight;
		mapRCN[vecRCN1[5]].m2_bCharset = &metrics.lfStatusFont.lfCharSet;
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

	int getDPI(void)
	{
		int dpiY = 96;
		CClientDC dc(hwndPP1);

		if (dc) { dpiY = GetDeviceCaps(dc, LOGPIXELSY); }
		return dpiY;
	}

	//从字体高度获取字体大小。
	int getFontSize(LONG lFontHight)
	{
		//获得字体大小。从字体高度H到字体大小S需要进行换算，
		//H = -(S * dpiY / 72)
		//S = -(H * 72 / dpiY)
		//MulDiv(a, b, c) 就是计算 a * b / c，结果四舍五入
		//m_metrics.lfCaptionFont.lfHeight = -MulDiv(fontSizes8[0], dpiY, 72);

		int dpiY = getDPI();		//获取DPI。
		int iSzie = -(lFontHight * 72 / dpiY);
		return iSzie;
	}

	//从字体大小获取字体高度。
	LONG getFontHight(int lFontSize)
	{
		//获得字体大小。从字体高度H到字体大小S需要进行换算，
		//H = -(S * dpiY / 72)
		//S = -(H * 72 / dpiY)
		//MulDiv(a, b, c) 就是计算 a * b / c，结果四舍五入
		//m_metrics.lfCaptionFont.lfHeight = -MulDiv(fontSizes8[0], dpiY, 72);

		int dpiY = getDPI();		//获取DPI。
		int lFontHight = -(lFontSize * dpiY / 72);
		return lFontHight;
	}

	// 生成预设配置数据结构
	int getPreset(vector<CString>& vecDataX)
	{
		//生成一种配置所包含的字符标识容器数组mapRCNx
		map<CString, CString> mapRCNx;
		for (unsigned i = 0; i < vecDataX.size(); i++) {
			mapRCNx[vecRCN[i]] = vecDataX[i];
		}

		// 读取字体容器循环赋值
		CString str;
		int iRet;
		for (auto& rcn2 : vecRCN2) {
			for (auto& rcn1 : vecRCN1) {
				str = rcn1 + L"_" + rcn2 + L"_" + strRCN3;
				if (rcn2 == vecRCN2[0]) {		// 字体名称容器赋值
					wcscpy_s(*mapRCN[rcn1].m0_strFace, min(mapRCNx[str].GetLength() + 1, 32), mapRCNx[str].GetBuffer(0));
				}
				else if (rcn2 == vecRCN2[1]) {	// 字体大小循环赋值
					iRet = _wtoi(mapRCNx[str]);
					*mapRCN[rcn1].m1_lHeight = getFontHight(iRet);	//将获得的iSize转换为字体高返回
				}
				else if (rcn2 == vecRCN2[2]) {	// 字符集循环赋值
					iRet = _wtoi(mapRCNx[str]);
					*mapRCN[rcn1].m2_bCharset = iRet;
				}
			}
		}

		//保存所有字体信息
		SetAllFont();

		// 读取图标间距。读取不成功，使用默认值
		tagIS.nHS = iRet = _wtoi(mapRCNx[vecIS[0]]);
		if (0 == iRet) { tagIS.nHS = 80; }

		tagIS.nVS = iRet = _wtoi(mapRCNx[vecIS[1]]);
		if (0 == iRet) { tagIS.nVS = 48; }

		return 1;
	}
};
