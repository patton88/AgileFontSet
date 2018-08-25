// CommonFun.h��ͨ�ú���ͷ�ļ�
#pragma once

// Ϊ�˲�����windows.h�е�һЩ�����ݣ���������һ�к궨��
//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// �Ѿ���ChCreateProcess����ShellExecute�����ӽ���
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

//#define _WIN32_WINNT 0x0500	// ע�⣺����� #define _WIN32_WINNT  0x0500��������뱨��

#include <windows.h>
#include <TlHelp32.h>

#include <algorithm>
#include <functional>

#include <string>
#include <iostream>
#include <vector>

#include <io.h>

//using namespace std;
using std::string; using std::vector;

// ���ַ���ת��ΪСд��Ϊ����������ʱ�ַ������ַ������������Ч�ʡ�������ã�����ֵ�����÷�ʽ
//CString str;
//str.MakeLower();
//str.MakeUpper();
CString& StrToLowerQ(CString& str)
{
	str.MakeLower();
	return str;
}

CString StrToLower(const CString& str)
{
	CString strTemp = str;
	strTemp.MakeLower();
	return strTemp;
}

// �ݹ�õ���������2012-12-26
void GetProcessTree(DWORD dwProcessID, vector<DWORD>& vecProcessPID)
{
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, dwProcessID);
	if(hProcessSnap == INVALID_HANDLE_VALUE)
		return ;
	
	BOOL bMore = Process32First(hProcessSnap, &pe);
	
	while( bMore != FALSE)
	{
		if (dwProcessID != pe.th32ProcessID && dwProcessID == pe.th32ParentProcessID)
		{
			vecProcessPID.push_back(pe.th32ProcessID);
			GetProcessTree(pe.th32ProcessID, vecProcessPID);
		}
		bMore = Process32Next(hProcessSnap, &pe);
	}
	
	CloseHandle(hProcessSnap);
}


// �˷���������Ҫ����OpenProcess������������Ҫһ����Ȩ�ޣ���Ȩ�޲���ʱ����Щ���̽����ܱ��򿪡�
// �����������Ȩ�޵���ش��룺
// hProcess [in] : Ҫ�����Ľ��̣�Ŀ����̡�2005-12-15 13:35 ������
// lpPrivilegeName [in] : Ҫ����������Ȩ��Ŀ����Ȩ
// ����ֵ : TRUE : �ɹ�; FALSE : ʧ��
BOOL UpdateProcessPrivilege(HANDLE hProcess, LPCTSTR lpPrivilegeName)
{
	HANDLE hToken;
	int iResult;
	TOKEN_PRIVILEGES TokenPrivileges;
	
	if (OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &hToken))
	{
		LUID destLuid;
		if (LookupPrivilegeValue(NULL, lpPrivilegeName, &destLuid))
		{
			TokenPrivileges.PrivilegeCount = 1;
			TokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			TokenPrivileges.Privileges[0].Luid = destLuid;
			
			if (iResult = ::AdjustTokenPrivileges(hToken, FALSE, &TokenPrivileges, 0, NULL, NULL))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

// ��·������ļ���
CString GetFileNameFromPath(const CString& strPath)
{
	return strPath.Right(strPath.GetLength() - strPath.ReverseFind(L'\\') - 1);
}

// �жϽ����Ƿ����
bool FindProcess(CString& strProcessName)
{
	bool bProcessExist = false;
	PROCESSENTRY32 pe = {0};
	pe.dwSize = sizeof(pe);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return false;
	
	BOOL bRet = Process32First(hProcessSnap, &pe);	//��������ֵΪBOOL����bool�����о���
	while (bRet)
	{
		if (0 == _wcsicmp(strProcessName, pe.szExeFile))	//�����ִ�Сд��wide case insensitive compare
		{
			bProcessExist = true;
			break;	//�ҵ������ѭ��
		}
		bRet = Process32Next(hProcessSnap, &pe);	//��������ֵΪBOOL����bool�����о���
	}
	CloseHandle(hProcessSnap);
	
	return bProcessExist;
}

// �ӽ���dwProcessID��ȡ��������strProcessName
CString GetProcessName(DWORD dwProcessID)
{
	CString strProcessName = L"";
	PROCESSENTRY32 pe = {0};
	pe.dwSize = sizeof(pe);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return strProcessName;
	
	BOOL bRet = Process32First(hProcessSnap, &pe);	//��������ֵΪBOOL����bool�����о���
	while (bRet)
	{
		if (dwProcessID == pe.th32ProcessID)
		{
			strProcessName = pe.szExeFile;
			break;		//�ҵ������ѭ��
		}
		bRet = Process32Next(hProcessSnap, &pe);	//��������ֵΪBOOL����bool�����о���
	}
	CloseHandle(hProcessSnap);
	
	return strProcessName;
}

// �ӽ�������strProcessName��ȡ�䶥������dwProcessID����������Ҫ���丸�������Ʒ�strProcessName
DWORD GetTopProcessID(CString& strProcessName)
{
	DWORD dwProcessID = 0;
	PROCESSENTRY32 pe = {0};
	pe.dwSize = sizeof(pe);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return dwProcessID;
	
	BOOL bRet = Process32First(hProcessSnap, &pe);	//��������ֵΪBOOL����bool�����о���
	while (bRet)
	{
		if (0 == _wcsicmp(strProcessName, pe.szExeFile))//�����ִ�Сд��wide case insensitive compare
		{
			//��������Ҫ���丸�������Ʒ�strProcessName
			if (0 != _wcsicmp( strProcessName, GetProcessName(pe.th32ParentProcessID) ))
			{
				dwProcessID = pe.th32ProcessID;
				break;			//�ҵ������ѭ��
			}
		}
		bRet = Process32Next(hProcessSnap, &pe);	//��������ֵΪBOOL����bool�����о���
	}
	CloseHandle(hProcessSnap);
	
	return dwProcessID;
}

//�Ż�������ָ��Ŀ¼������Ŀ¼��ָ�����Ƶ��ļ�(���ִ�Сд)���ҵ����ص�һ���ҵ��ļ���ȫ·�������򷵻ؿ�
CString wstrTraveDir(const CString& strCurrDir, const CString& strFileName)
{
	if (L"" == strCurrDir || L"" == strFileName)	return L"";
	
	static CString strResult = L"";
	struct _wfinddata_t c_file = {0};
	CString strDirOld = strCurrDir;
	if(strDirOld[strDirOld.GetLength() - 1] != L'\\')	strDirOld += L"\\";
	
	//������ΪVS2017Ĭ��ѡ��x64����ģʽ���£�ѡ��x86����ģʽû�о���
	//warning C4244: 'initializing': conversion from 'intptr_t' to 'long', possible loss of data
	//long _wfindfirst( wchar_t *filespec, struct _wfinddata_t *fileinfo );
	long hFile = _wfindfirst(strDirOld + L"*.*", &c_file);
	if(-1 == hFile)	return L"";
	
	do
	{
		if(0 == wcscmp(c_file.name, L".") || 0 == wcscmp(c_file.name, L".."))	continue;
		
		if(c_file.attrib & _A_SUBDIR )			// search SubDdir
			wstrTraveDir(strDirOld + c_file.name, strFileName);
		else if(0 == _wcsicmp(c_file.name, strFileName) && strResult.IsEmpty())//stricmp���ִ�Сд��strcmp�ִ�Сд
		{
			strResult = strDirOld + c_file.name;
			break;
		}
	} while(0 == _wfindnext(hFile, &c_file));
	
	_findclose(hFile);
	
	return strResult;
}

//��CreateProcess�����ӽ���strAppCmd������ֵ���붨��ΪBOOL�������о���
BOOL ChCreateProcess(STARTUPINFO& siApp, PROCESS_INFORMATION& piApp, CString strAppCmd, int nAppShow)
{
	//��CreateProcess�����ӽ���strAppCmd
	ZeroMemory( &siApp, sizeof(siApp) );
	siApp.cb = sizeof(siApp);
	siApp.dwFlags = STARTF_USESHOWWINDOW;
	siApp.wShowWindow = nAppShow;
	ZeroMemory( &piApp, sizeof(piApp) );
	
	// �����ӽ���strAppCmd������ֵ���붨��ΪBOOL�������о���
	BOOL bAppCreateProcess = CreateProcessW(
		NULL,						// No module name (use command line).
		(LPWSTR)(LPCWSTR)strAppCmd,	// Command line.
		NULL,						// Process handle not inheritable.
		NULL,						// Thread handle not inheritable.
		FALSE,						// Set handle inheritance to FALSE.
		0,							// No creation flags.
		NULL,						// Use parent's environment block.
		NULL,						// Use parent's starting directory.
		&siApp,						// Pointer to STARTUPINFO structure.
		&piApp );					// Pointer to PROCESS_INFORMATION structure.

	//strAppCmd.ReleaseBuffer();		//��Ȼ�������ֲ�����strAppCmd��ʧ�����Բ���ReleaseBuffer()
	//����Ϊһ�����ñ��ϰ�ߣ��Ժ󷲱�����CString-GetBuffer��LPTSTR��LPSTR����LPWSTR�ĵط������������ReleaseBuffer()

	return bAppCreateProcess;
}

//ǿ����ֹvecProcessPID��ŵ��ӽ��̣��ӽ����޷�����β����
bool ChTerminateProcess(vector<DWORD>& vecProcessPID)
{
	HANDLE hProcess;
	for(vector<DWORD>::iterator it = vecProcessPID.begin(); it != vecProcessPID.end(); it++)
	{
		if (*it)
		{
			hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, *it);
			if (hProcess)
				TerminateProcess(hProcess, 4);	//ǿ����ֹ�ӽ��̣��ӽ����޷�����β����
		}
	}

	return true;
}

// ----------------------------------------------------------------------------------------
// UNICODE CString�ַ���תstring�ַ�����Quote�����Ч��
// ----------------------------------------------------------------------------------------
string& CStringToStringQ(const CString& wstr, string& str)
{
	str = "";		//��������շ��ر���str������wstrΪ��ʱ�᷵��һЩδ֪����
	if (L"" == wstr)
		return str;

	int nLen = WideCharToMultiByte(CP_ACP, 0, wstr, wstr.GetLength(), NULL, 0, NULL, NULL);
	char* pChar = new char[nLen + 1];
	if (!pChar)
		return str;

	WideCharToMultiByte(CP_ACP, 0, wstr, wstr.GetLength(), pChar, nLen, NULL, NULL);
	pChar[nLen] = '\0';

	str = pChar;
	delete[] pChar;

	return str;
}

//CString Trim��Ϊ�˵��ø�Ϊ��࣬���Բ������ò����ͷ���ֵ
CString CTrim(CString str)
{
	str.TrimLeft();
	str.TrimRight();
	return str;
}

//CString Trim���������ò����ͷ���ֵ Quote
CString& CTrimQ(CString& str)
{
	str.TrimLeft();
	str.TrimRight();
	return str;
}

// CString��תint  ���ַ�ת��Ϊ����������ʹ��atoi��_atoi64��atol��

/*
char strInt[255];
int len = str.length();
memset(strInt, 0, sizeof(strInt));
sprintf(strInt, "%02d", rand() % 100);			//��rand() % 100תΪ2λ(����ǰ�油0)10���Ʊ�ʾ���ַ�����
*/
// CString sת��Ϊint��int n = _wtoi(s);
// ��intת����CString
CString itos(int i)
{
	//wchar_t str[50];	//����sstream�⺯�����������exe�ļ���С300K
	//_itow(i, str, 10);

	CString str;
	str.Format(L"%d", i);
	return str;
}

// ��intת����nλCString
CString itosn(int i, int n)
{
	//wchar_t str[50];	//����sstream�⺯�����������exe�ļ���С300K
	//_itow(i, str, 10);

	CString str;
	CString str2 = L"%0" + itos(n) + L"d";
	str.Format(str2, i);
	return str;
}

// ��intת����stl��string
string itostl(int i)
{
	char str[50];	//����sstream�⺯�����������exe�ļ���С300K
	//itoa(i, str, 10);
	memset(str, 0, sizeof(str));
	sprintf_s(str, "%d", i);
	return str;
}

// ��intת����nλstl��string
string itostln(int i, int n)
{
	char str[50];	//����sstream�⺯�����������exe�ļ���С300K
	//itoa(i, str, 10);
	memset(str, 0, sizeof(str));
	string str2 = "%0" + itostl(n) + "d";
	sprintf_s(str, str2.c_str(), i);
	return str;
}

// ----------------------------------------------------------------------------------------
//best-CString��split������strSep�����Ƕ���ַ���WIn32Con_UNICDOE_01-CStringSplit���Գɹ�. S: Skip Empty Field
// ----------------------------------------------------------------------------------------
int CStringSplitS(vector<CString>& vecResult, const CString& str, CString strSep)
{
	vecResult.clear();
	if (L"" == str)
		return 0;

	int len = strSep.GetLength();
	int next = 0;
	int it = str.Find(strSep, 0);
	while (it != -1)
	{
		if (it - next >= len)		//S: Skip Empty Field	//�����Ǵ���strSep�ĳ��ȣ������ǿ��ֶ�
			vecResult.push_back(CTrim(str.Mid(next, it - next)));
		next = it + len;
		it = str.Find(strSep, next);
	}

	//��������ĩβ�ֶΣ���δ����strSep�����Բ�Ҫ > len��ֻҪ >0���Ͳ��ǿ��ֶΡ�
	//�����(str.GetLength() - next) > len���ͻὫ����С��len��ĩβ�ֶ�©��
	if (it = -1 && (str.GetLength() - next) > 0)	//ʵ���ϣ����� it = -1 ��ʡ�ԡ�Ϊ�����Ķ���⣬���Ա���
		vecResult.push_back(CTrim(str.Right(str.GetLength() - next)));

	return vecResult.size();
}

// ----------------------------------------------------------------------------------------
//best-CString��split������strSep�����Ƕ���ַ���WIn32Con_UNICDOE_01-CStringSplit���Գɹ�.
// ----------------------------------------------------------------------------------------
vector<CString>& CStringSplit(vector<CString>& vecResult, const CString& str, CString strSep)
{
	vecResult.clear();
	if (L"" == str)
		return vecResult;

	int len = strSep.GetLength();
	int next = 0;
	int it = str.Find(strSep, 0);
	while (it != -1)
	{
		vecResult.push_back(CTrim(str.Mid(next, it - next)));
		next = it + len;
		it = str.Find(strSep, next);
	}

	if (it = -1)	//ʵ���ϣ���һ��if������ʡ�ԡ�Ϊ�����Ķ���⣬���Ա���
		vecResult.push_back(CTrim(str.Right(str.GetLength() - next)));

	return vecResult;
}

// ----------------------------------------------------------------------------------------
//best-CString��split������strSep�����Ƕ���ַ���WIn32Con_UNICDOE_01-CStringSplit���Գɹ�.
// ----------------------------------------------------------------------------------------
int CStringSplitN(vector<CString>& vecResult, const CString& str, CString strSep)	//�����ֶ���
{
	vecResult.clear();
	if (L"" == str)
		return 0;

	int len = strSep.GetLength();
	int next = 0;
	int it = str.Find(strSep, 0);
	while (it != -1)
	{
		vecResult.push_back(CTrim(str.Mid(next, it - next)));
		next = it + len;
		it = str.Find(strSep, next);
	}

	if (it = -1)	//ʵ���ϣ���һ��if������ʡ�ԡ�Ϊ�����Ķ���⣬���Ա���
		vecResult.push_back(CTrim(str.Right(str.GetLength() - next)));

	return vecResult.size();;
}
// ----------------------------------------------------------------------------------------
//ȡ��ǰ·����0 ���ص�ǰȫ·��(���ļ���)��1 ���ص�ǰĿ¼(ĩβ��б��)��2 ���ص�ǰĿ¼(ĩβ��б��)��3 ���ص�ǰ��������4 ���ص�ǰģ����
//ϵͳ��һ������GetCurrentDirectory();
CString getCurDir(int iFlag)
{
	CString strCurrentPath;
	GetModuleFileNameW(NULL, strCurrentPath.GetBuffer(MAX_PATH), MAX_PATH);
	strCurrentPath.ReleaseBuffer(wcslen(strCurrentPath));

	switch (iFlag)
	{
	case 0:		//���ص�ǰȫ·��(���ļ���)
		return strCurrentPath;
		break;
	case 1:		//���ص�ǰĿ¼(ĩβ��б��)
		return strCurrentPath.Left(strCurrentPath.ReverseFind(L'\\'));	//ĩβ��б��
		break;
	case 2:		//���ص�ǰĿ¼(ĩβ��б��)
		return strCurrentPath.Left(strCurrentPath.ReverseFind(L'\\') + 1);	//ĩβ��б��
		break;
	case 3:		//���ص�ǰ������
		return strCurrentPath.Right(strCurrentPath.Find(L':') + 1);		//ĩβ��б��
		break;
	case 4:		//���ص�ǰģ����
		return GetFileNameFromPath(strCurrentPath);
		break;
	default:
		return L"";
		break;
	}
}

// ----------------------------------------------------------------------------------------
// �Ӹ���ȫ·����ȡ�ļ�����·����
//iFlag:		0 ���ش���չ�����ļ�����1 ���ز�����չ�����ļ�����
//			2 ���ز����ļ�����·��(ĩβ��б��)��3 ���ز����ļ�����·��(ĩβ��б��)
CString getPathPart(CString strPath, int iFlag)
{
	switch (iFlag)
	{
	case 0:		//���ش���չ�����ļ���
		strPath = strPath.Right(strPath.GetLength() - strPath.ReverseFind(L'\\') - 1);
		return strPath;
		break;
	case 1:		//���ز�����չ�����ļ���
		strPath = strPath.Right(strPath.GetLength() - strPath.ReverseFind(L'\\') - 1);
		strPath = strPath.Left(strPath.ReverseFind(L'.'));
		return strPath;
		break;
	case 2:		//���ز����ļ�����·��(ĩβ��б��)
		return strPath.Left(strPath.ReverseFind(L'\\'));		//ĩβ��б��
		break;
	case 3:		//���ز����ļ�����·��(ĩβ��б��)
		return strPath.Left(strPath.ReverseFind(L'\\') + 1);	//ĩβ��б��
		break;
	default:
		return L"";
		break;
	}
}

/*
RegOpenKeyEx���� 2��ԭ��
����ͬ���ǽ��죬��ʹ���������RegOpenKeyEx��ʱ������ִ�в��ɹ�������������2��GetLastError����0��
  ��CSDN�ϲ�������˵�Ƿ���2��ԭ����ע����ж�Ӧ·�������ڣ������ҵ�����ע����Ǹ���ֵ�������ڵġ�
  �������������ԣ�������ֵ��ѯ����VC6��VS2010����������vs2008����������vc6�²��ܲ�ѯʲ���ֱ���ܳɹ���
  �����˰���ʱ�䣬����ǲ��С�

  ����ڷ��������ʱ�򣬾��ô��������⣬�������ڷ����ˣ�
	���õ���TCHAR���������Һ������õ���RegOpenKeyExA������ԭ������������

	�����Ҿ���RegOpenKeyEx����2��ԭ��һ��·����Ĳ����ڣ����ǲ���������������Ȩ�����⣨�����û����������
*/

//��ȡ��д��ע����ֵ
//hkeyRootkey��������strHkey��������strSubkey���Ӽ���dwType����ֵ���ͣ�&strKeyVal����ֵ��iOption��������0��ȡ(Ĭ��)��1д��
CString& rwKeyVal(HKEY hkeyRootkey, CString strHkey, CString strSubkey, DWORD dwType, CString &strKeyVal, int iOption = 0)
{
	HKEY hKey;	//HKEY��handle key��΢��ĸ�������
				//RegOpenKeyEx����ֵ ����������óɹ����򷵻�0��ERROR_SUCCESS�������򣬷���ֵΪ�ļ�WINERROR.h�ж����һ������Ĵ�����롣
	LONG result = ::RegOpenKeyEx(hkeyRootkey, strHkey, 0, KEY_READ | KEY_WRITE, &hKey);
	//KEY_READ�򿪵�Ȼ����д�ˣ�ҪKEY_READ | KEY_WRITE �򿪲��ܶ�д

	//rwKeyVal(HKEY_LOCAL_MACHINE, L"SOFTWARE\\MySoftwarePath", L"PrePath", REG_SZ, strFilePath, 1);
	//Win2008R2��64λϵͳ������
	//[HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\MySoftwarePath]
	//"PrePath" = "G:\\MyVC2018\\WTL_ICON\\Debug\\myFile1.ini"

	//��ָ�����Ӽ���������::RegOpenKeyEx��ʧ�ܷ��ط�0������Ҫ�ȴ���ָ�����Ӽ���hKey���ش����Ӽ��ľ��
	//RegCreateKey����[Long] 0��ERROR_SUCCESS����ʾ�ɹ��������κ�ֵ������һ���������.
	if (0 != result)
	{
		result = ::RegCreateKey(hkeyRootkey, strHkey, &hKey);
	}

	if (0 == iOption)		//0��ȡ(Ĭ��)
	{
		wchar_t retBuff[2048];
		ZeroMemory(retBuff, 2048);
		DWORD szRetBuff = sizeof(retBuff) / sizeof(wchar_t);
		result = RegQueryValueExW(hKey, strSubkey, 0, &dwType, (LPBYTE)retBuff, &szRetBuff);
		RegCloseKey(hKey);
		strKeyVal = retBuff;
	}
	else if (1 == iOption)	//	1д��
	{
		//RegSetValueEx����ֵLong��0��ERROR_SUCCESS����ʾ�ɹ��������κ�ֵ������һ��������롣
		//����ע�⣺UnicodeʱRegSetValueExW�����һ�������������2�������ֻ��д���һ���ַ�
		result = RegSetValueExW(hKey, strSubkey, 0, dwType, (LPBYTE)strKeyVal.GetBuffer(0), strKeyVal.GetLength() * 2);
		strKeyVal.ReleaseBuffer();
		RegCloseKey(hKey);
	}

	return strKeyVal;
}

//��ò����ļ�����·��(ĩβ��б��)�еĲ��ظ�Ψһ�ļ�����(������չ��)
CString getUniqFileName(CString strPath, CString strFileName)
{
	//�鿴���ļ�strOldName�Ƿ���ڣ������������θ���Ϊ��mySysFontSet1��...��mySysFontSetN
	//�鿴���ļ�strOldName�Ƿ���ڣ������������Ϊ��ʱ�����ļ��������ڴ�������е�����쳣���¼�¼���ݶ�ʧ
	//��_waccess(�����io.h)����fopen�ж��ļ��Ƿ���ڣ���fopen���ļ����ɶ�������

	//ȥ���ַ���ĩβ����������
	while ((strFileName[strFileName.GetLength() - 1] >= '0' && strFileName[strFileName.GetLength() - 1] <= '9'))
	{
		strFileName = strFileName.Left(strFileName.GetLength() - 1);
	}

	int i = 0;
	CString strOldName = strFileName;
	while (0 == _waccess(strPath + L"\\" + strFileName + L".ini", 0))		//�ļ�����_waccess����0�����򷵻�-1
	{
		i++;
		strFileName = strOldName + itos(i);
	}

	return strFileName;
}

//////////////////////////////////////////////////////////////////////////
//��ȡWindowsϵͳ�汾-Begin
//2016��08��24�� 18:21 : 48	�Ķ�����261
//GetVersionExֻ��Ӧ��Win8��֮ǰ��ϵͳ����Windows8.1֮��GetVersionEx���API��΢�����ĸ������ˣ�
//Ҳ����˵��Windows8.1��ʼ֮�󣨰���Windows10�������API��������¾��Ƿ���6.2�ˡ�

//VerifyVersionInfo ΢���Ƽ�ʹ��
//��������ָ���İ汾�ź͵�ǰϵͳʵ�ʵİ汾�Ž��бȽϣ��ȽϷ�����û��GetVersionEx����������

// �Ƿ��ָ���汾�����
BOOL IsWinVerEqualTo(DWORD dwMajorVersion, DWORD dwMinorVersion)
{
	OSVERSIONINFOEX osvi;
	DWORDLONG dwlConditionMask = 0;

	// ��ʼ��ϵͳ�汾��Ϣ���ݽṹ
	ZeroMemory(&osvi, sizeof(osvi));
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	osvi.dwMajorVersion = dwMajorVersion;
	osvi.dwMinorVersion = dwMinorVersion;

	// ��ʼ����������
	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_EQUAL);
	VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_EQUAL);

	return VerifyVersionInfo(&osvi, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionMask);
}

//The following code verifies that the application is running on Windows Server
BOOL IsWinServer(DWORD dwMajorVersion, DWORD dwMinorVersion)
{
	OSVERSIONINFOEX osvi;
	DWORDLONG dwlConditionMask = 0;
	int op = VER_GREATER_EQUAL;

	// Initialize the OSVERSIONINFOEX structure.

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	osvi.dwMajorVersion = dwMajorVersion;
	osvi.dwMinorVersion = dwMinorVersion;
	osvi.wServicePackMajor = 0;
	osvi.wServicePackMinor = 0;
	osvi.wProductType = VER_NT_SERVER;

	// Initialize the condition mask.

	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, op);
	VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, op);
	VER_SET_CONDITION(dwlConditionMask, VER_SERVICEPACKMAJOR, op);
	VER_SET_CONDITION(dwlConditionMask, VER_SERVICEPACKMINOR, op);
	VER_SET_CONDITION(dwlConditionMask, VER_PRODUCT_TYPE, VER_EQUAL);

	// Perform the test.

	return VerifyVersionInfo(
		&osvi,
		VER_MAJORVERSION | VER_MINORVERSION |
		VER_SERVICEPACKMAJOR | VER_SERVICEPACKMINOR |
		VER_PRODUCT_TYPE,
		dwlConditionMask);
}

// �Ƿ����ָ���汾��
BOOL IsWinVerGreaterThan(DWORD dwMajorVersion, DWORD dwMinorVersion)
{
	OSVERSIONINFOEX osvi;
	DWORDLONG dwlConditionMask = 0;

	// ��ʼ��ϵͳ�汾��Ϣ���ݽṹ
	ZeroMemory(&osvi, sizeof(osvi));
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	osvi.dwMajorVersion = dwMajorVersion;
	osvi.dwMinorVersion = dwMinorVersion;

	// ��ʼ����������
	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_GREATER);

	// ������ж�
	if (VerifyVersionInfo(&osvi, VER_MAJORVERSION, dwlConditionMask))
	{
		return TRUE;
	}

	// ��ʼ����������
	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_EQUAL);
	VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_GREATER);

	// �ΰ汾���ж�
	return VerifyVersionInfo(&osvi, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionMask);
}

// �Ƿ�С��ָ���汾��
BOOL IsWinVerLessThan(DWORD dwMajorVersion, DWORD dwMinorVersion)
{
	OSVERSIONINFOEX osvi;
	DWORDLONG dwlConditionMask = 0;

	// ��ʼ��ϵͳ�汾��Ϣ���ݽṹ
	ZeroMemory(&osvi, sizeof(osvi));
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	osvi.dwMajorVersion = dwMajorVersion;
	osvi.dwMinorVersion = dwMinorVersion;

	// ��ʼ����������
	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_LESS);

	// ������ж�
	if (VerifyVersionInfo(&osvi, VER_MAJORVERSION, dwlConditionMask))
	{
		return TRUE;
	}

	// ��ʼ����������
	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_EQUAL);
	VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_LESS);

	// �ΰ汾���ж�
	return VerifyVersionInfo(&osvi, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionMask);
}

// ���Windowsϵͳ���ƺͰ汾(֧��Win95��Win10��WinNT4.0��WinSrv2016)
DWORD GetWinOsName(CString& strOsName)
{
	SYSTEM_INFO info;        //��SYSTEM_INFO�ṹ�ж�64λAMD������ 
	GetSystemInfo(&info);    //����GetSystemInfo�������ṹ 
	WORD wMajor, wMinor;

	if (IsWinVerEqualTo(4, 0))
	{
		wMajor = 4; wMinor = 0;
		if(IsWinServer(4, 0))
			strOsName = L"Windows NT 4.0 (v4.0)"; //1996��7�·��� 
		else
			strOsName = L"Windows 95 (v4.0)";
	}
	else if (IsWinVerEqualTo(4, 10))
	{
		wMajor = 4; wMinor = 10;
		strOsName = L"Windows 98 (v4.10)";
	}
	else if (IsWinVerEqualTo(4, 90))
	{
		wMajor = 4; wMinor = 90;
		strOsName = L"Windows Me (v4.90)";
	}
	else if (IsWinVerEqualTo(5, 0))
	{
		wMajor = 5; wMinor = 0;
		if (IsWinServer(5, 0))
			strOsName = L"Windows Server 2000 (v5.0)";
		else
			strOsName = L"Windows 2000 (v5.0)";//1999��12�·���
	}
	else if (IsWinVerEqualTo(5, 1))
	{
		wMajor = 5; wMinor = 1;
		strOsName = L"Windows XP (v5.1)";//2001��8�·���
	}
	else if (IsWinVerEqualTo(5, 2))
	{
		wMajor = 5; wMinor = 2;
		if (!IsWinServer(5, 2) && info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
		{
			strOsName = L"Windows XP Professional x64 Edition (v5.2)";
		}
		else if (IsWinServer(5, 2) && GetSystemMetrics(SM_SERVERR2) == 0)
			strOsName = L"Windows Server 2003 (v5.2)";//2003��3�·��� 
		else if (IsWinServer(5, 2) && GetSystemMetrics(SM_SERVERR2) != 0)
			strOsName = L"Windows Server 2003 R2 (v5.2)";
	}
	else if (IsWinVerEqualTo(6, 0))
	{
		wMajor = 6; wMinor = 0;
		if (IsWinServer(6, 0))
			strOsName = L"Windows Server 2008 (v6.0)";//�������汾 
		else
			strOsName = L"Windows Vista (v6.0)";
	}
	else if (IsWinVerEqualTo(6, 1))
	{
		wMajor = 6; wMinor = 1;
		if (IsWinServer(6, 1))
			strOsName = L"Windows Server 2008 R2 (v6.1)";
		else
			strOsName = L"Windows 7 (v6.1)";
	}
	else if (IsWinVerEqualTo(6, 2))
	{
		wMajor = 6; wMinor = 2;
		if (IsWinServer(6, 2))
			strOsName = L"Windows Server 2012 (v6.2)";
		else
			strOsName = L"Windows 8 (v6.2)";
	}
	else if (IsWinVerEqualTo(6, 3))
	{
		wMajor = 6; wMinor = 3;
		if (IsWinServer(6, 3))
			strOsName = L"Windows Server 2012 R2 (v6.3)";
		else
			strOsName = L"Windows 8.1 (v6.3)";
	}
	else if (IsWinVerEqualTo(10, 0))
	{
		wMajor = 10; wMinor = 0;
		if (IsWinServer(10, 0))
			strOsName = L"Windows Server 2016 (v10.0)";//�������汾 
		else
			strOsName = L"Windows 10 (v10.0)";
	}
	else
	{
		strOsName = L"UNKNOMN Windows Version";
	}

	//LRESULT lResult = MAKEWPARAM(wLow,wHigh);  //�ϳ�LRESULT������ֵ��WPARAM��DWORD
	return MAKEWPARAM(wMajor, wMinor);
}

/*
ʲô��vs �����manifest�ļ�
manifest ��VS��������������������side-by-side�齨,��ATL, CRT�ȵ��嵥��

ΪʲôҪ��manifest�ļ�
һ̨pc�ϣ���һ�齨�������в�ֹһ���汾��c:/windows/winsxs��ϵͳĿ¼�£��������ڼ��ص�ʱ�򣬲�֪�����ĸ���
����manifest�ļ���ָ����

manifest���Ķ�����δ�����
�����VS����������Setͨ��porperty->configuration properties->linker->manifest file->Generate manifest To Yes
���Զ�����manifest��ָ��ϵͳ�ĺ�CRT��assembly�汾��

�������������ⲿ��manifest file������embedded manifest��Ϣ���Ա�д�������ɵĶ������ļ���
Set porperty->configuration properties->manifest tool->embed manifest  To Yes

����xp����ǰ��windows�汾��external manifest���embed manifest�и��ߵ����ȼ���
������windows server����İ汾���෴��

�� Visual Studio �������������ô�������ѡ��
����Ŀ�ġ�����ҳ���Ի��� �й���ϸ��Ϣ����μ���Σ�����Ŀ����ҳ��
չ�����������ԡ��ڵ㡣
չ�������������ڵ㡣
ѡ���嵥�ļ�������ҳ��
�޸ġ������嵥�����ԡ�
*/

//��������Ŀ·�����ֶ�����һ��UTF8��ʽ���ļ�������Ҫ���������ճ����ȥ���ٽ����ļ�������Ŀ���ɡ����磺
//g:\MyVC2017\noMeiryoUI235\noMeiryoUI235\res\noMeiryoUI235.exe.manifest

//��Ȼ���������manifest�ļ���Win8.1��Win10 ���صİ汾�Ż���6.2��
//ֻ����<application></application> ����Ӹ�ϵͳID���ɣ�������ʾ��
/*
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly manifestVersion="1.0" xmlns="urn:schemas-microsoft-com:asm.v1" xmlns:asmv3="urn:schemas-microsoft-com:asm.v3">
    <assemblyIdentity 
        type="win32" 
        name="Test.exe"
        version="1.0.0.0"
        processorArchitecture="X86"
    />
    <description> my exe </description>
    <trustInfo xmlns="urn:schemas-microsoft-com:asm.v3">
        <security>
            <requestedPrivileges>
                <requestedExecutionLevel
                    level="asInvoker"
                    uiAccess="false"
                />  
            </requestedPrivileges>
        </security>
    </trustInfo>
    <compatibility xmlns="urn:schemas-microsoft-com:compatibility.v1"> 
        <application> 
            <!-- Windows 10 --> 
            <supportedOS Id="{8e0f7a12-bfb3-4fe8-b9a5-48fd50a15a9a}"/>
            <!-- Windows 8.1 -->
            <supportedOS Id="{1f676c76-80e1-4239-95bb-83d0f6d0da78}"/>
            <!-- Windows Vista -->
            <supportedOS Id="{e2011457-1546-43c5-a5fe-008deee3d3f0}"/> 
            <!-- Windows 7 -->
            <supportedOS Id="{35138b9a-5d96-4fbd-8e2d-a2440225f93a}"/>
            <!-- Windows 8 -->
            <supportedOS Id="{4a2f28e3-53b9-4441-ba9c-d69d4a4a6e38}"/>
        </application> 
    </compatibility>
</assembly>
*/

//��ȡWindowsϵͳ�汾-End
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//ini�ļ�������-Begin
//private ��getSectionCount��getAllSections����
void myGetPrivateProfileSectionNames(wchar_t** buf, int bufLen, CString& iniFilePath)
{
	DWORD dwRes = GetPrivateProfileSectionNames(*buf, bufLen, iniFilePath);
	//��������С,�������û�����
	while (dwRes == bufLen - 2)
	{
		delete[]buf;
		bufLen += 1024;
		*buf = new wchar_t[bufLen];
		dwRes = GetPrivateProfileSectionNames(*buf, bufLen, iniFilePath);
	}
}

//��ȡ���е�section��
void getAllSections(vector<CString>& vecSections, CString& iniFilePath)
{
	int _bufLen = 1024;
	vecSections.clear();

	wchar_t* buf = new wchar_t[_bufLen];
	myGetPrivateProfileSectionNames(&buf, _bufLen, iniFilePath);

	/*
	**ͨ��string��ȡ��ÿ��section������
	**GetPrivateProfileSectionNamesȡ����sections��\0���������buf��
	**string����\0����ֹ
	*/
	wchar_t* p = buf;
	CString strName = p;
	while (strName.GetLength() != 0)
	{
		vecSections.push_back(strName);
		p += strName.GetLength() + 1;
		strName = p;
	}

	delete[]buf;
}

//�ж��Ƿ����ָ����section
bool isSectionExists(CString sectionName, CString& iniFilePath)
{
	vector<CString> vec;
	getAllSections(vec, iniFilePath);

	int count = vec.size();
	for (int i = 0; i < count; i++)
	{
		if (vec[i] == sectionName)
		{
			return true;
		}
	}

	return false;
}
//ini�ļ�������-End
//////////////////////////////////////////////////////////////////////////
BOOL isEngChar(wchar_t w) {
	if (!(w >= 'A' && w <= 'Z') && !(w >= 'a' && w <= 'z')) {
			return false;
	}
	return true;
}