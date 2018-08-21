// AgileFontSet.h

#include "stdafx.h"
//#include "CommonFun.h"		//PP1_FontSet.cpp中已经包含，这里再包含就是重复包含。只能以extern外部引用
#include "PP0_PropertySheet.h"

//CString仅用了(LPCWSTR)不需要ReleaseBuffer()，用了(LPTSTR)或(wchar_t*)或(char*)后才需要ReleaseBuffer()
//但作为一种良好编程习惯，以后凡必须用CString-GetBuffer、LPTSTR、LPSTR、或LPWSTR的地方，用完后都立即ReleaseBuffer()
extern CString g_strVerInfo;
extern CString StrToLower(const CString& str);
extern int CStringSplitN(vector<CString>& vecResult, const CString& str, CString strSep);	//返回字段数

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
int GetDataEx(vector<CString> &vecStrIS, vector<int> &vecIntIS, CString strFlag)
{
	int nRet = 0;	//参数非法、或者转换失败

	if (L"" == strFlag && L"-h" == StrToLower(vecStrIS[0].Left(2)) && L"-v" == StrToLower(vecStrIS[1].Left(2)))
	{
		CString str0 = vecStrIS[0].Right(vecStrIS[0].GetLength() - 2);	//去除前面两个字符
		vecIntIS[0] = myStoi(str0);
		//vecIconSpacing.push_back(_wtoi(str0.GetBuffer(str0.GetLength())));
		//vecIconSpacing[0] = _wtoi((LPWSTR)(LPCWSTR)vecCmdLine[0]);			//转换失败
		//str0.ReleaseBuffer();

		CString str1 = vecStrIS[1].Right(vecStrIS[1].GetLength() - 2);	//去除前面两个字符
		vecIntIS[1] = myStoi(str1);
		//vecIconSpacing.push_back(_wtoi(str1.GetBuffer(str1.GetLength())));
		//vecIconSpacing[1] = _wtoi((LPWSTR)(LPCWSTR)vecCmdLine[1]);			//转换失败
		//vecCmdLine[1].ReleaseBuffer();

		// 若 str 不能转换成 int、或者结果不在 0-150之间，myStoi 将返回 -1
		if (-1 != vecIntIS[0] && -1 != vecIntIS[1])
		{
			nRet = 1;
		}	//转换成功
	}
	else if (L"-h" == strFlag && L"-h" == StrToLower(vecStrIS[0].Left(2)))
	{
		CString str0 = vecStrIS[0].Right(vecStrIS[0].GetLength() - 2);	//去除前面两个字符
		vecIntIS[0] = myStoi(str0);
		// 若 str 不能转换成 int、或者结果不在 0-150之间，myStoi 将返回 -1
		if (-1 != vecIntIS[0])
		{
			nRet = 1;
		}	//转换成功
	}
	else if (L"-v" == strFlag && L"-v" == StrToLower(vecStrIS[1].Left(2)))
	{
		CString str1 = vecStrIS[1].Right(vecStrIS[1].GetLength() - 2);	//去除前面两个字符
		vecIntIS[1] = myStoi(str1);
		// 若 str 不能转换成 int、或者结果不在 0-150之间，myStoi 将返回 -1
		if (-1 != vecIntIS[1])
		{
			nRet = 1;
		}	//转换成功
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
	int iSeg = 0;	//命令行参数成功分割的段数
					// 获取命令行参数
	CString strCmdLine = lpCmdLine;
	// 删除前后空白符
	strCmdLine = CTrimQ(strCmdLine);


	// 处理命令行参数
	vector<CString> vecCmdLine;
	vector<int> vecIntIS(2, -1);	//(vecIntIconSpacing)初始化了2个默认值为-1的元素。避免出错时误设置
	PP0_PropertySheet progsheet(g_strVerInfo.GetBuffer(0), 0);
	g_strVerInfo.ReleaseBuffer();

	//用CStringSplitS分割 v -h3 时出错。所以用CStringSplitN
	iSeg = CStringSplitN(vecCmdLine, strCmdLine, ' ');	//strCmdLine中的参数段可不分先后次序

														//这样处理更清晰、易于理解和管理，不容易出错
														// (vecStrIconSpacing)，初始化为5个L""，0-4单元分别用于存放：-Hxxx、-Vxxx、-N、-R、-?
	vector<CString> vecStrIS(5, L"");

	//遍历vecCmdLine中的参数段：
	//	取出L"-n"设置bRefresh；
	//	取出L"-?"设置bHelp；
	//	取出L"-h"、L"-v"存入vecIS[0]、vecIS[1]
	for (auto str : vecCmdLine)
	{
		//错误做法：删除vecCmdLine所有元素中的空格，这将在有空格的地方存入0、切断后面的字符
		//str.Replace(' ', NULL);
		//str.Replace(' ', '');	//提示错误：空字符常量
		str.Replace(L" ", L"");	//正确做法：删除vecCmdLine所有元素中的空格

								//C++ switch (x) 语句句后面的表达式x可以是整型、字符型、枚举型。
								//WTL的 switch (x) 语句的表达式x，必须是整型或枚举类型，字符型报错

								//switch (str.GetBuffer(0)) {default: break; }
								//iconspacing.h(210) : error C2450 : switch expression of type 'wchar_t *' is illegal
								//iconspacing.h(210) : note: Integral expression required

								//switch (str) {default: break; }
								//iconspacing.h(214) : error C2450 : switch expression of type 'ATL::CStringT<wchar_t,ATL::StrTraitATL<wchar_t,ATL::ChTraitsCRT<wchar_t>>>' is illegal
								//iconspacing.h(214) : note: No user - defined - conversion operator available that can perform this conversion, or the operator cannot be called

		if (L"-h" == StrToLower(str.Left(2)))
		{	//当有两个L"-h"参数时，只用前面一个参数
			if (L"" == vecStrIS[0]) vecStrIS[0] = str;
			else { iSeg = -1; break; }	//同样参数出现两次
		}
		else if (L"-v" == StrToLower(str.Left(2)))
		{	//当有两个L"-v"参数时，只用前面一个参数
			if (L"" == vecStrIS[1]) vecStrIS[1] = str;
			else { iSeg = -1; break; }	//同样参数出现两次
		}
		else if (L"-n" == StrToLower(str))
		{
			if (L"" == vecStrIS[2]) vecStrIS[2] = str;
			else { iSeg = -1; break; }	//同样参数出现两次
		}
		else if (L"-r" == StrToLower(str))
		{
			if (L"" == vecStrIS[3]) vecStrIS[3] = str;
			else { iSeg = -1; break; }	//同样参数出现两次
		}
		else if (L"-?" == StrToLower(str))
		{
			if (L"" == vecStrIS[4]) vecStrIS[4] = str;
			else { iSeg = -1; break; }	//同样参数出现两次
		}
		else
		{
			iSeg = -1; break;
		}			//出现不属于上面的5种合法参数，报错。并跳出for循环
	}

	// (vecStrIconSpacing)，初始化为5个L""，0-4单元分别用于存放：-Hxxx、-Vxxx、-N、-R、-?
	//vector<CString> vecStrIS(5, L"");

	//这样处理更清晰、易于理解和管理，不容易出错
	if (-1 == iSeg)
	{	//参数非法、或参数越界报错
		MessageBox(NULL, L"  给定的参数不合法，或者图标间距超出0-150的范围。\r\n\
  图标间距是0到150之间的3位正整数(Win7风格度量单位)。\r\n\
  \r\n\
  请运行 \"IconSpacing.exe -?\" 获得帮助信息.\r\n",
			L"参数非法或图标间距超出范围", MB_OK | MB_ICONINFORMATION);
	}
	else if (0 == iSeg)
	{	// 参数为空，显示设置对话框和帮助信息(当前选项卡)
		nRet = progsheet.DoModal();
	}
	else if (1 == iSeg && L"-?" == vecStrIS[4])	//本可以放在后面处理，提前处理提高效率
	{	// 参数为一段且为 L"-?" ，显示设置对话框(当前选项卡)和帮助信息
		progsheet.SetActivePage(1);	//设置属性表单出现时的当前选项卡
		nRet = progsheet.DoModal();
	}
	else if (1 == iSeg || 2 == iSeg || 3 == iSeg)// 参数为1到3段
	{
		int iState = 0;	//iState = 0 表示参数非法或越界

		if (L"-r" == StrToLower(vecStrIS[3].Left(3)))
		{	//只有L"-r"，只刷新桌面
			//bRefresh = TRUE;	//前面定义时已付默认初值TRUE
			if (1 == iSeg)		//L"-r"必须是独立参数
			{
				iState = progsheet.GetIconSpacing(vecIntIS);		//获取当前图标间距
			}
		}
		else if (L"-h" == StrToLower(vecStrIS[0].Left(2)) && L"" == vecStrIS[1])
		{	//只有L"-h"、[L"-n"]。此时vecIntIS[0]、vecIntIS[1]都为-1
			progsheet.GetIconSpacing(vecIntIS);		//获取当前图标间距
			iState = GetDataEx(vecStrIS, vecIntIS, L"-h");
		}
		else if (L"-v" == StrToLower(vecStrIS[1].Left(2)) && L"" == vecStrIS[0])
		{	//只有L"-v"、[L"-n"]
			progsheet.GetIconSpacing(vecIntIS);		//获取当前图标间距
			iState = GetDataEx(vecStrIS, vecIntIS, L"-v");
		}
		else if (L"-h" == StrToLower(vecStrIS[0].Left(2)) && L"-v" == StrToLower(vecStrIS[1].Left(2)))
		{	//同时有L"-h"、L"-v"、[L"-n"]
			iState = GetDataEx(vecStrIS, vecIntIS, L"");
		}

		if (1 == iState)
		{	//应用设置，刷新桌面
			progsheet.SetIconSpacing(vecIntIS[0], vecIntIS[1], L"-n" != StrToLower(vecStrIS[2].Left(2)));
		}
		else
		{	//参数非法、或参数越界报错
			MessageBox(NULL, L"  给定的参数不合法，或者图标间距超出0-150的范围。\r\n\
  图标间距是0到150之间的3位正整数(Win7风格度量单位)。\r\n\
  \r\n\
  请运行 \"IconSpacing.exe -?\" 获得帮助信息.\r\n",
				L"参数非法或图标间距超出范围", MB_OK | MB_ICONINFORMATION);
		}
	}
	else
	{	//参数处理失败和其余情况，都显示设置对话框(当前选项卡)和帮助信息
		progsheet.SetActivePage(1);	//设置属性表单出现时当前选项卡
		nRet = progsheet.DoModal();
	}

	//		MessageBox(NULL, L"  IconSpacing is illegal or out of range.\r\n\
		  //IconSpacing is 3-digit integer between 15 and 150 (Windows 7 metric).\r\n\
  //\r\n\
  //Please run \"IconSpacing.exe -?\" to display help message.\r\n",
//			L"IconSpacing is illegal or out of range", MB_OK);

//switch (nRet)
//{
//case 2:		//成功设置，直接跳出
//	break;
//case 4:		//参数非法、或参数越界报错
//	MessageBox(NULL, L"  IconSpacing is illegal or out of range.\r\n\
 // IconSpacing is 3-digit integer between 15 and 150 (Windows 7 metric).\r\n\
 // \r\n\
 // Please run \"IconSpacing.exe -?\" to display help message.\r\n",
//		L"IconSpacing is illegal or out of range", MB_OK);
//	break;
//default:		//处理失败和其余情况都显示Help信息
//				//IconSpacing_Help iconSpacing_Help;
//				//iconSpacing_Help.CenterWindow();	//该处报错终止，设置窗口属性Center为True解决
//				//nRet = (int)iconSpacing_Help.DoModal();

//	PP0_PropertySheet progsheet(L"ChromePortable.ini 设置界面 - by ybmj@vip.163.com", 0);
//	int nRet = progsheet.DoModal();

//	break;
//}

	return nRet;
}
