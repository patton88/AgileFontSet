// AgileFontSet.h

#include "stdafx.h"
//#include "CommonFun.h"		//PP1_FontSet.cpp中已经包含，这里再包含就是重复包含。只能以extern外部引用
#include "PP0_PropertySheet.h"

//CString仅用了(LPCWSTR)不需要ReleaseBuffer()，用了(LPTSTR)或(wchar_t*)或(char*)后才需要ReleaseBuffer()
//但作为一种良好编程习惯，以后凡必须用CString-GetBuffer、LPTSTR、LPSTR、或LPWSTR的地方，用完后都立即ReleaseBuffer()
extern CString g_strVerInfo;
extern CString StrToLower(const CString& str);
extern int CStringSplitN(vector<CString>& vecResult, const CString& str, CString strSep);	//返回字段数
extern CString getCurDir(int iFlag);
extern BOOL isEngChar(wchar_t w);
extern bool isSectionExists(CString sectionName, CString& iniFilePath);

bool SetIconSpacing0()
{
	//PP0_PropertySheet progsheet(L"ChromePortable.ini 设置界面 - by ybmj@vip.163.com", 0);
	PP0_PropertySheet progsheet(g_strVerInfo.GetBuffer(0), 0);
	g_strVerInfo.ReleaseBuffer();
	int nRet = progsheet.DoModal();

	return true;
}

//判断字符串是否都是数字
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
头文件：#include <stdlib.h>

atoi() 函数用来将字符串转换成整数(int)，其原型为：
int atoi (const char * str);

【函数说明】atoi() 函数会扫描参数 str 字符串，跳过前面的空白字符（例如空格，tab缩进等，
可以通过 isspace() 函数来检测），直到遇上数字或正负符号才开始做转换，
而再遇到非数字或字符串结束时('\0')才结束转换，并将结果返回。
【返回值】返回转换后的整型数；如果 str 不能转换成 int 或者 str 为空字符串，那么将返回 0。

温馨提示：
ANSI C 规范定义了 stof()、atoi()、atol()、strtod()、strtol()、strtoul()
共6个可以将字符串转换为数字的函数，大家可以对比学习。
另外在 C99 / C++11 规范中又新增了5个函数，
分别是 atoll()、strtof()、strtold()、strtoll()、strtoull()，在此不做介绍，请大家自行学习。

范例：将字符串a 与字符串b 转换成数字后相加。
*/

//10第n次方
int myPower10(int n)
{
	int power = 1;
	for (int i = 0; i < n; i++) { power *= 10; }
	return power;
}

//字符数字转正整数
int myWCtoInt(wchar_t w)
{	// '0'为48
	w = w - 48;
	return w;
}

//简单的三位0-150的字符数值转换为正整数。
//成功返回0-150的正整数，失败返回-1
int myStoi(CString str)
{
	int iRet = -1;
	int volue = 0;
	int iLen = str.GetLength();
	if (iLen > 0 && iLen <= 3)
	{
		int i;
		for (i = 0; i < iLen; i++)
		{	// '0'为48，'9'为57
			if (str[i] < 48 || str[i] > 57) { break; }
			volue += myWCtoInt(str[i]) * myPower10(iLen - i - 1);
		}

		if (i == iLen && (volue >= 0 && volue <= 150)) { iRet = volue; }
	}

	return iRet;
}

//int GetData(vector<CString> &vecStrIS, vector<int> &vecIntIS)
//{
//	int nRet = 0;	//参数多为、或者转换失败
//
//	if (L"-h" == StrToLower(vecStrIS[0].Left(2)) && L"-v" == StrToLower(vecStrIS[1].Left(2)))
//	{
//		CString str0 = vecStrIS[0].Right(vecStrIS[0].GetLength() - 2);	//去除前面两个字符
//		vecIntIS[0] = myStoi(str0);
//		//vecIconSpacing.push_back(_wtoi(str0.GetBuffer(str0.GetLength())));
//		//vecIconSpacing[0] = _wtoi((LPWSTR)(LPCWSTR)vecCmdLine[0]);			//转换失败
//		//str0.ReleaseBuffer();
//
//		CString str1 = vecStrIS[1].Right(vecStrIS[1].GetLength() - 2);	//去除前面两个字符
//		vecIntIS[1] = myStoi(str1);
//		//vecIconSpacing.push_back(_wtoi(str1.GetBuffer(str1.GetLength())));
//		//vecIconSpacing[1] = _wtoi((LPWSTR)(LPCWSTR)vecCmdLine[1]);			//转换失败
//		//vecCmdLine[1].ReleaseBuffer();
//
//		// 若 str 不能转换成 int、或者结果不在 0-150之间，myStoi 将返回 -1
//		if (-1 != vecIntIS[0] && -1 != vecIntIS[1])
//		{
//			nRet = 1;
//		}	//转换成功
//	}
//
//	return nRet;
//}

//vecStrIS(vecStrIconSpacing)、vecIntIS(vecIntIconSpacing)
int GetDataEx(vector<CString> &vecStrIS, vector<unsigned> &vecUnIS, CString strFlag)
{
	int nRet = 0;	//参数非法、或者转换失败

	if (L"" == strFlag && L"-h" == StrToLower(vecStrIS[0].Left(2)) && L"-v" == StrToLower(vecStrIS[1].Left(2)))
	{
		CString str0 = vecStrIS[0].Right(vecStrIS[0].GetLength() - 2);	//去除前面两个字符
		vecUnIS[0] = myStoi(str0);
		//vecIconSpacing.push_back(_wtoi(str0.GetBuffer(str0.GetLength())));
		//vecIconSpacing[0] = _wtoi((LPWSTR)(LPCWSTR)vecCmdLine[0]);			//转换失败
		//str0.ReleaseBuffer();

		CString str1 = vecStrIS[1].Right(vecStrIS[1].GetLength() - 2);	//去除前面两个字符
		vecUnIS[1] = myStoi(str1);
		//vecIconSpacing.push_back(_wtoi(str1.GetBuffer(str1.GetLength())));
		//vecIconSpacing[1] = _wtoi((LPWSTR)(LPCWSTR)vecCmdLine[1]);			//转换失败
		//vecCmdLine[1].ReleaseBuffer();

		// 若 str 不能转换成 int、或者结果不在 0-150之间，myStoi 将返回 -1
		if (-1 != vecUnIS[0] && -1 != vecUnIS[1])
		{
			nRet = 1;
		}	//转换成功
	}
	else if (L"-h" == strFlag && L"-h" == StrToLower(vecStrIS[0].Left(2)))
	{
		CString str0 = vecStrIS[0].Right(vecStrIS[0].GetLength() - 2);	//去除前面两个字符
		vecUnIS[0] = myStoi(str0);
		// 若 str 不能转换成 int、或者结果不在 0-150之间，myStoi 将返回 -1
		if (-1 != vecUnIS[0])
		{
			nRet = 1;
		}	//转换成功
	}
	else if (L"-v" == strFlag && L"-v" == StrToLower(vecStrIS[1].Left(2)))
	{
		CString str1 = vecStrIS[1].Right(vecStrIS[1].GetLength() - 2);	//去除前面两个字符
		vecUnIS[1] = myStoi(str1);
		// 若 str 不能转换成 int、或者结果不在 0-150之间，myStoi 将返回 -1
		if (-1 != vecUnIS[1])
		{
			nRet = 1;
		}	//转换成功
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
	int iSeg = 0;	//命令行参数成功分割的段数
					// 获取命令行参数
	CString strCmdLine = lpCmdLine;
	// 删除前后空白符
	strCmdLine = CTrimQ(strCmdLine);

	CString strErroe = L"  给定的参数不合法。\r\n\
  \r\n\
  可以不带参数直接启动窗口界面\r\n\
  可给定ini配置文件路径(含空格的路径必须用双引号包围)，比如：D:\\myFontSet.ini\r\n\
  可给定ini配置文件路径和 -xxx 参数选择ini文件中的xxx配置，比如：D:\\myFontSet.ini -xxx\r\n\
  可给定ini配置文件路径和 -hide 参数进行后台设置，比如：D:\\myFontSet.ini -hide\r\n\
  可给定ini配置文件路径和 -xxx -hide 参数后台设置ini文件中的xxx配置，比如：D:\\myFontSet.ini -xxx -hide\r\n\
  可给定 -? 参数查看帮助信息，比如：AgileFontSet.exe -?\r\n\
  所有参数都不分大小写\r\n";

	// 处理命令行参数
	vector<CString> vecCmdLine;
	PP0_PropertySheet progsheet(g_strVerInfo.GetBuffer(0), 0);
	g_strVerInfo.ReleaseBuffer();

	//用CStringSplitS分割 v -h3 时出错。所以用CStringSplitN
	iSeg = CStringSplitN(vecCmdLine, strCmdLine, ' ');	//strCmdLine中的参数段可不分先后次序

	//这样处理更清晰、易于理解和管理，不容易出错
	//初始化为4个L""，0-3单元分别用于存放：path、-xxx、-hide、-?
	vector<CString> vecStrCmd(4, L"");

	//遍历vecCmdLine中的参数段：
	for (auto str : vecCmdLine)
	{
		CTrimQ(str);		//去除str两段空格

		if (L".ini" == StrToLower(str.Right(4)) || L".ini\"" == StrToLower(str.Right(5)))
		{	//当有两个L".ini"参数时，只用前面一个参数
			if (L"" == vecStrCmd[0]) {
				if(L'\"' == str[0]){
					str.Replace(L"\"", L"");		//正确做法：删除str中的双引号
					CTrimQ(str);
				}
				if (L':' != str[1])		//判断Path第2个字符非L':'，便非绝对路径，加上m_strCurrentDir
				{
					str = getCurDir(2) + str;
				}
				vecStrCmd[0] = str;
			}
			else { iSeg = -1; break; }	//同样参数出现两次
		}
		else if (L"-hide" == StrToLower(str))
		{	//当有两个L"-hide"参数时，只用前面一个参数
			if (L"" == vecStrCmd[2]) vecStrCmd[2] = str;
			else { iSeg = -1; break; }	//同样参数出现两次
		}
		else if (L"-?" == str)
		{	//当有两个L"-?"参数时，只用前面一个参数
			if (L"" == vecStrCmd[3]) vecStrCmd[3] = str;
			else { iSeg = -1; break; }	//同样参数出现两次
		}
		else if (str.GetLength() > 1 && L'-' == str[0] && isEngChar(str[1]))
		{	//当有两个L"-xxx"参数时，只用前面一个参数
			if (L"" == vecStrCmd[1]) vecStrCmd[1] = str;
			else { iSeg = -1; break; }	//同样参数出现两次
		}
		else
		{
			iSeg = -1; break;
		}	//出现不属于上面的几种合法参数，报错。并跳出for循环
	}

	//这样处理更清晰、易于理解和管理，不容易出错
	if (-1 == iSeg)
	{	//参数非法
		MessageBox(NULL, strErroe, L"参数非法", MB_OK | MB_ICONINFORMATION);
	}
	else if (0 == iSeg)
	{	// 参数为空，显示设置对话框和帮助信息(当前选项卡)
		nRet = progsheet.DoModal();
	}
	else if (1 == iSeg && L"-?" == vecStrCmd[3])	//本可以放在后面处理，提前处理提高效率
	{	// 参数为一段且为 L"-?" ，显示设置对话框(当前选项卡)和帮助信息
		progsheet.SetActivePage(1);	//设置属性表单出现时的当前选项卡
		nRet = progsheet.DoModal();
	}
	// 参数为1段path、或者2段path -hide、或者3段path -xxx -hide
	else if ((1 == iSeg || 2 == iSeg || 3 == iSeg) && !vecStrCmd[0].IsEmpty())
	{
		int iState = 0;	//iState = 0 表示参数非法或越界

		//用_waccess(需包含io.h)代替fopen判断文件是否存在，用fopen若文件不可读会误判
		if (-1 == _waccess(vecStrCmd[0], 0))	//文件存在_waccess返回0，否则返回-1
		{
			::MessageBox(NULL, L"配置文件 " + vecStrCmd[0] + L" 文件不存在。", L"配置文件不存在", MB_OK | MB_ICONINFORMATION);
			return false;
		}

		if (FALSE == progsheet.m_pp1FontSet.loadFontInfo(vecStrCmd[0])) {
			::MessageBox(NULL, L"无法加载字体配置文件 " + vecStrCmd[0], L"错误", MB_OK | MB_ICONEXCLAMATION);
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
		else if (3 == iSeg && !vecStrCmd[1].IsEmpty() && L"-hide" == StrToLower(vecStrCmd[2])) // 参数为3段path -xxx -hide
		{
			if (isSectionExists(vecStrCmd[1], vecStrCmd[0]))
			{
				::MessageBox(NULL, vecStrCmd[0] + L"文件中不存在配置：" + vecStrCmd[1], L"错误", MB_OK | MB_ICONEXCLAMATION);
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
	{	//参数处理失败和其余情况，都显示设置对话框(当前选项卡)和帮助信息
		progsheet.SetActivePage(1);	//设置属性表单出现时当前选项卡
		nRet = progsheet.DoModal();
	}

	return nRet;
}
