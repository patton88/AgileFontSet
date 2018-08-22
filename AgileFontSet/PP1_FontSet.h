// PP3_FontSet dialog

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "CEditImpl.h"
#include "CButtonImpl.h"
#include "CComboImpl.h"
#include <vector>
#include <map>
#include <utility>
using std::vector; using std::map; using std::pair; using std::make_pair;

class PP1_FontSet :
	public CPropertyPageImpl<PP1_FontSet>,
	public CWinDataExchange<PP1_FontSet>,
	public CDialogResize<PP1_FontSet>
{
public:
	enum { IDD = IDD_PP1_FontSet };

	// Construction
	PP1_FontSet();

// this is the user-defined message
#define WM_SETPAGEFOCUS17 WM_APP + 11//必须响应此消息才能调用OnSetPageFocus()禁用启用按钮

	//处理ID在IDC_MONDAY和IDC_FRIDAY之间的控件发送的BN_CLICKED通知消息。
	//COMMAND_RANGE_CODE_HANDLER_EX(IDC_MONDAY, IDC_FRIDAY, BN_CLICKED, OnDayClicked)

	// Maps
	BEGIN_MSG_MAP(PP1_FontSet)
		MSG_WM_INITDIALOG(OnInitDialog)
		MESSAGE_HANDLER(WM_SETPAGEFOCUS17, OnSetPageFocus)//必须响应此消息才能调用OnSetPageFocus()禁用启用按钮

		COMMAND_ID_HANDLER(IDM_COMPAT7, OnCompat7)

		COMMAND_ID_HANDLER(IDC_CHECK_ALLFONT, OnCheckAllFont)

		//解决在未启用Theme的机器上只读CEdit控件的背景为灰色的问题，测试有效
		//MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)			//普通的CEdit
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEditOnly)		//只读状态的CEdit

		//处理字体选择按键事件
		//COMMAND_RANGE_CODE_HANDLER_EX(idFirst, idLast, code, func)
		//处理ID范围内的控件的特定的通知。
		COMMAND_RANGE_CODE_HANDLER_EX(IDB_SEL_ALLFONT, IDB_SEL_TIP, BN_CLICKED, OnSelFont)
		//监测文本获得焦点
		COMMAND_RANGE_CODE_HANDLER_EX(IDC_EDIT_ALLFONT, IDC_EDIT_TIP, EN_SETFOCUS, OnEnSetfoucsEdit)

		//组合框按钮响应
		COMMAND_HANDLER(IDC_COMBO_PRECONFIT, CBN_SELCHANGE, OnSelchangeCombo)

		CHAIN_MSG_MAP(CPropertyPageImpl<PP1_FontSet>)
		CHAIN_MSG_MAP(CDialogResize<PP1_FontSet>)
	END_MSG_MAP()

	//注意：要实现标签控件自动缩放，必须并为其指定独立ID号
	BEGIN_DLGRESIZE_MAP(PP1_FontSet)
		//DLGRESIZE_CONTROL(IDC_TEXT1, DLSZ_SIZE_X)
		//DLGRESIZE_CONTROL(IDC_EDIT_AllSetText, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		//DLGRESIZE_CONTROL(IDC_BUTTON1, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		//DLGRESIZE_CONTROL(IDCANCEL, DLSZ_MOVE_X)
		//DLGRESIZE_CONTROL(IDC_TREE, DLSZ_SIZE_X)
		//DLGRESIZE_CONTROL(IDC_RELATIONSHIP_TREE, DLSZ_SIZE_X | DLSZ_SIZE_Y)

		//DLGRESIZE_CONTROL(IDC_STATIC_PRECONFIT, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		//DLGRESIZE_CONTROL(IDC_STATIC_ALLFONT, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		//DLGRESIZE_CONTROL(IDC_STATIC_TITLE, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		//DLGRESIZE_CONTROL(IDC_STATIC_ICON, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		//DLGRESIZE_CONTROL(IDC_STATIC_MENU, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		//DLGRESIZE_CONTROL(IDC_STATIC_MESSAGE, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		//DLGRESIZE_CONTROL(IDC_STATIC_PALETTE, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		//DLGRESIZE_CONTROL(IDC_STATIC_TIP, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		//DLGRESIZE_CONTROL(IDC_STATIC_ICON_HS, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		//DLGRESIZE_CONTROL(IDC_STATIC_ICON_VS, DLSZ_SIZE_X | DLSZ_SIZE_Y)

		DLGRESIZE_CONTROL(IDC_STATIC_VERNO, DLSZ_SIZE_X | DLSZ_SIZE_Y)

		DLGRESIZE_CONTROL(IDC_GROUP_ICONSPACING_SET, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_GROUP_FONTSET, DLSZ_SIZE_X | DLSZ_SIZE_Y)

		DLGRESIZE_CONTROL(IDC_COMBO_PRECONFIT, DLSZ_SIZE_X | DLSZ_SIZE_Y)

		DLGRESIZE_CONTROL(IDC_EDIT_ALLFONT, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_EDIT_TITLE, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_EDIT_ICON, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_EDIT_MENU, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_EDIT_MESSAGE, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_EDIT_PALETTE, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_EDIT_TIP, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_EDIT_HS, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_EDIT_VS, DLSZ_SIZE_X | DLSZ_SIZE_Y)

		DLGRESIZE_CONTROL(IDB_SEL_ALLFONT, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDB_SEL_TITLE, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDB_SEL_ICON, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDB_SEL_MENU, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDB_SEL_MESSAGE, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDB_SEL_PALETTE, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDB_SEL_TIP, DLSZ_MOVE_X | DLSZ_MOVE_Y)

		DLGRESIZE_CONTROL(IDC_CHECK_ALLFONT, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_CHECK_TITLE, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_CHECK_ICON, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_CHECK_MENU, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_CHECK_MESSAGE, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_CHECK_PALETTE, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_CHECK_TIP, DLSZ_MOVE_X | DLSZ_MOVE_Y)

		DLGRESIZE_CONTROL(IDC_CHECK_HS, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_CHECK_VS, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_SPIN_HS, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_SPIN_VS, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_ICON_HS, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_ICON_VS, DLSZ_MOVE_X | DLSZ_MOVE_Y)
	END_DLGRESIZE_MAP()

	BEGIN_DDX_MAP(PP1_FontSet)
		//DDX_CHECK(IDC_CHECK_REFRESH, m_iRefresh)		// DDX_CHECK只能与int类型变量交换数据
		DDX_CONTROL_HANDLE(IDC_COMBO_PRESET, m_comboPreSet);

		DDX_CONTROL(IDC_CHECK_ALLFONT, m_chkAllfont)
		DDX_CONTROL(IDC_CHECK_TITLE, m_chkTitle)
		DDX_CONTROL(IDC_CHECK_ICON, m_chkIcon)
		DDX_CONTROL(IDC_CHECK_MENU, m_chkMenu)
		DDX_CONTROL(IDC_CHECK_MESSAGE, m_chkMessage)
		DDX_CONTROL(IDC_CHECK_PALETTE, m_chkPalette)
		DDX_CONTROL(IDC_CHECK_TIP, m_chkTip)
		DDX_CONTROL(IDC_CHECK_HS, m_chkHS)
		DDX_CONTROL(IDC_CHECK_VS, m_chkVS)

		DDX_CHECK(IDC_CHECK_ALLFONT, m_iCheckAllfont)
		DDX_CHECK(IDC_CHECK_TITLE, m_iCheckTitle)
		DDX_CHECK(IDC_CHECK_ICON, m_iCheckIcon)
		DDX_CHECK(IDC_CHECK_MENU, m_iCheckMenu)
		DDX_CHECK(IDC_CHECK_MESSAGE, m_iCheckMessage)
		DDX_CHECK(IDC_CHECK_PALETTE, m_iCheckPalette)
		DDX_CHECK(IDC_CHECK_TIP, m_iCheckTip)
		DDX_CHECK(IDC_CHECK_HS, m_iCheckHS)
		DDX_CHECK(IDC_CHECK_VS, m_iCheckVS)

		DDX_CONTROL(IDC_EDIT_ALLFONT, m_editAllFont)
		DDX_CONTROL(IDC_EDIT_TITLE, m_editTitleFont)
		DDX_CONTROL(IDC_EDIT_ICON, m_editIconFont)
		DDX_CONTROL(IDC_EDIT_MENU, m_editMenuFont)
		DDX_CONTROL(IDC_EDIT_MESSAGE, m_editMessageFont)
		DDX_CONTROL(IDC_EDIT_PALETTE, m_editPaletteFont)
		DDX_CONTROL(IDC_EDIT_TIP, m_editTipFont)

		DDX_TEXT(IDC_EDIT_ALLFONT, m_strAllFontName)
		DDX_TEXT(IDC_EDIT_TITLE, m_strTitleFontName)
		DDX_TEXT(IDC_EDIT_ICON, m_strIconFontName)
		DDX_TEXT(IDC_EDIT_MENU, m_strMenuFontName)
		DDX_TEXT(IDC_EDIT_MESSAGE, m_strMessageFontName)
		DDX_TEXT(IDC_EDIT_PALETTE, m_strPaletteFontName)
		DDX_TEXT(IDC_EDIT_TIP, m_strTipFontName)
	END_DDX_MAP()

	//自定义类型前置申明
	enum fontType;
	struct TagFontOld;
	struct TagIS;

	// Message handlers
	BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam);
	BOOL OnSetActive();

	//监测文本改变
	LRESULT OnEnChangeEdit(UINT uNotifyCode, int nID, CWindow wndCtl);

	//监测文本获得焦点
	//LRESULT OnEnSetfoucsEdit(UINT uNotifyCode, int nID, CWindow wndCtl);

	//LONG OnSetPageFocus(UINT wParam, LONG lParam);
	LRESULT OnSetPageFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCheckAllFont(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelchangeCombo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	// DDX variables
	CString m_strChrome;
	CEditImpl m_editChrome;
	BOOL m_bReInit;

	//Windows内建控件CSpinButtonCtrl的WTL封装类为：CUpDownCtrl
	CUpDownCtrl m_spinHS;
	CUpDownCtrl m_spinVS;
	CEditImpl m_editHS;
	CEditImpl m_editVS;

	//测试EN_CHANGE消息响应
	CUpDownCtrl m_spin1;
	CEditImpl m_edit1;
	CComboImpl m_comboPreSet;
	int m_nComboCurSel;

	unsigned m_nOldHS;
	unsigned m_nOldVS;

	//添加自noMeiryoUI235
	LRESULT OnSet(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSetExit(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSetAll(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSelFont(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/);
	//LRESULT OnSelFont(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	//func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam);
	LRESULT OnSave(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnLoad(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAbout(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnHelpTopic(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSet8(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSet10(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSetCurrent(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCompat7(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnUniqThread(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	//修改只读CEdit的颜色
	LRESULT OnCtlColorEditOnly(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//成功解决当CEdit获得焦点时去掉全选，光标定位到起点或终点
	//监测文本获得焦点。响应成功
	LRESULT OnEnSetfoucsEdit(UINT uNotifyCode, int nID, CWindow wndCtl);

	void getActualFont(void);
	void theUpdateDisplay(void);
	int getFontPointInt(LOGFONTW *font, HWND hWnd);
	double getFontPoint(LOGFONTW *font, HWND hWnd);
	HFONT createFont(LOGFONTW *font);
	BOOL IsCtrl(HWND hWnd, CString str);
	void theSetFont(NONCLIENTMETRICSW *fontMetrics, LOGFONTW *iconLogFont);
	BOOL startSaveFont(CString filename);
	BOOL saveFont(CString filename, CString section, LOGFONT* font);
	BOOL loadFont(CString filename, CString section, LOGFONT* font);
	BOOL loadFontInfo(CString filename);
	int getDPI(void);
	int getFontSize(int iFontHight);

	//我们将对每种国家语言进行判断，并根据每种国家语言进行初始化。
	void initializeLocale(void);
	//加载资源项目
	void readResourceItem(CString file, CString key, CString fallback);
	//开始加载资源。
	void readResourceFile(CString file);
	//加载Windows 8的字体预设资源
	int readFontResource8(CString file);
	//加载Windows 10的字体预设资源
	int readFontResource10(CString file);
	//加载资源（用于字体名称）。
	int readFontFace(CString &buffer, CString file, CString key);
	//加载资源（字体大小）。
	int readFontSize(LONG& buffer, CString file, CString key);
	//加载资源（用于字体字符集）。
	int readFontCharset(BYTE& buffer, CString file, CString key);
	//加载资源（用于图标间距）。
	int readIconSpacing(unsigned& buffer, CString file, CString key);
	//字体容器、字体选择容器初始化。tag是结构体struct缩写的前缀
	void initTagFont(void);
	void initSelFont(void);
	int mySetFontItem(LOGFONTW& font, CString& strFaceName, LONG& lHeight, BYTE& bCharSet);
	int ChangeFont(LOGFONTW& font, LOGFONTW& fontNew, CString& strFontName, HFONT& hFont, CEditImpl& edit);

	//应用设置，刷新桌面
	LRESULT SetIconSpacing(unsigned iHS, unsigned iVS, BOOL bRefresh = TRUE);
	//获取当前图标间距
	LRESULT GetIconSpacing(TagIS& tagIS);
	LRESULT GetIconSpacingOld(vector<unsigned>& vecIS);

	//必须放在TagFont定义之后。已有前置申明
	int mySetFont(NONCLIENTMETRICSW& metrics, LOGFONTW& iconFont, TagFontOld& tagFont);
	//加载预设资源
	int readFontResource(CString file, TagFontOld& tagFont);

	BOOL SaveCurSetToFile();		//保存当前配置到文件
	BOOL SaveCurSetToTag();		//保存当前配置到结构体变量

	enum fontType {
		allFont,
		titleFont,
		iconFont,
		menuFont,
		messageFont,
		paletteFont,
		tipFont
	};

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

	//C++11初始化列表。FN1：Font Name part 1
	vector<CString> vecFN1{
		L"CAPTION",
		L"ICON",
		L"MENU",
		L"MESSAGE",
		L"SMALLCAPTION",
		L"STATUS"
	};

	//C++11初始化列表。FN2：Font Name part 2
	vector<CString> vecFN2{
		L"FACE",
		L"SIZE",
		L"CHARSET"
	};

	struct TagSet			//tag of Set
	{
		TagSet& operator =(const TagSet& tag)	//重载赋值运算符
		{
			if (this != &tag)
			{
				this->metrics = tag.metrics;
				this->iconFont = tag.iconFont;
				this->tagIScur = tag.tagIScur;
			}
			return *this;
		}

		TagSet(CString str) : strFN3(str) {}

		CString strFN3;	//FN3：Font Name part 3
		NONCLIENTMETRICSW metrics;
		LOGFONTW iconFont;
		TagIS tagIScur;		//存放当前图标间距
	};

	struct TagFontOld
	{
		vector<pair<CString, CString>> vecFaces;
		vector<pair<CString, LONG>> vecSizes;
		vector<pair<CString, BYTE>> vecCharset;
		vector<pair<CString, unsigned>> vecIS;
	};
	TagFontOld tagFontCur;
	TagFontOld tagFontOld;
	TagFontOld tagFontWin8;
	TagFontOld tagFontWin10;

	map<unsigned, pair<enum fontType, LPLOGFONTW>> mapSelFont;
	
	//DDX 9个Check按钮的状态
	int m_iCheckAllfont = 0;
	int m_iCheckTitle = 0;
	int m_iCheckIcon = 0;
	int m_iCheckMenu = 0;
	int m_iCheckMessage = 0;
	int m_iCheckPalette = 0;
	int m_iCheckTip = 0;
	int m_iCheckHS = 0;
	int m_iCheckVS = 0;

	//DDX 9个Check按钮控件变量。用CButton不行
	CButtonImpl m_chkAllfont;
	CButtonImpl m_chkTitle;
	CButtonImpl m_chkIcon;
	CButtonImpl m_chkMenu;
	CButtonImpl m_chkMessage;
	CButtonImpl m_chkPalette;
	CButtonImpl m_chkTip;
	CButtonImpl m_chkHS;
	CButtonImpl m_chkVS;

	CString m_strAllFontName;
	CString m_strTitleFontName;
	CString m_strIconFontName;
	CString m_strMenuFontName;
	CString m_strMessageFontName;
	CString m_strPaletteFontName;
	CString m_strTipFontName;

	CString m_strSettingFile;

	NONCLIENTMETRICSW m_metrics, m_metricsOld;
	LOGFONTW m_iconFont, m_iconFontOld;
	TagIS m_tagIScur;	//存放当前图标间距

	NONCLIENTMETRICSW m_metricsAll;
	LOGFONTW m_iconFontAll;

	HFONT m_fontAll;
	HFONT m_fontTitle;
	HFONT m_fontIcon;
	HFONT m_fontMenu;
	HFONT m_fontMessage;
	HFONT m_fontPalette;
	HFONT m_fontTip;		//提示，暗示，Tip，Tooltip，Hint。Tip更常用

	CEditImpl m_editAllFont;
	CEditImpl m_editTitleFont;
	CEditImpl m_editIconFont;
	CEditImpl m_editMenuFont;
	CEditImpl m_editMessageFont;
	CEditImpl m_editPaletteFont;
	CEditImpl m_editTipFont;		//提示，暗示，Tip，Tooltip，Hint。Tip更常用

	bool use7Compat = true;
	bool hasCurPreset = true;
	bool has8Preset = true;
	bool has10Preset = true;
	/**您是否使用Windows 8的字体大小计算公式？ */
	bool WIN8_SIZE = true;
	/**语言资源 */
	vector<CString> langResource;


};
