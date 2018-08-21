// AgileFontSet.h

#include "stdafx.h"
//#include "CommonFun.h"		//PP1_FontSet.cpp���Ѿ������������ٰ��������ظ�������ֻ����extern�ⲿ����
#include "PP0_PropertySheet.h"

//CString������(LPCWSTR)����ҪReleaseBuffer()������(LPTSTR)��(wchar_t*)��(char*)�����ҪReleaseBuffer()
//����Ϊһ�����ñ��ϰ�ߣ��Ժ󷲱�����CString-GetBuffer��LPTSTR��LPSTR����LPWSTR�ĵط������������ReleaseBuffer()
extern CString g_strVerInfo;
extern CString StrToLower(const CString& str);
extern int CStringSplitN(vector<CString>& vecResult, const CString& str, CString strSep);	//�����ֶ���

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
int GetDataEx(vector<CString> &vecStrIS, vector<int> &vecIntIS, CString strFlag)
{
	int nRet = 0;	//�����Ƿ�������ת��ʧ��

	if (L"" == strFlag && L"-h" == StrToLower(vecStrIS[0].Left(2)) && L"-v" == StrToLower(vecStrIS[1].Left(2)))
	{
		CString str0 = vecStrIS[0].Right(vecStrIS[0].GetLength() - 2);	//ȥ��ǰ�������ַ�
		vecIntIS[0] = myStoi(str0);
		//vecIconSpacing.push_back(_wtoi(str0.GetBuffer(str0.GetLength())));
		//vecIconSpacing[0] = _wtoi((LPWSTR)(LPCWSTR)vecCmdLine[0]);			//ת��ʧ��
		//str0.ReleaseBuffer();

		CString str1 = vecStrIS[1].Right(vecStrIS[1].GetLength() - 2);	//ȥ��ǰ�������ַ�
		vecIntIS[1] = myStoi(str1);
		//vecIconSpacing.push_back(_wtoi(str1.GetBuffer(str1.GetLength())));
		//vecIconSpacing[1] = _wtoi((LPWSTR)(LPCWSTR)vecCmdLine[1]);			//ת��ʧ��
		//vecCmdLine[1].ReleaseBuffer();

		// �� str ����ת���� int�����߽������ 0-150֮�䣬myStoi ������ -1
		if (-1 != vecIntIS[0] && -1 != vecIntIS[1])
		{
			nRet = 1;
		}	//ת���ɹ�
	}
	else if (L"-h" == strFlag && L"-h" == StrToLower(vecStrIS[0].Left(2)))
	{
		CString str0 = vecStrIS[0].Right(vecStrIS[0].GetLength() - 2);	//ȥ��ǰ�������ַ�
		vecIntIS[0] = myStoi(str0);
		// �� str ����ת���� int�����߽������ 0-150֮�䣬myStoi ������ -1
		if (-1 != vecIntIS[0])
		{
			nRet = 1;
		}	//ת���ɹ�
	}
	else if (L"-v" == strFlag && L"-v" == StrToLower(vecStrIS[1].Left(2)))
	{
		CString str1 = vecStrIS[1].Right(vecStrIS[1].GetLength() - 2);	//ȥ��ǰ�������ַ�
		vecIntIS[1] = myStoi(str1);
		// �� str ����ת���� int�����߽������ 0-150֮�䣬myStoi ������ -1
		if (-1 != vecIntIS[1])
		{
			nRet = 1;
		}	//ת���ɹ�
	}

	return nRet;
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


	// ���������в���
	vector<CString> vecCmdLine;
	vector<int> vecIntIS(2, -1);	//(vecIntIconSpacing)��ʼ����2��Ĭ��ֵΪ-1��Ԫ�ء��������ʱ������
	PP0_PropertySheet progsheet(g_strVerInfo.GetBuffer(0), 0);
	g_strVerInfo.ReleaseBuffer();

	//��CStringSplitS�ָ� v -h3 ʱ����������CStringSplitN
	iSeg = CStringSplitN(vecCmdLine, strCmdLine, ' ');	//strCmdLine�еĲ����οɲ����Ⱥ����

														//����������������������͹��������׳���
														// (vecStrIconSpacing)����ʼ��Ϊ5��L""��0-4��Ԫ�ֱ����ڴ�ţ�-Hxxx��-Vxxx��-N��-R��-?
	vector<CString> vecStrIS(5, L"");

	//����vecCmdLine�еĲ����Σ�
	//	ȡ��L"-n"����bRefresh��
	//	ȡ��L"-?"����bHelp��
	//	ȡ��L"-h"��L"-v"����vecIS[0]��vecIS[1]
	for (auto str : vecCmdLine)
	{
		//����������ɾ��vecCmdLine����Ԫ���еĿո��⽫���пո�ĵط�����0���жϺ�����ַ�
		//str.Replace(' ', NULL);
		//str.Replace(' ', '');	//��ʾ���󣺿��ַ�����
		str.Replace(L" ", L"");	//��ȷ������ɾ��vecCmdLine����Ԫ���еĿո�

								//C++ switch (x) �������ı��ʽx���������͡��ַ��͡�ö���͡�
								//WTL�� switch (x) ���ı��ʽx�����������ͻ�ö�����ͣ��ַ��ͱ���

								//switch (str.GetBuffer(0)) {default: break; }
								//iconspacing.h(210) : error C2450 : switch expression of type 'wchar_t *' is illegal
								//iconspacing.h(210) : note: Integral expression required

								//switch (str) {default: break; }
								//iconspacing.h(214) : error C2450 : switch expression of type 'ATL::CStringT<wchar_t,ATL::StrTraitATL<wchar_t,ATL::ChTraitsCRT<wchar_t>>>' is illegal
								//iconspacing.h(214) : note: No user - defined - conversion operator available that can perform this conversion, or the operator cannot be called

		if (L"-h" == StrToLower(str.Left(2)))
		{	//��������L"-h"����ʱ��ֻ��ǰ��һ������
			if (L"" == vecStrIS[0]) vecStrIS[0] = str;
			else { iSeg = -1; break; }	//ͬ��������������
		}
		else if (L"-v" == StrToLower(str.Left(2)))
		{	//��������L"-v"����ʱ��ֻ��ǰ��һ������
			if (L"" == vecStrIS[1]) vecStrIS[1] = str;
			else { iSeg = -1; break; }	//ͬ��������������
		}
		else if (L"-n" == StrToLower(str))
		{
			if (L"" == vecStrIS[2]) vecStrIS[2] = str;
			else { iSeg = -1; break; }	//ͬ��������������
		}
		else if (L"-r" == StrToLower(str))
		{
			if (L"" == vecStrIS[3]) vecStrIS[3] = str;
			else { iSeg = -1; break; }	//ͬ��������������
		}
		else if (L"-?" == StrToLower(str))
		{
			if (L"" == vecStrIS[4]) vecStrIS[4] = str;
			else { iSeg = -1; break; }	//ͬ��������������
		}
		else
		{
			iSeg = -1; break;
		}			//���ֲ����������5�ֺϷ�����������������forѭ��
	}

	// (vecStrIconSpacing)����ʼ��Ϊ5��L""��0-4��Ԫ�ֱ����ڴ�ţ�-Hxxx��-Vxxx��-N��-R��-?
	//vector<CString> vecStrIS(5, L"");

	//����������������������͹��������׳���
	if (-1 == iSeg)
	{	//�����Ƿ��������Խ�籨��
		MessageBox(NULL, L"  �����Ĳ������Ϸ�������ͼ���೬��0-150�ķ�Χ��\r\n\
  ͼ������0��150֮���3λ������(Win7��������λ)��\r\n\
  \r\n\
  ������ \"IconSpacing.exe -?\" ��ð�����Ϣ.\r\n",
			L"�����Ƿ���ͼ���೬����Χ", MB_OK | MB_ICONINFORMATION);
	}
	else if (0 == iSeg)
	{	// ����Ϊ�գ���ʾ���öԻ���Ͱ�����Ϣ(��ǰѡ�)
		nRet = progsheet.DoModal();
	}
	else if (1 == iSeg && L"-?" == vecStrIS[4])	//�����Է��ں��洦����ǰ�������Ч��
	{	// ����Ϊһ����Ϊ L"-?" ����ʾ���öԻ���(��ǰѡ�)�Ͱ�����Ϣ
		progsheet.SetActivePage(1);	//�������Ա�����ʱ�ĵ�ǰѡ�
		nRet = progsheet.DoModal();
	}
	else if (1 == iSeg || 2 == iSeg || 3 == iSeg)// ����Ϊ1��3��
	{
		int iState = 0;	//iState = 0 ��ʾ�����Ƿ���Խ��

		if (L"-r" == StrToLower(vecStrIS[3].Left(3)))
		{	//ֻ��L"-r"��ֻˢ������
			//bRefresh = TRUE;	//ǰ�涨��ʱ�Ѹ�Ĭ�ϳ�ֵTRUE
			if (1 == iSeg)		//L"-r"�����Ƕ�������
			{
				iState = progsheet.GetIconSpacing(vecIntIS);		//��ȡ��ǰͼ����
			}
		}
		else if (L"-h" == StrToLower(vecStrIS[0].Left(2)) && L"" == vecStrIS[1])
		{	//ֻ��L"-h"��[L"-n"]����ʱvecIntIS[0]��vecIntIS[1]��Ϊ-1
			progsheet.GetIconSpacing(vecIntIS);		//��ȡ��ǰͼ����
			iState = GetDataEx(vecStrIS, vecIntIS, L"-h");
		}
		else if (L"-v" == StrToLower(vecStrIS[1].Left(2)) && L"" == vecStrIS[0])
		{	//ֻ��L"-v"��[L"-n"]
			progsheet.GetIconSpacing(vecIntIS);		//��ȡ��ǰͼ����
			iState = GetDataEx(vecStrIS, vecIntIS, L"-v");
		}
		else if (L"-h" == StrToLower(vecStrIS[0].Left(2)) && L"-v" == StrToLower(vecStrIS[1].Left(2)))
		{	//ͬʱ��L"-h"��L"-v"��[L"-n"]
			iState = GetDataEx(vecStrIS, vecIntIS, L"");
		}

		if (1 == iState)
		{	//Ӧ�����ã�ˢ������
			progsheet.SetIconSpacing(vecIntIS[0], vecIntIS[1], L"-n" != StrToLower(vecStrIS[2].Left(2)));
		}
		else
		{	//�����Ƿ��������Խ�籨��
			MessageBox(NULL, L"  �����Ĳ������Ϸ�������ͼ���೬��0-150�ķ�Χ��\r\n\
  ͼ������0��150֮���3λ������(Win7��������λ)��\r\n\
  \r\n\
  ������ \"IconSpacing.exe -?\" ��ð�����Ϣ.\r\n",
				L"�����Ƿ���ͼ���೬����Χ", MB_OK | MB_ICONINFORMATION);
		}
	}
	else
	{	//��������ʧ�ܺ��������������ʾ���öԻ���(��ǰѡ�)�Ͱ�����Ϣ
		progsheet.SetActivePage(1);	//�������Ա�����ʱ��ǰѡ�
		nRet = progsheet.DoModal();
	}

	//		MessageBox(NULL, L"  IconSpacing is illegal or out of range.\r\n\
		  //IconSpacing is 3-digit integer between 15 and 150 (Windows 7 metric).\r\n\
  //\r\n\
  //Please run \"IconSpacing.exe -?\" to display help message.\r\n",
//			L"IconSpacing is illegal or out of range", MB_OK);

//switch (nRet)
//{
//case 2:		//�ɹ����ã�ֱ������
//	break;
//case 4:		//�����Ƿ��������Խ�籨��
//	MessageBox(NULL, L"  IconSpacing is illegal or out of range.\r\n\
 // IconSpacing is 3-digit integer between 15 and 150 (Windows 7 metric).\r\n\
 // \r\n\
 // Please run \"IconSpacing.exe -?\" to display help message.\r\n",
//		L"IconSpacing is illegal or out of range", MB_OK);
//	break;
//default:		//����ʧ�ܺ������������ʾHelp��Ϣ
//				//IconSpacing_Help iconSpacing_Help;
//				//iconSpacing_Help.CenterWindow();	//�ô�������ֹ�����ô�������CenterΪTrue���
//				//nRet = (int)iconSpacing_Help.DoModal();

//	PP0_PropertySheet progsheet(L"ChromePortable.ini ���ý��� - by ybmj@vip.163.com", 0);
//	int nRet = progsheet.DoModal();

//	break;
//}

	return nRet;
}
