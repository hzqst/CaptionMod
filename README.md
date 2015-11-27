# CaptionMod 3.1
This is a caption/subtitle plugin for GoldSRC mod using VGUI2.
Mainly for displaying captions/subtitles in singleplayer games.

It could display captions/subtitles when :
  sound or sentence is played,
  HudText message is displayed,
  some other caption is displayed.

Requirement:

  MetaHook (https://github.com/Nagist/metahook)
Compiler:

  Visual Studio 2005 (msvc8)
  
changelog:

2015-11-27

把vgui2里的Scheme系统换成了起源的那套，使得字体支持outline和outline2两种描边属性。
现在非默认字体必须添加"custom"属性才能生效。
移除CaptionScheme.res中的Border项目，因为Border没卵用。
自动替换翻译文本中的换行符"\r" "\n"为'\r' '\n'（前者是两个字符组成的字符串，后者是一个字符，只有单个字符的换行符才能被正确识别）
增加对CS的HudTextPro消息的支持。

2015-11-26

SubtitlePanel.res增加"htimescale"属性，用于控制字幕总时长的缩放。
SENTENCE同时支持!和#开头和不带符号开头的了。（都会扫描一遍）
SENTENCE支持以声音总长度作为字幕时间了。
