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
	int nRet = 0;	//�������Ϸ�������ת��ʧ��

	if (strFlag.IsEmpty() && L"-h" == StrToLower(vecStrIS[0].Left(2)) && L"-v" == StrToLower(vecStrIS[1].Left(2)))
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

// �ж� -xxx �Ƿ�Ϸ�
// xxx �����ǣ�Win8xPreset��Win10Preset��UserPreset1 - UserPreset100 ֮һ
int isLegal(CString str)
{
	//�ж�ǰ��ĺ�� -x
	if (str.GetLength() < 1 || L'-' != str[0]) {
		return 0;
	}

	int iRet;
	str = str.Right(str.GetLength() - 1);	//ȥ��ǰ��ĺ�� -
	str = StrToLower(str);

	if (L"win7preset" == str) {
		iRet = 101;
	}
	else if (L"win8xpreset" == str) {
		iRet = 102;
	}
	else if(L"win10preset" == str) {
		iRet = 103;
	}
	else if (L"userpreset" == str.Left(wcslen(L"userpreset"))) {
		iRet = _wtoi(str.Right(str.GetLength() - wcslen(L"userpreset")));
		if (iRet <= 0 || iRet > 100) {
			iRet = 0;
		}
	}
	else {
		iRet = 0;
	}

	return iRet;
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

	CString strErroe = L"  �����Ĳ������Ϸ����û��ɰ����¸�ʽ�Ĳ��������������в��������ִ�Сд��\r\n\
\r\n\
  A�����Բ�������ֱ���������ڽ��棻\r\n\
  B����ָ�� -? �����鿴������Ϣ�����磺AgileFontSet -? ��\r\n\
  C����ָ������ini�����ļ���·������������ʾini�����ļ��е�Ĭ�����ã�ע�⣺���ո��·��������˫���Ű�Χ��\
���磺AgileFontSet \"D:\\Program Files\\User Data\\myFontSet.ini\" ��\r\n\
  D����ָ������ini�����ļ���·���� -xxx ������������ѡ��ini�ļ��е�xxx���ã�\
xxx�����ǣ�Win7Preset��Win8xPreset��Win10Preset��UserPreset1 - UserPreset100 ֮һ��\
���磺AgileFontSet D:\\myFontSet.ini -UserPreset1 ��\r\n\
  E����ָ������ini�����ļ���·���� -hide ���������򽫺�̨����ini�����ļ��е�Ĭ�����ã�\
���磺AgileFontSet D:\\myFontSet.ini -hide ��\r\n\
  F����ָ������ini�����ļ���·���� -xxx -hide ���������򽫺�̨����ini�ļ��е�xxx���ã�\
���磺AgileFontSet D:\\myFontSet.ini -Win10Preset -hide ��\r\n\
";

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
			if (vecStrCmd[0].IsEmpty()) {
				if(L'\"' == str[0]){
					str.Replace(L"\"", L"");		//��ȷ������ɾ��str�е�˫����
					CTrimQ(str);
				}
				if (L':' != str[1]) {	//�ж�Path��2���ַ���L':'����Ǿ���·��������m_strCurrentDir
					str = getCurDir(2) + str;
				}
				vecStrCmd[0] = str;
			}
			else { iSeg = -1; break; }	//ͬ��������������
		}
		else if (L"-hide" == StrToLower(str)) {	//��������L"-hide"����ʱ��ֻ��ǰ��һ������
			if (vecStrCmd[2].IsEmpty()) vecStrCmd[2] = str;
			else { iSeg = -1; break; }	//ͬ��������������
		}
		else if (L"-?" == str) {	//��������L"-?"����ʱ��ֻ��ǰ��һ������
			if (vecStrCmd[3].IsEmpty()) vecStrCmd[3] = str;
			else { iSeg = -1; break; }	//ͬ��������������
		}
		else if (isLegal(str) > 0) {	//��������L"-xxx"����ʱ��ֻ��ǰ��һ������
			// xxx �����ǣ�Win8xPreset��Win10Preset��UserPreset1 - UserPreset100 ֮һ
			if (vecStrCmd[1].IsEmpty()) vecStrCmd[1] = str;
			else { iSeg = -1; break; }	//ͬ��������������
		}
		else {		//���ֲ���������ļ��ֺϷ�����������������forѭ��
			iSeg = -1; break;
		}
	}

	//����������������������͹��������׳���
	if (-1 == iSeg) {	//�������Ϸ�
		MessageBox(NULL, strErroe, L"�������Ϸ�", MB_OK | MB_ICONINFORMATION);
	}
	else if (0 == iSeg) {	// ����Ϊ�գ���ʾ���öԻ���Ͱ�����Ϣ(��ǰѡ�)
		nRet = progsheet.DoModal();
	}
	else if (1 == iSeg && L"-?" == vecStrCmd[3]) {	//�����Է��ں��洦����ǰ�������Ч��
		// ����Ϊһ����Ϊ L"-?" ����ʾ���öԻ���(��ǰѡ�)�Ͱ�����Ϣ
		progsheet.SetActivePage(1);	//�������Ա�����ʱ�ĵ�ǰѡ�
		nRet = progsheet.DoModal();
	}
	// ������path������Ϊ1��path������2��path -hide������3��path -xxx -hide
	else if ((1 == iSeg || 2 == iSeg || 3 == iSeg) && !vecStrCmd[0].IsEmpty()) {
		// 1��������������ļ����� path
		//��_waccess(�����io.h)����fopen�ж��ļ��Ƿ���ڣ���fopen���ļ����ɶ�������
		if (-1 == _waccess(vecStrCmd[0], 0)) {	//�ļ�����_waccess����0�����򷵻�-1
			::MessageBox(NULL, L"�����ļ� " + vecStrCmd[0] + L" �ļ������ڡ�", L"�����ļ�������", MB_OK | MB_ICONINFORMATION);
			return false;
		}

		if (FALSE == progsheet.m_pp1FontSet.loadFontInfo(vecStrCmd[0], 0)) {
			::MessageBox(NULL, L"�޷����������ļ� " + vecStrCmd[0], L"�����ļ��޷�����", MB_OK | MB_ICONEXCLAMATION);
			return false;
		}

		//�������Ϊ1��path
		if (1 == iSeg) {
			nRet = progsheet.DoModal();
		}
		//�������Ϊ2��path -hide
		else if (2 == iSeg && (L"-hide" == StrToLower(vecStrCmd[2]))) {
			progsheet.m_pp1FontSet.m_iCheckAllfont = 0;
			progsheet.m_pp1FontSet.m_iCheckTitle = 1;
			progsheet.m_pp1FontSet.m_iCheckIcon = 1;
			progsheet.m_pp1FontSet.m_iCheckMenu = 1;
			progsheet.m_pp1FontSet.m_iCheckMessage = 1;
			progsheet.m_pp1FontSet.m_iCheckPalette = 1;
			progsheet.m_pp1FontSet.m_iCheckTip = 1;

			progsheet.m_pp1FontSet.OnSet(0, 0, NULL, nCmdShow);
		}
		// ������-xxx������Ϊ2��path -xxx������3��path -xxx -hide
		else if ((2 == iSeg || 3 == iSeg) && !vecStrCmd[1].IsEmpty()) {
			// 2.������������ò��� -xxx
			// 2.1�����ָ������ xxx �Ƿ����
			if (!isSectionExists(vecStrCmd[1], vecStrCmd[0])) {
				::MessageBox(NULL, vecStrCmd[0] + L" �ļ��в��������ã�" + vecStrCmd[1], L"���ò�����", MB_OK | MB_ICONEXCLAMATION);
				return false;
			}
			else {
				// 2.2������ָ������ xxx
				int i = isLegal(vecStrCmd[1]);
				if (101 == i) {	//L"Win7Preset"
					progsheet.m_pp1FontSet.mySetFont(progsheet.m_pp1FontSet.m_metrics, progsheet.m_pp1FontSet.m_iconFont, progsheet.m_pp1FontSet.m_tagSetWin7);
					progsheet.m_pp1FontSet.m_nComboInitSel = 2;
				}
				else if (102 == i) {	//L"Win8xPreset"
					progsheet.m_pp1FontSet.mySetFont(progsheet.m_pp1FontSet.m_metrics, progsheet.m_pp1FontSet.m_iconFont, progsheet.m_pp1FontSet.m_tagSetWin8);
					progsheet.m_pp1FontSet.m_nComboInitSel = 3;
				}
				else if (103 == i) {	//L"Win10Preset"
					progsheet.m_pp1FontSet.mySetFont(progsheet.m_pp1FontSet.m_metrics, progsheet.m_pp1FontSet.m_iconFont, progsheet.m_pp1FontSet.m_tagSetWin10);
					progsheet.m_pp1FontSet.m_nComboInitSel = 4;
				}
				else if (i > 0 && i <= 100) {
					progsheet.m_pp1FontSet.mySetFont(progsheet.m_pp1FontSet.m_metrics, progsheet.m_pp1FontSet.m_iconFont, progsheet.m_pp1FontSet.m_vecTagSetUser[i]);
					progsheet.m_pp1FontSet.m_nComboInitSel = 4 + i;
				}
				
				// 3�������̨���ò��� -hide
				if (2 == iSeg) {
					nRet = progsheet.DoModal();

				}
				else if (3 == iSeg && L"-hide" == StrToLower(vecStrCmd[2])) {
					progsheet.m_pp1FontSet.m_iCheckAllfont = 0;
					progsheet.m_pp1FontSet.m_iCheckTitle = 1;
					progsheet.m_pp1FontSet.m_iCheckIcon = 1;
					progsheet.m_pp1FontSet.m_iCheckMenu = 1;
					progsheet.m_pp1FontSet.m_iCheckMessage = 1;
					progsheet.m_pp1FontSet.m_iCheckPalette = 1;
					progsheet.m_pp1FontSet.m_iCheckTip = 1;

					progsheet.m_pp1FontSet.OnSet(0, 0xFFFF, NULL, nCmdShow);
				}
			}
		}
		else {	//�������Ϸ�
			MessageBox(NULL, strErroe, L"�������Ϸ�", MB_OK | MB_ICONINFORMATION);
			return false;
		}
	}
	else {	//��������ʧ�ܺ��������������ʾ���öԻ���(��ǰѡ�)�Ͱ�����Ϣ
		progsheet.SetActivePage(1);	//�������Ա�����ʱ��ǰѡ�
		nRet = progsheet.DoModal();
	}

	return nRet;
}
