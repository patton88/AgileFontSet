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
  ���Բ�������ֱ���������ڽ��棻\r\n\
  �ɸ���ini�����ļ�·��(���ո��·��������˫���Ű�Χ)�����磺D:\\mySet.ini\r\n\
  �ɸ���ini�����ļ�·���� -hide �������к�̨���ã����磺D:\\myFontSet.ini -hide\r\n\
  �ɸ��� -? �����鿴������Ϣ�����磺AgileFontSet.exe -?\r\n\
  ���в��������ִ�Сд\r\n";

	// ���������в���
	vector<CString> vecCmdLine;
	PP0_PropertySheet progsheet(g_strVerInfo.GetBuffer(0), 0);
	g_strVerInfo.ReleaseBuffer();

	//��CStringSplitS�ָ� v -h3 ʱ����������CStringSplitN
	iSeg = CStringSplitN(vecCmdLine, strCmdLine, ' ');	//strCmdLine�еĲ����οɲ����Ⱥ����

	//����������������������͹��������׳���
	//��ʼ��Ϊ3��L""��0-1��Ԫ�ֱ����ڴ�ţ�-hide��path��-?
	vector<CString> vecStrIS(3, L"");

	//����vecCmdLine�еĲ����Σ�
	for (auto str : vecCmdLine)
	{
		CTrimQ(str);		//ȥ��str���οո�

		if (L".ini" == StrToLower(str.Right(4)) || L".ini\"" == StrToLower(str.Right(5)))
		{	//��������L".ini"����ʱ��ֻ��ǰ��һ������
			if (L"" == vecStrIS[0]) {
				if(L'\"' == str[0]){
					str.Replace(L"\"", L"");		//��ȷ������ɾ��str�е�˫����
					CTrimQ(str);
				}
				vecStrIS[0] = str;
			}
			else { iSeg = -1; break; }	//ͬ��������������
		}
		else if (L"-hide" == StrToLower(str))
		{	//��������L"-hide"����ʱ��ֻ��ǰ��һ������
			if (L"" == vecStrIS[1]) vecStrIS[1] = str;
			else { iSeg = -1; break; }	//ͬ��������������
		}
		else if (L"-?" == str)
		{
			if (L"" == vecStrIS[2]) vecStrIS[2] = str;
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
	else if (1 == iSeg && L"-?" == vecStrIS[2])	//�����Է��ں��洦����ǰ�������Ч��
	{	// ����Ϊһ����Ϊ L"-?" ����ʾ���öԻ���(��ǰѡ�)�Ͱ�����Ϣ
		progsheet.SetActivePage(1);	//�������Ա�����ʱ�ĵ�ǰѡ�
		nRet = progsheet.DoModal();
	}
	else if (1 == iSeg || 2 == iSeg)// ����Ϊ1��path������2��path -hide
	{
		int iState = 0;	//iState = 0 ��ʾ�����Ƿ���Խ��

		// 7of9. ���� chrome.exe �ļ����ڵļ�⣬��������ʾ����ǰ�˳�
		if (L':' != vecStrIS[0][1])		//�ж�Path��2���ַ���L':'����Ǿ���·��������m_strCurrentDir
			vecStrIS[0] = getCurDir(2) + vecStrIS[0];

		//��_waccess(�����io.h)����fopen�ж��ļ��Ƿ���ڣ���fopen���ļ����ɶ�������
		if (-1 == _waccess(vecStrIS[0], 0))	//�ļ�����_waccess����0�����򷵻�-1
		{
			::MessageBox(NULL, vecStrIS[0] + L" �ļ������ڡ�", L"�ļ�������", MB_OK | MB_ICONINFORMATION);
			return false;
		}

		if (1 == iSeg)
		{
			if (FALSE == progsheet.m_pp1FontSet.loadFontInfo(vecStrIS[0])) {
				::MessageBox(NULL, L"�޷�������������", L"����", MB_OK | MB_ICONEXCLAMATION);
			}
			nRet = progsheet.DoModal();
		}

		if (2 == iSeg && L"-hide" == StrToLower(vecStrIS[1]))
		{
			if (FALSE == progsheet.m_pp1FontSet.loadFontInfo(vecStrIS[0])) {
				::MessageBox(NULL, L"�޷�������������", L"����", MB_OK | MB_ICONEXCLAMATION);
			}
			else
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
		}
	}
	else
	{	//��������ʧ�ܺ��������������ʾ���öԻ���(��ǰѡ�)�Ͱ�����Ϣ
		progsheet.SetActivePage(1);	//�������Ա�����ʱ��ǰѡ�
		nRet = progsheet.DoModal();
	}

	return nRet;
}
