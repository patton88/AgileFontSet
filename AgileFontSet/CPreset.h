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
	TagIS& operator =(const TagIS& tag)		//���ظ�ֵ�����
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

//����ָ��Ķ���
//int(*PArrayType)[5];
//int c[5] = { 1,2,3,4,5 };
////PArrayType pArray;
//PArrayType = &c;

struct tagFontInfo
{
	//wchar_t* m0_strFace;	//L"FACE"����������������
	wchar_t (*m0_strFace) [32];	//L"FACE"������ָ��Ķ���
	LONG* m1_lHeight;		//L"SIZE"������ʾ��CEdit�������ļ�ʱ���轫m1_lHeightת��ΪSize
	BYTE* m2_bCharset;		//L"CHARSET"
};

class CPreset		//class Preset
{
public:
	CPreset& operator =(const CPreset& tag)	//���ظ�ֵ�����
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

	const CString strRCN3;	//RCN3��Resource Center Name part 3
	vector<CString> vecRCN;

	//��ֵ����ʱ����Ҫ�������³�Ա
	NONCLIENTMETRICSW metrics;
	LOGFONTW iconFont;

	NONCLIENTMETRICSW metricsAll;
	LOGFONTW iconFontAll;

	TagIS tagIS;				//���ͼ����

	//C++11��ʼ���б�RCN1��Resource Center Name part 1
	vector<CString> vecRCN1{
		L"CAPTION",
		L"ICON",
		L"MENU",
		L"MESSAGE",
		L"SMALLCAPTION",
		L"STATUS"
	};

	//C++11��ʼ���б�RCN2��Resource Center Name part 2
	vector<CString> vecRCN2{
		L"FACE",
		L"SIZE",
		L"CHARSET"
	};

	//C++11��ʼ���б�IS��Icon Spacing
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

	//����ѭ������Ĺؼ����ݽṹmapRCN
	map<CString, tagFontInfo> mapRCN;

	//���ɶ�дini�ļ�ʱ�ļ�ֵ��
	CPreset(CString str) : strRCN3(str)	{
		//����һ���������������ַ���ʶ��������vecRCN
		for(unsigned j = 0; j < vecRCN2.size(); j++) {
			for (unsigned i = 0; i < vecRCN1.size(); i++) {
				vecRCN.push_back(vecRCN1[i] + L"_" + vecRCN2[j] + L"_" + strRCN3);
				if (0 == j) { mapRCN[vecRCN1[i]] = tagFontInfo(); }	//����mapRCNԪ��
			}
		}
		vecRCN.push_back(vecIS[0] + L"_" + strRCN3);
		vecRCN.push_back(vecIS[1] + L"_" + strRCN3);

		//Ϊ�Ѵ�����mapRCNԪ�ظ�ֵ
		//error C2440: '=': cannot convert from 'WCHAR (*)[32]' to 'wchar_t *'
		//WCHAR(*p)[32];
		//p = &metrics.lfCaptionFont.lfFaceName;

		//m_vecTagSetUser[i].initmapRCN();		//CPreset��������ڴ�������г�ʼ���������ַ����
		//���ƴ�ʱ���������ڴ滹δ���շ���ȷ�������Դ�ʱȡ������ַ��ֵ���ԡ���ʱ��ʼ����ַ����
		initmapRCN();
	}

	// ���˵��������ϢӦ���������������Ϣ��
	void initmapRCN()
	{
		FillMemory(&metrics, sizeof(NONCLIENTMETRICSW), 0x00);
		FillMemory(&metricsAll, sizeof(NONCLIENTMETRICSW), 0x00);
		FillMemory(&iconFont, sizeof(LOGFONTW), 0x00);
		FillMemory(&iconFontAll, sizeof(LOGFONTW), 0x00);
		tagIS.nHS = tagIS.nVS = -1;		//δ�������õı�־

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

	// ���˵��������ϢӦ���������������Ϣ��
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

	//������߶Ȼ�ȡ�����С��
	int getFontSize(LONG lFontHight)
	{
		//��������С��������߶�H�������СS��Ҫ���л��㣬
		//H = -(S * dpiY / 72)
		//S = -(H * 72 / dpiY)
		//MulDiv(a, b, c) ���Ǽ��� a * b / c�������������
		//m_metrics.lfCaptionFont.lfHeight = -MulDiv(fontSizes8[0], dpiY, 72);

		int dpiY = getDPI();		//��ȡDPI��
		int iSzie = -(lFontHight * 72 / dpiY);
		return iSzie;
	}

	//�������С��ȡ����߶ȡ�
	LONG getFontHight(int lFontSize)
	{
		//��������С��������߶�H�������СS��Ҫ���л��㣬
		//H = -(S * dpiY / 72)
		//S = -(H * 72 / dpiY)
		//MulDiv(a, b, c) ���Ǽ��� a * b / c�������������
		//m_metrics.lfCaptionFont.lfHeight = -MulDiv(fontSizes8[0], dpiY, 72);

		int dpiY = getDPI();		//��ȡDPI��
		int lFontHight = -(lFontSize * dpiY / 72);
		return lFontHight;
	}

	// ����Ԥ���������ݽṹ
	int getPreset(vector<CString>& vecDataX)
	{
		//����һ���������������ַ���ʶ��������mapRCNx
		map<CString, CString> mapRCNx;
		for (unsigned i = 0; i < vecDataX.size(); i++) {
			mapRCNx[vecRCN[i]] = vecDataX[i];
		}

		// ��ȡ��������ѭ����ֵ
		CString str;
		int iRet;
		for (auto& rcn2 : vecRCN2) {
			for (auto& rcn1 : vecRCN1) {
				str = rcn1 + L"_" + rcn2 + L"_" + strRCN3;
				if (rcn2 == vecRCN2[0]) {		// ��������������ֵ
					wcscpy_s(*mapRCN[rcn1].m0_strFace, min(mapRCNx[str].GetLength() + 1, 32), mapRCNx[str].GetBuffer(0));
				}
				else if (rcn2 == vecRCN2[1]) {	// �����Сѭ����ֵ
					iRet = _wtoi(mapRCNx[str]);
					*mapRCN[rcn1].m1_lHeight = getFontHight(iRet);	//����õ�iSizeת��Ϊ����߷���
				}
				else if (rcn2 == vecRCN2[2]) {	// �ַ���ѭ����ֵ
					iRet = _wtoi(mapRCNx[str]);
					*mapRCN[rcn1].m2_bCharset = iRet;
				}
			}
		}

		//��������������Ϣ
		SetAllFont();

		// ��ȡͼ���ࡣ��ȡ���ɹ���ʹ��Ĭ��ֵ
		tagIS.nHS = iRet = _wtoi(mapRCNx[vecIS[0]]);
		if (0 == iRet) { tagIS.nHS = 80; }

		tagIS.nVS = iRet = _wtoi(mapRCNx[vecIS[1]]);
		if (0 == iRet) { tagIS.nVS = 48; }

		return 1;
	}
};
