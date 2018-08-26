// AgileFontSet.h

#include "stdafx.h"
//#include "CommonFun.h"		//PP1_FontSet.cpp���Ѿ������������ٰ��������ظ�������ֻ����extern�ⲿ����
#include "PP0_PropertySheet.h"

//CString������(LPCWSTR)����ҪReleaseBuffer()������(LPTSTR)��(wchar_t*)��(char*)�����ҪReleaseBuffer()
//����Ϊһ�����ñ��ϰ�ߣ��Ժ󷲱�����CString-GetBuffer��LPTSTR��LPSTR����LPWSTR�ĵط������������ReleaseBuffer()
extern CString g_strVerInfo;
extern CString StrToLower(const CString& str);
extern int CStringSplitN(vector<CString>& vecResult, const CString& str, CString strSep);	//�����ֶ���
extern CString getCurDir(int iFlag);
extern BOOL isEngChar(wchar_t w);
extern bool isSectionExists(CString sectionName, CString& iniFilePath);

bool SetIconSpacing0()
{
	//PP0_PropertySheet progsheet(L"ChromePortable.ini ���ý��� - by ybmj@vip.163.com", 0);
	PP0_PropertySheet progsheet(g_strVerInfo.GetBuffer(0), 0);
	g_strVerInfo.ReleaseBuffer();
	int nRet = progsheet.DoModal();

	return true;
}

//�ж��ַ����Ƿ�������
bool AllisNum(CString str)
{
	for (int i = 0; i < str.GetLength(); i++)
	{
		int tmp = (int)str[i];
		if (tmp >= 48 && tmp <= 57)
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	return true;
}

/*
ͷ�ļ���#include <stdlib.h>

atoi() �����������ַ���ת��������(int)����ԭ��Ϊ��
int atoi (const char * str);

������˵����atoi() ������ɨ����� str �ַ���������ǰ��Ŀհ��ַ�������ո�tab�����ȣ�
����ͨ�� isspace() ��������⣩��ֱ���������ֻ��������Ųſ�ʼ��ת����
�������������ֻ��ַ�������ʱ('\0')�Ž���ת��������������ء�
������ֵ������ת���������������� str ����ת���� int ���� str Ϊ���ַ�������ô������ 0��

��ܰ��ʾ��
ANSI C �淶������ stof()��atoi()��atol()��strtod()��strtol()��strtoul()
��6�����Խ��ַ���ת��Ϊ���ֵĺ�������ҿ��ԶԱ�ѧϰ��
������ C99 / C++11 �淶����������5��������
�ֱ��� atoll()��strtof()��strtold()��strtoll()��strtoull()���ڴ˲������ܣ���������ѧϰ��

���������ַ���a ���ַ���b ת�������ֺ���ӡ�
*/

//10��n�η�
int myPower10(int n)
{
	int power = 1;
	for (int i = 0; i < n; i++) { power *= 10; }
	return power;
}

//�ַ�����ת������
int myWCtoInt(wchar_t w)
{	// '0'Ϊ48
	w = w - 48;
	return w;
}

//�򵥵���λ0-150���ַ���ֵת��Ϊ��������
//�ɹ�����0-150����������ʧ�ܷ���-1
int myStoi(CString str)
{
	int iRet = -1;
	int volue = 0;
	int iLen = str.GetLength();
	if (iLen > 0 && iLen <= 3)
	{
		int i;
		for (i = 0; i < iLen; i++)
		{	// '0'Ϊ48��'9'Ϊ57
			if (str[i] < 48 || str[i] > 57) { break; }
			volue += myWCtoInt(str[i]) * myPower10(iLen - i - 1);
		}

		if (i == iLen && (volue >= 0 && volue <= 150)) { iRet = volue; }
	}

	return iRet;
}

//int GetData(vector<CString> &vecStrIS, vector<int> &vecIntIS)
//{
//	int nRet = 0;	//������Ϊ������ת��ʧ��
//
//	if (L"-h" == StrToLower(vecStrIS[0].Left(2)) && L"-v" == StrToLower(vecStrIS[1].Left(2)))
//	{
//		CString str0 = vecStrIS[0].Right(vecStrIS[0].GetLength() - 2);	//ȥ��ǰ�������ַ�
//		vecIntIS[0] = myStoi(str0);
//		//vecIconSpacing.push_back(_wtoi(str0.GetBuffer(str0.GetLength())));
//		//vecIconSpacing[0] = _wtoi((LPWSTR)(LPCWSTR)vecCmdLine[0]);			//ת��ʧ��
//		//str0.ReleaseBuffer();
//
//		CString str1 = vecStrIS[1].Right(vecStrIS[1].GetLength() - 2);	//ȥ��ǰ�������ַ�
//		vecIntIS[1] = myStoi(str1);
//		//vecIconSpacing.push_back(_wtoi(str1.GetBuffer(str1.GetLength())));
//		//vecIconSpacing[1] = _wtoi((LPWSTR)(LPCWSTR)vecCmdLine[1]);			//ת��ʧ��
//		//vecCmdLine[1].ReleaseBuffer();
//
//		// �� str ����ת���� int�����߽������ 0-150֮�䣬myStoi ������ -1
//		if (-1 != vecIntIS[0] && -1 != vecIntIS[1])
//		{
//			nRet = 1;
//		}	//ת���ɹ�
//	}
//
//	return nRet;
//}

//vecStrIS(vecStrIconSpacing)��vecIntIS(vecIntIconSpacing)
int GetDataEx(vector<CString> &vecStrIS, vector<unsigned> &vecUnIS, CString strFlag)
{
	int nRet = 0;	//�����Ƿ�������ת��ʧ��

	if (L"" == strFlag && L"-h" == StrToLower(vecStrIS[0].Left(2)) && L"-v" == StrToLower(vecStrIS[1].Left(2)))
	{
		CString str0 = vecStrIS[0].Right(vecStrIS[0].GetLength() - 2);	//ȥ��ǰ�������ַ�
		vecUnIS[0] = myStoi(str0);
		//vecIconSpacing.push_back(_wtoi(str0.GetBuffer(str0.GetLength())));
		//vecIconSpacing[0] = _wtoi((LPWSTR)(LPCWSTR)vecCmdLine[0]);			//ת��ʧ��
		//str0.ReleaseBuffer();

		CString str1 = vecStrIS[1].Right(vecStrIS[1].GetLength() - 2);	//ȥ��ǰ�������ַ�
		vecUnIS[1] = myStoi(str1);
		//vecIconSpacing.push_back(_wtoi(str1.GetBuffer(str1.GetLength())));
		//vecIconSpacing[1] = _wtoi((LPWSTR)(LPCWSTR)vecCmdLine[1]);			//ת��ʧ��
		//vecCmdLine[1].ReleaseBuffer();

		// �� str ����ת���� int�����߽������ 0-150֮�䣬myStoi ������ -1
		if (-1 != vecUnIS[0] && -1 != vecUnIS[1])
		{
			nRet = 1;
		}	//ת���ɹ�
	}
	else if (L"-h" == strFlag && L"-h" == StrToLower(vecStrIS[0].Left(2)))
	{
		CString str0 = vecStrIS[0].Right(vecStrIS[0].GetLength() - 2);	//ȥ��ǰ�������ַ�
		vecUnIS[0] = myStoi(str0);
		// �� str ����ת���� int�����߽������ 0-150֮�䣬myStoi ������ -1
		if (-1 != vecUnIS[0])
		{
			nRet = 1;
		}	//ת���ɹ�
	}
	else if (L"-v" == strFlag && L"-v" == StrToLower(vecStrIS[1].Left(2)))
	{
		CString str1 = vecStrIS[1].Right(vecStrIS[1].GetLength() - 2);	//ȥ��ǰ�������ַ�
		vecUnIS[1] = myStoi(str1);
		// �� str ����ת���� int�����߽������ 0-150֮�䣬myStoi ������ -1
		if (-1 != vecUnIS[1])
		{
			nRet = 1;
		}	//ת���ɹ�
	}

	return nRet;
}

int isLegal(CString str)
{
	if (L"userpreset" != str.Left(wcslen(L"userpreset")))
	{
		return 0;
	}
	int i = _wtoi(str.Right(str.GetLength() - wcslen(L"userpreset")));
	if(i <= 0 || i > 100)
	{
		return 0;
	}
	return i;
}

//int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
int APIENTRY VS2013_Win32App_wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	int nRet = 0;
	int iSeg = 0;	//�����в����ɹ��ָ�Ķ���
					// ��ȡ�����в���
	CString strCmdLine = lpCmdLine;
	// ɾ��ǰ��հ׷�
	strCmdLine = CTrimQ(strCmdLine);

	CString strErroe = L"  �����Ĳ������Ϸ���\r\n\
  \r\n\
  ���Բ�������ֱ���������ڽ���\r\n\
  �ɸ���ini�����ļ�·��(���ո��·��������˫���Ű�Χ)�����磺D:\\myFontSet.ini\r\n\
  �ɸ���ini�����ļ�·���� -xxx ����ѡ��ini�ļ��е�xxx���ã����磺D:\\myFontSet.ini -xxx\r\n\
  �ɸ���ini�����ļ�·���� -hide �������к�̨���ã����磺D:\\myFontSet.ini -hide\r\n\
  �ɸ���ini�����ļ�·���� -xxx -hide ������̨����ini�ļ��е�xxx���ã����磺D:\\myFontSet.ini -xxx -hide\r\n\
  �ɸ��� -? �����鿴������Ϣ�����磺AgileFontSet.exe -?\r\n\
  ���в��������ִ�Сд\r\n";

	// ���������в���
	vector<CString> vecCmdLine;
	PP0_PropertySheet progsheet(g_strVerInfo.GetBuffer(0), 0);
	g_strVerInfo.ReleaseBuffer();

	//��CStringSplitS�ָ� v -h3 ʱ����������CStringSplitN
	iSeg = CStringSplitN(vecCmdLine, strCmdLine, ' ');	//strCmdLine�еĲ����οɲ����Ⱥ����

	//����������������������͹��������׳���
	//��ʼ��Ϊ4��L""��0-3��Ԫ�ֱ����ڴ�ţ�path��-xxx��-hide��-?
	vector<CString> vecStrCmd(4, L"");

	//����vecCmdLine�еĲ����Σ�
	for (auto str : vecCmdLine)
	{
		CTrimQ(str);		//ȥ��str���οո�

		if (L".ini" == StrToLower(str.Right(4)) || L".ini\"" == StrToLower(str.Right(5)))
		{	//��������L".ini"����ʱ��ֻ��ǰ��һ������
			if (L"" == vecStrCmd[0]) {
				if(L'\"' == str[0]){
					str.Replace(L"\"", L"");		//��ȷ������ɾ��str�е�˫����
					CTrimQ(str);
				}
				if (L':' != str[1])		//�ж�Path��2���ַ���L':'����Ǿ���·��������m_strCurrentDir
				{
					str = getCurDir(2) + str;
				}
				vecStrCmd[0] = str;
			}
			else { iSeg = -1; break; }	//ͬ��������������
		}
		else if (L"-hide" == StrToLower(str))
		{	//��������L"-hide"����ʱ��ֻ��ǰ��һ������
			if (L"" == vecStrCmd[2]) vecStrCmd[2] = str;
			else { iSeg = -1; break; }	//ͬ��������������
		}
		else if (L"-?" == str)
		{	//��������L"-?"����ʱ��ֻ��ǰ��һ������
			if (L"" == vecStrCmd[3]) vecStrCmd[3] = str;
			else { iSeg = -1; break; }	//ͬ��������������
		}
		else if (str.GetLength() > 1 && L'-' == str[0] && isEngChar(str[1]))
		{	//��������L"-xxx"����ʱ��ֻ��ǰ��һ������
			if (L"" == vecStrCmd[1]) vecStrCmd[1] = str;
			else { iSeg = -1; break; }	//ͬ��������������
		}
		else
		{
			iSeg = -1; break;
		}	//���ֲ���������ļ��ֺϷ�����������������forѭ��
	}

	//����������������������͹��������׳���
	if (-1 == iSeg)
	{	//�����Ƿ�
		MessageBox(NULL, strErroe, L"�����Ƿ�", MB_OK | MB_ICONINFORMATION);
	}
	else if (0 == iSeg)
	{	// ����Ϊ�գ���ʾ���öԻ���Ͱ�����Ϣ(��ǰѡ�)
		nRet = progsheet.DoModal();
	}
	else if (1 == iSeg && L"-?" == vecStrCmd[3])	//�����Է��ں��洦����ǰ�������Ч��
	{	// ����Ϊһ����Ϊ L"-?" ����ʾ���öԻ���(��ǰѡ�)�Ͱ�����Ϣ
		progsheet.SetActivePage(1);	//�������Ա�����ʱ�ĵ�ǰѡ�
		nRet = progsheet.DoModal();
	}
	// ����Ϊ1��path������2��path -hide������3��path -xxx -hide
	else if ((1 == iSeg || 2 == iSeg || 3 == iSeg) && !vecStrCmd[0].IsEmpty())
	{
		int iState = 0;	//iState = 0 ��ʾ�����Ƿ���Խ��

		//��_waccess(�����io.h)����fopen�ж��ļ��Ƿ���ڣ���fopen���ļ����ɶ�������
		if (-1 == _waccess(vecStrCmd[0], 0))	//�ļ�����_waccess����0�����򷵻�-1
		{
			::MessageBox(NULL, L"�����ļ� " + vecStrCmd[0] + L" �ļ������ڡ�", L"�����ļ�������", MB_OK | MB_ICONINFORMATION);
			return false;
		}

		if (FALSE == progsheet.m_pp1FontSet.loadFontInfo(vecStrCmd[0])) {
			::MessageBox(NULL, L"�޷��������������ļ� " + vecStrCmd[0], L"����", MB_OK | MB_ICONEXCLAMATION);
			return false;
		}

		if (1 == iSeg)
		{
			nRet = progsheet.DoModal();
		}
		else if (2 == iSeg && (L"-hide" == StrToLower(vecStrCmd[2])))
		{
			progsheet.m_pp1FontSet.m_iCheckAllfont = 0;
			progsheet.m_pp1FontSet.m_iCheckTitle = 1;
			progsheet.m_pp1FontSet.m_iCheckIcon = 1;
			progsheet.m_pp1FontSet.m_iCheckMenu = 1;
			progsheet.m_pp1FontSet.m_iCheckMessage = 1;
			progsheet.m_pp1FontSet.m_iCheckPalette = 1;
			progsheet.m_pp1FontSet.m_iCheckTip = 1;

			progsheet.m_pp1FontSet.OnSet(0, 0, NULL, nCmdShow);
		}
		else if (3 == iSeg && !vecStrCmd[1].IsEmpty() && L"-hide" == StrToLower(vecStrCmd[2])) // ����Ϊ3��path -xxx -hide
		{
			if (isSectionExists(vecStrCmd[1], vecStrCmd[0]))
			{
				::MessageBox(NULL, vecStrCmd[0] + L"�ļ��в��������ã�" + vecStrCmd[1], L"����", MB_OK | MB_ICONEXCLAMATION);
			}
			else
			{
				int i = isLegal(vecStrCmd[1]);
				if (L"Win8xPreset" == vecStrCmd[1])
				{
					progsheet.m_pp1FontSet.mySetFont(progsheet.m_pp1FontSet.m_metrics, progsheet.m_pp1FontSet.m_iconFont, progsheet.m_pp1FontSet.m_tagSetWin8);
				}
				else if (L"Win10Preset" == vecStrCmd[1])
				{
					progsheet.m_pp1FontSet.mySetFont(progsheet.m_pp1FontSet.m_metrics, progsheet.m_pp1FontSet.m_iconFont, progsheet.m_pp1FontSet.m_tagSetWin10);
				}
				else if (i > 0)
				{
					progsheet.m_pp1FontSet.mySetFont(progsheet.m_pp1FontSet.m_metrics, progsheet.m_pp1FontSet.m_iconFont, progsheet.m_pp1FontSet.m_vecTagSetUser[i]);
				}

				progsheet.m_pp1FontSet.m_iCheckAllfont = 0;
				progsheet.m_pp1FontSet.m_iCheckTitle = 1;
				progsheet.m_pp1FontSet.m_iCheckIcon = 1;
				progsheet.m_pp1FontSet.m_iCheckMenu = 1;
				progsheet.m_pp1FontSet.m_iCheckMessage = 1;
				progsheet.m_pp1FontSet.m_iCheckPalette = 1;
				progsheet.m_pp1FontSet.m_iCheckTip = 1;

				progsheet.m_pp1FontSet.OnSet(0, 0, NULL, nCmdShow);
			}
		}
	}
	else
	{	//��������ʧ�ܺ��������������ʾ���öԻ���(��ǰѡ�)�Ͱ�����Ϣ
		progsheet.SetActivePage(1);	//�������Ա�����ʱ��ǰѡ�
		nRet = progsheet.DoModal();
	}

	return nRet;
}
