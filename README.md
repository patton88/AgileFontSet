# AgileFontSet

  AgileFontSet v2.1 程序说明
  
  迅捷字体设置程序v2.1 - ybmj@vip.163.com(20190129)

  (1)、程序用途：快捷设置Windows系统字体和桌面图标间距
  WinXP、Win7用户可在系统提供的高级外观设置里修改系统字体和桌面图标间距，但是，Win8、Win10系统却没有提供这一功能。熟悉系统的用户虽然可以通过修改注册表、bat、vbs等方式调整系统字体和桌面图标间距，但这些方法都需要注销后重新登录、或重启计算机才能生效。
  该程序 AgileFontSet 便是快捷设置Windows系统字体和桌面图标间距的小程序，中文名为“迅捷字体设置程序”。该程序可以加载和保存Win7Preset、Win8xPreset、Win10Preset、UserPreset1 - UserPreset100等多种用户配置。该程序提供窗口界面、命令行两种使用方式。窗口界面简洁快捷、一目了然，普通用户也可轻松使用，并且，设置后无需注销或重启便可立即生效；命令行方式可用于WinPE等需要自动设置系统字体和桌面图标间距的情况。

  (2)、使用环境
  该程序采用VS2017 WTL10 x86编译(Windows Template Library，http://wtl.sourceforge.net/ )，程序短小精悍、可独立运行，支持32位和64位的Vista、Win7、Win8.x、Win10及相应Windows Server版等操作系统。

  (3)、窗口界面快捷设置系统字体和桌面图标间距
  在Windows中直接运行 AgileFontSet 便可启动窗口界面，启动后会显示系统字体和桌面图标间距的当前值，设置对话框简洁快捷、一目了然，普通用户也可快速方便地设置系统字体和桌面图标间距，并且可以立即生效。在窗口界面，用户可以直观地加载和保存默认配置、Win7Preset、Win8xPreset、Win10Preset、UserPreset1 - UserPreset100等多种用户配置。

  (4)、命令行方式设置系统字体和桌面图标间距
  在WinPE等需要自动设置系统字体和桌面图标间距的时候，可以使用命令行方式。用户可按以下格式的参数启动程序，所有参数都不分大小写。更改系统字体和桌面图标间距后程序将立即刷新桌面。加方括号[]的为可选参数，加圆括号()的为只能单独使用的可选独立参数，(无参)是不带任何参数。
  [path] ：指定加载ini配置文件的路径，启动后将显示ini配置文件中的默认配置，注意：含空格的路径必须用双引号包围；
  [path -xxx] ：指定加载ini配置文件的路径path，-xxx 参数选择ini文件中的xxx配置。xxx可以是：Win7Preset、Win8xPreset、Win10Preset、UserPreset1 - UserPreset100 之一；
  [path -hide] ：指定加载ini配置文件的路径path，-hide 参数指定后台加载ini配置文件中的默认配置；
  [path -xxx -hide] ：指定加载ini配置文件的路径path，-xxx 参数选择ini文件中的xxx配置，-hide 参数指定进行后台设置。3个参数组合使用，可以后台加载ini配置文件中的xxx配置；
  (-?) ：显示设置对话框和此帮助信息(当前选项卡)，可显示系统字体和桌面图标间距的当前值；
  (无参) ：显示设置对话框(当前选项卡)和此帮助信息，可显示系统字体和桌面图标间距的当前值。

  (5)、程序使用实例
  ＃ 指定加载ini配置文件"D:\Program Files\User Data\myFontSet.ini"，启动后将显示ini配置文件中的默认配置，注意：含空格的路径必须用双引号包围。
  AgileFontSet "D:\Program Files\User Data\myFontSet.ini"

  ＃ 指定加载ini配置文件D:\myFontSet.ini，启动后将选择ini文件中的UserPreset1配置。
  AgileFontSet D:\myFontSet.ini -UserPreset1

  ＃ 指定后台设置ini配置文件D:\myFontSet.ini中的默认配置。
  AgileFontSet D:\myFontSet.ini -hide

  ＃ 指定后台设置ini配置文件D:\myFontSet.ini中的Win10Preset配置。
  AgileFontSet D:\myFontSet.ini -Win10Preset -hide

  ＃ 显示设置对话框和此帮助信息(当前选项卡)，获取系统字体和桌面图标间距的当前值。
  AgileFontSet -?

  ＃ 显示设置对话框(当前选项卡)和此帮助信息，获取系统字体和桌面图标间距的当前值。
  AgileFontSet

  (6)、免责申明
  用户可自行斟酌选用该程序，若转载请注明出处。对一切后果，作者不承担任何责任！

