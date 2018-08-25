// CommonFun.h，通用函数头文件
#pragma once

// 为了不依赖windows.h中的一些旧内容，启用下面一行宏定义
//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// 已经用ChCreateProcess代替ShellExecute启动子进程
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

//#define _WIN32_WINNT 0x0500	// 注意：必须加 #define _WIN32_WINNT  0x0500，否则编译报错

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

// 将字符串转换为小写，为避免生成临时字符串和字符串拷贝、提高效率、方便调用，返回值用引用方式
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

// 递归得到进程树，2012-12-26
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


// 此方法由于需要进行OpenProcess操作，所以需要一定的权限，当权限不够时，有些进程将不能被打开。
// 下面给出提升权限的相关代码：
// hProcess [in] : 要提升的进程，目标进程。2005-12-15 13:35 艾凡赫
// lpPrivilegeName [in] : 要提升到的特权，目标特权
// 返回值 : TRUE : 成功; FALSE : 失败
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

// 从路径获得文件名
CString GetFileNameFromPath(const CString& strPath)
{
	return strPath.Right(strPath.GetLength() - strPath.ReverseFind(L'\\') - 1);
}

// 判断进程是否存在
bool FindProcess(CString& strProcessName)
{
	bool bProcessExist = false;
	PROCESSENTRY32 pe = {0};
	pe.dwSize = sizeof(pe);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return false;
	
	BOOL bRet = Process32First(hProcessSnap, &pe);	//函数返回值为BOOL，用bool将会有警告
	while (bRet)
	{
		if (0 == _wcsicmp(strProcessName, pe.szExeFile))	//不区分大小写，wide case insensitive compare
		{
			bProcessExist = true;
			break;	//找到便结束循环
		}
		bRet = Process32Next(hProcessSnap, &pe);	//函数返回值为BOOL，用bool将会有警告
	}
	CloseHandle(hProcessSnap);
	
	return bProcessExist;
}

// 从进程dwProcessID获取进程名称strProcessName
CString GetProcessName(DWORD dwProcessID)
{
	CString strProcessName = L"";
	PROCESSENTRY32 pe = {0};
	pe.dwSize = sizeof(pe);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return strProcessName;
	
	BOOL bRet = Process32First(hProcessSnap, &pe);	//函数返回值为BOOL，用bool将会有警告
	while (bRet)
	{
		if (dwProcessID == pe.th32ProcessID)
		{
			strProcessName = pe.szExeFile;
			break;		//找到便结束循环
		}
		bRet = Process32Next(hProcessSnap, &pe);	//函数返回值为BOOL，用bool将会有警告
	}
	CloseHandle(hProcessSnap);
	
	return strProcessName;
}

// 从进程名称strProcessName获取其顶级进程dwProcessID，顶级进程要求其父进程名称非strProcessName
DWORD GetTopProcessID(CString& strProcessName)
{
	DWORD dwProcessID = 0;
	PROCESSENTRY32 pe = {0};
	pe.dwSize = sizeof(pe);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return dwProcessID;
	
	BOOL bRet = Process32First(hProcessSnap, &pe);	//函数返回值为BOOL，用bool将会有警告
	while (bRet)
	{
		if (0 == _wcsicmp(strProcessName, pe.szExeFile))//不区分大小写，wide case insensitive compare
		{
			//顶级进程要求其父进程名称非strProcessName
			if (0 != _wcsicmp( strProcessName, GetProcessName(pe.th32ParentProcessID) ))
			{
				dwProcessID = pe.th32ProcessID;
				break;			//找到便结束循环
			}
		}
		bRet = Process32Next(hProcessSnap, &pe);	//函数返回值为BOOL，用bool将会有警告
	}
	CloseHandle(hProcessSnap);
	
	return dwProcessID;
}

//优化：查找指定目录及其子目录下指定名称的文件(不分大小写)。找到返回第一次找到文件的全路径，否则返回空
CString wstrTraveDir(const CString& strCurrDir, const CString& strFileName)
{
	if (L"" == strCurrDir || L"" == strFileName)	return L"";
	
	static CString strResult = L"";
	struct _wfinddata_t c_file = {0};
	CString strDirOld = strCurrDir;
	if(strDirOld[strDirOld.GetLength() - 1] != L'\\')	strDirOld += L"\\";
	
	//这是因为VS2017默认选择x64编译模式所致，选择x86编译模式没有警告
	//warning C4244: 'initializing': conversion from 'intptr_t' to 'long', possible loss of data
	//long _wfindfirst( wchar_t *filespec, struct _wfinddata_t *fileinfo );
	long hFile = _wfindfirst(strDirOld + L"*.*", &c_file);
	if(-1 == hFile)	return L"";
	
	do
	{
		if(0 == wcscmp(c_file.name, L".") || 0 == wcscmp(c_file.name, L".."))	continue;
		
		if(c_file.attrib & _A_SUBDIR )			// search SubDdir
			wstrTraveDir(strDirOld + c_file.name, strFileName);
		else if(0 == _wcsicmp(c_file.name, strFileName) && strResult.IsEmpty())//stricmp不分大小写，strcmp分大小写
		{
			strResult = strDirOld + c_file.name;
			break;
		}
	} while(0 == _wfindnext(hFile, &c_file));
	
	_findclose(hFile);
	
	return strResult;
}

//用CreateProcess创建子进程strAppCmd。返回值必须定义为BOOL，否则有警告
BOOL ChCreateProcess(STARTUPINFO& siApp, PROCESS_INFORMATION& piApp, CString strAppCmd, int nAppShow)
{
	//用CreateProcess创建子进程strAppCmd
	ZeroMemory( &siApp, sizeof(siApp) );
	siApp.cb = sizeof(siApp);
	siApp.dwFlags = STARTF_USESHOWWINDOW;
	siApp.wShowWindow = nAppShow;
	ZeroMemory( &piApp, sizeof(piApp) );
	
	// 创建子进程strAppCmd。返回值必须定义为BOOL，否则有警告
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

	//strAppCmd.ReleaseBuffer();		//虽然，用完后局部变量strAppCmd消失，可以不用ReleaseBuffer()
	//但作为一种良好编程习惯，以后凡必须用CString-GetBuffer、LPTSTR、LPSTR、或LPWSTR的地方，用完后都立即ReleaseBuffer()

	return bAppCreateProcess;
}

//强行终止vecProcessPID存放的子进程，子进程无法做收尾工作
bool ChTerminateProcess(vector<DWORD>& vecProcessPID)
{
	HANDLE hProcess;
	for(vector<DWORD>::iterator it = vecProcessPID.begin(); it != vecProcessPID.end(); it++)
	{
		if (*it)
		{
			hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, *it);
			if (hProcess)
				TerminateProcess(hProcess, 4);	//强行终止子进程，子进程无法做收尾工作
		}
	}

	return true;
}

// ----------------------------------------------------------------------------------------
// UNICODE CString字符串转string字符串。Quote。提高效率
// ----------------------------------------------------------------------------------------
string& CStringToStringQ(const CString& wstr, string& str)
{
	str = "";		//必须先清空返回变量str，否则wstr为空时会返回一些未知内容
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

//CString Trim。为了调用更为简洁，所以不用引用参数和返回值
CString CTrim(CString str)
{
	str.TrimLeft();
	str.TrimRight();
	return str;
}

//CString Trim。用于引用参数和返回值 Quote
CString& CTrimQ(CString& str)
{
	str.TrimLeft();
	str.TrimRight();
	return str;
}

// CString互转int  将字符转换为整数，可以使用atoi、_atoi64或atol。

/*
char strInt[255];
int len = str.length();
memset(strInt, 0, sizeof(strInt));
sprintf(strInt, "%02d", rand() % 100);			//将rand() % 100转为2位(不足前面补0)10进制表示的字符串。
*/
// CString s转换为int是int n = _wtoi(s);
// 将int转换成CString
CString itos(int i)
{
	//wchar_t str[50];	//不用sstream库函数，编译出的exe文件减小300K
	//_itow(i, str, 10);

	CString str;
	str.Format(L"%d", i);
	return str;
}

// 将int转换成n位CString
CString itosn(int i, int n)
{
	//wchar_t str[50];	//不用sstream库函数，编译出的exe文件减小300K
	//_itow(i, str, 10);

	CString str;
	CString str2 = L"%0" + itos(n) + L"d";
	str.Format(str2, i);
	return str;
}

// 将int转换成stl的string
string itostl(int i)
{
	char str[50];	//不用sstream库函数，编译出的exe文件减小300K
	//itoa(i, str, 10);
	memset(str, 0, sizeof(str));
	sprintf_s(str, "%d", i);
	return str;
}

// 将int转换成n位stl的string
string itostln(int i, int n)
{
	char str[50];	//不用sstream库函数，编译出的exe文件减小300K
	//itoa(i, str, 10);
	memset(str, 0, sizeof(str));
	string str2 = "%0" + itostl(n) + "d";
	sprintf_s(str, str2.c_str(), i);
	return str;
}

// ----------------------------------------------------------------------------------------
//best-CString的split函数，strSep可以是多个字符。WIn32Con_UNICDOE_01-CStringSplit测试成功. S: Skip Empty Field
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
		if (it - next >= len)		//S: Skip Empty Field	//必须是大于strSep的长度，否则是空字段
			vecResult.push_back(CTrim(str.Mid(next, it - next)));
		next = it + len;
		it = str.Find(strSep, next);
	}

	//这里已是末尾字段，并未包含strSep，所以不要 > len。只要 >0，就不是空字段。
	//如果用(str.GetLength() - next) > len，就会将长度小于len的末尾字段漏掉
	if (it = -1 && (str.GetLength() - next) > 0)	//实质上，条件 it = -1 可省略。为便于阅读理解，可以保留
		vecResult.push_back(CTrim(str.Right(str.GetLength() - next)));

	return vecResult.size();
}

// ----------------------------------------------------------------------------------------
//best-CString的split函数，strSep可以是多个字符。WIn32Con_UNICDOE_01-CStringSplit测试成功.
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

	if (it = -1)	//实质上，这一句if都可以省略。为便于阅读理解，可以保留
		vecResult.push_back(CTrim(str.Right(str.GetLength() - next)));

	return vecResult;
}

// ----------------------------------------------------------------------------------------
//best-CString的split函数，strSep可以是多个字符。WIn32Con_UNICDOE_01-CStringSplit测试成功.
// ----------------------------------------------------------------------------------------
int CStringSplitN(vector<CString>& vecResult, const CString& str, CString strSep)	//返回字段数
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

	if (it = -1)	//实质上，这一句if都可以省略。为便于阅读理解，可以保留
		vecResult.push_back(CTrim(str.Right(str.GetLength() - next)));

	return vecResult.size();;
}
// ----------------------------------------------------------------------------------------
//取当前路径。0 返回当前全路径(含文件名)；1 返回当前目录(末尾无斜杠)；2 返回当前目录(末尾含斜杠)；3 返回当前驱动器；4 返回当前模块名
//系统有一个函数GetCurrentDirectory();
CString getCurDir(int iFlag)
{
	CString strCurrentPath;
	GetModuleFileNameW(NULL, strCurrentPath.GetBuffer(MAX_PATH), MAX_PATH);
	strCurrentPath.ReleaseBuffer(wcslen(strCurrentPath));

	switch (iFlag)
	{
	case 0:		//返回当前全路径(含文件名)
		return strCurrentPath;
		break;
	case 1:		//返回当前目录(末尾无斜杠)
		return strCurrentPath.Left(strCurrentPath.ReverseFind(L'\\'));	//末尾无斜杠
		break;
	case 2:		//返回当前目录(末尾无斜杠)
		return strCurrentPath.Left(strCurrentPath.ReverseFind(L'\\') + 1);	//末尾含斜杠
		break;
	case 3:		//返回当前驱动器
		return strCurrentPath.Right(strCurrentPath.Find(L':') + 1);		//末尾无斜杠
		break;
	case 4:		//返回当前模块名
		return GetFileNameFromPath(strCurrentPath);
		break;
	default:
		return L"";
		break;
	}
}

// ----------------------------------------------------------------------------------------
// 从给定全路径获取文件名或路径。
//iFlag:		0 返回带扩展名的文件名；1 返回不带扩展名的文件名；
//			2 返回不带文件名的路径(末尾无斜杠)；3 返回不带文件名的路径(末尾带斜杠)
CString getPathPart(CString strPath, int iFlag)
{
	switch (iFlag)
	{
	case 0:		//返回带扩展名的文件名
		strPath = strPath.Right(strPath.GetLength() - strPath.ReverseFind(L'\\') - 1);
		return strPath;
		break;
	case 1:		//返回不带扩展名的文件名
		strPath = strPath.Right(strPath.GetLength() - strPath.ReverseFind(L'\\') - 1);
		strPath = strPath.Left(strPath.ReverseFind(L'.'));
		return strPath;
		break;
	case 2:		//返回不带文件名的路径(末尾无斜杠)
		return strPath.Left(strPath.ReverseFind(L'\\'));		//末尾无斜杠
		break;
	case 3:		//返回不带文件名的路径(末尾带斜杠)
		return strPath.Left(strPath.ReverseFind(L'\\') + 1);	//末尾带斜杠
		break;
	default:
		return L"";
		break;
	}
}

/*
RegOpenKeyEx返回 2及原因
　　同样是今天，在使用这个函数RegOpenKeyEx的时候，老是执行不成功，函数本身返回2，GetLastError返回0。
  在CSDN上查阅资料说是返回2的原因是注册表中对应路径不存在，可是我电脑中注册表那个键值明明存在的。
  就这样慢慢调试，换个键值查询，用VC6、VS2010（本来是用vs2008），可是在vc6下不管查询什麽简直都能成功，
  折腾了半天时间，最后还是不行。

  最后在分析代码的时候，觉得代码有问题，于是终于发现了：
	我用的是TCHAR！！！而且函数我用的是RegOpenKeyExA！！！原因就在这里！！！

	所以我觉得RegOpenKeyEx返回2的原因一是路径真的不存在，二是参数错误，三可能是权限问题（这个我没遇到）。　
*/

//读取和写入注册表键值
//hkeyRootkey：根键；strHkey：主键；strSubkey：子键；dwType：键值类型；&strKeyVal：键值；iOption：操作，0读取(默认)，1写入
CString& rwKeyVal(HKEY hkeyRootkey, CString strHkey, CString strSubkey, DWORD dwType, CString &strKeyVal, int iOption = 0)
{
	HKEY hKey;	//HKEY：handle key，微软的根键表项
				//RegOpenKeyEx返回值 如果函数调用成功，则返回0（ERROR_SUCCESS）。否则，返回值为文件WINERROR.h中定义的一个非零的错误代码。
	LONG result = ::RegOpenKeyEx(hkeyRootkey, strHkey, 0, KEY_READ | KEY_WRITE, &hKey);
	//KEY_READ打开当然不能写了，要KEY_READ | KEY_WRITE 打开才能读写

	//rwKeyVal(HKEY_LOCAL_MACHINE, L"SOFTWARE\\MySoftwarePath", L"PrePath", REG_SZ, strFilePath, 1);
	//Win2008R2等64位系统保存在
	//[HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\MySoftwarePath]
	//"PrePath" = "G:\\MyVC2018\\WTL_ICON\\Debug\\myFile1.ini"

	//若指定的子键不存在则::RegOpenKeyEx打开失败返回非0。就需要先创建指定的子键，hKey返回创建子键的句柄
	//RegCreateKey返回[Long] 0（ERROR_SUCCESS）表示成功。其他任何值都代表一个错误代码.
	if (0 != result)
	{
		result = ::RegCreateKey(hkeyRootkey, strHkey, &hKey);
	}

	if (0 == iOption)		//0读取(默认)
	{
		wchar_t retBuff[2048];
		ZeroMemory(retBuff, 2048);
		DWORD szRetBuff = sizeof(retBuff) / sizeof(wchar_t);
		result = RegQueryValueExW(hKey, strSubkey, 0, &dwType, (LPBYTE)retBuff, &szRetBuff);
		RegCloseKey(hKey);
		strKeyVal = retBuff;
	}
	else if (1 == iOption)	//	1写入
	{
		//RegSetValueEx返回值Long，0（ERROR_SUCCESS）表示成功。其他任何值都代表一个错误代码。
		//必须注意：Unicode时RegSetValueExW，最后一个参数必须乘以2，否则就只能写入第一个字符
		result = RegSetValueExW(hKey, strSubkey, 0, dwType, (LPBYTE)strKeyVal.GetBuffer(0), strKeyVal.GetLength() * 2);
		strKeyVal.ReleaseBuffer();
		RegCloseKey(hKey);
	}

	return strKeyVal;
}

//获得不含文件名的路径(末尾无斜杠)中的不重复唯一文件名称(不含扩展名)
CString getUniqFileName(CString strPath, CString strFileName)
{
	//查看该文件strOldName是否存在，若存在则依次更名为：mySysFontSet1、...、mySysFontSetN
	//查看该文件strOldName是否存在，若存在则更名为临时备份文件，以免在处理过程中掉电或异常导致记录数据丢失
	//用_waccess(需包含io.h)代替fopen判断文件是否存在，用fopen若文件不可读会误判

	//去除字符串末尾的所有数字
	while ((strFileName[strFileName.GetLength() - 1] >= '0' && strFileName[strFileName.GetLength() - 1] <= '9'))
	{
		strFileName = strFileName.Left(strFileName.GetLength() - 1);
	}

	int i = 0;
	CString strOldName = strFileName;
	while (0 == _waccess(strPath + L"\\" + strFileName + L".ini", 0))		//文件存在_waccess返回0，否则返回-1
	{
		i++;
		strFileName = strOldName + itos(i);
	}

	return strFileName;
}

//////////////////////////////////////////////////////////////////////////
//获取Windows系统版本-Begin
//2016年08月24日 18:21 : 48	阅读数：261
//GetVersionEx只适应于Win8及之前的系统。从Windows8.1之后，GetVersionEx这个API被微软明文给废弃了，
//也就是说从Windows8.1开始之后（包括Windows10），这个API常规情况下就是返回6.2了。

//VerifyVersionInfo 微软推荐使用
//本质是拿指定的版本号和当前系统实际的版本号进行比较，比较繁琐，没有GetVersionEx用起来方便

// 是否和指定版本号相等
BOOL IsWinVerEqualTo(DWORD dwMajorVersion, DWORD dwMinorVersion)
{
	OSVERSIONINFOEX osvi;
	DWORDLONG dwlConditionMask = 0;

	// 初始化系统版本信息数据结构
	ZeroMemory(&osvi, sizeof(osvi));
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	osvi.dwMajorVersion = dwMajorVersion;
	osvi.dwMinorVersion = dwMinorVersion;

	// 初始化条件掩码
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

// 是否大于指定版本号
BOOL IsWinVerGreaterThan(DWORD dwMajorVersion, DWORD dwMinorVersion)
{
	OSVERSIONINFOEX osvi;
	DWORDLONG dwlConditionMask = 0;

	// 初始化系统版本信息数据结构
	ZeroMemory(&osvi, sizeof(osvi));
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	osvi.dwMajorVersion = dwMajorVersion;
	osvi.dwMinorVersion = dwMinorVersion;

	// 初始化条件掩码
	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_GREATER);

	// 主板号判断
	if (VerifyVersionInfo(&osvi, VER_MAJORVERSION, dwlConditionMask))
	{
		return TRUE;
	}

	// 初始化条件掩码
	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_EQUAL);
	VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_GREATER);

	// 次版本号判断
	return VerifyVersionInfo(&osvi, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionMask);
}

// 是否小于指定版本号
BOOL IsWinVerLessThan(DWORD dwMajorVersion, DWORD dwMinorVersion)
{
	OSVERSIONINFOEX osvi;
	DWORDLONG dwlConditionMask = 0;

	// 初始化系统版本信息数据结构
	ZeroMemory(&osvi, sizeof(osvi));
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	osvi.dwMajorVersion = dwMajorVersion;
	osvi.dwMinorVersion = dwMinorVersion;

	// 初始化条件掩码
	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_LESS);

	// 主板号判断
	if (VerifyVersionInfo(&osvi, VER_MAJORVERSION, dwlConditionMask))
	{
		return TRUE;
	}

	// 初始化条件掩码
	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_EQUAL);
	VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_LESS);

	// 次版本号判断
	return VerifyVersionInfo(&osvi, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionMask);
}

// 获得Windows系统名称和版本(支持Win95到Win10、WinNT4.0到WinSrv2016)
DWORD GetWinOsName(CString& strOsName)
{
	SYSTEM_INFO info;        //用SYSTEM_INFO结构判断64位AMD处理器 
	GetSystemInfo(&info);    //调用GetSystemInfo函数填充结构 
	WORD wMajor, wMinor;

	if (IsWinVerEqualTo(4, 0))
	{
		wMajor = 4; wMinor = 0;
		if(IsWinServer(4, 0))
			strOsName = L"Windows NT 4.0 (v4.0)"; //1996年7月发布 
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
			strOsName = L"Windows 2000 (v5.0)";//1999年12月发布
	}
	else if (IsWinVerEqualTo(5, 1))
	{
		wMajor = 5; wMinor = 1;
		strOsName = L"Windows XP (v5.1)";//2001年8月发布
	}
	else if (IsWinVerEqualTo(5, 2))
	{
		wMajor = 5; wMinor = 2;
		if (!IsWinServer(5, 2) && info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
		{
			strOsName = L"Windows XP Professional x64 Edition (v5.2)";
		}
		else if (IsWinServer(5, 2) && GetSystemMetrics(SM_SERVERR2) == 0)
			strOsName = L"Windows Server 2003 (v5.2)";//2003年3月发布 
		else if (IsWinServer(5, 2) && GetSystemMetrics(SM_SERVERR2) != 0)
			strOsName = L"Windows Server 2003 R2 (v5.2)";
	}
	else if (IsWinVerEqualTo(6, 0))
	{
		wMajor = 6; wMinor = 0;
		if (IsWinServer(6, 0))
			strOsName = L"Windows Server 2008 (v6.0)";//服务器版本 
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
			strOsName = L"Windows Server 2016 (v10.0)";//服务器版本 
		else
			strOsName = L"Windows 10 (v10.0)";
	}
	else
	{
		strOsName = L"UNKNOMN Windows Version";
	}

	//LRESULT lResult = MAKEWPARAM(wLow,wHigh);  //合成LRESULT参数的值，WPARAM即DWORD
	return MAKEWPARAM(wMajor, wMinor);
}

/*
什么是vs 程序的manifest文件
manifest 是VS程序用来标明所依赖的side-by-side组建,如ATL, CRT等的清单。

为什么要有manifest文件
一台pc上，用一组建往往会有不止一个版本（c:/windows/winsxs或系统目录下），程序在加载的时候，不知加载哪个，
于是manifest文件来指明。

manifest在哪儿，如何创建。
如果用VS开发，可以Set通过porperty->configuration properties->linker->manifest file->Generate manifest To Yes
来自动创建manifest来指定系统的和CRT的assembly版本。

除了这样产生外部的manifest file，还有embedded manifest信息可以被写到所生成的二进制文件内
Set porperty->configuration properties->manifest tool->embed manifest  To Yes

对于xp及早前的windows版本，external manifest会比embed manifest有更高的优先级，
但对于windows server及后的版本，相反。

在 Visual Studio 开发环境中设置此链接器选项
打开项目的“属性页”对话框。 有关详细信息，请参见如何：打开项目属性页。
展开“配置属性”节点。
展开“链接器”节点。
选择“清单文件”属性页。
修改“生成清单”属性。
*/

//可以在项目路径下手动创建一个UTF8格式的文件，将需要加入的内容粘贴进去，再将该文件加入项目即可。比如：
//g:\MyVC2017\noMeiryoUI235\noMeiryoUI235\res\noMeiryoUI235.exe.manifest

//当然如果不更改manifest文件，Win8.1和Win10 返回的版本号还是6.2，
//只需在<application></application> 中添加各系统ID即可，如下所示：
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

//获取Windows系统版本-End
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//ini文件处理函数-Begin
//private 供getSectionCount和getAllSections调用
void myGetPrivateProfileSectionNames(wchar_t** buf, int bufLen, CString& iniFilePath)
{
	DWORD dwRes = GetPrivateProfileSectionNames(*buf, bufLen, iniFilePath);
	//缓冲区过小,重新设置缓冲区
	while (dwRes == bufLen - 2)
	{
		delete[]buf;
		bufLen += 1024;
		*buf = new wchar_t[bufLen];
		dwRes = GetPrivateProfileSectionNames(*buf, bufLen, iniFilePath);
	}
}

//获取所有的section名
void getAllSections(vector<CString>& vecSections, CString& iniFilePath)
{
	int _bufLen = 1024;
	vecSections.clear();

	wchar_t* buf = new wchar_t[_bufLen];
	myGetPrivateProfileSectionNames(&buf, _bufLen, iniFilePath);

	/*
	**通过string来取出每个section的依据
	**GetPrivateProfileSectionNames取出的sections以\0间隔，存于buf中
	**string遇见\0就终止
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

//判断是否存在指定的section
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
//ini文件处理函数-End
//////////////////////////////////////////////////////////////////////////
BOOL isEngChar(wchar_t w) {
	if (!(w >= 'A' && w <= 'Z') && !(w >= 'a' && w <= 'z')) {
			return false;
	}
	return true;
}