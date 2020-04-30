# CaptionMod 3.3

This is a caption/subtitle plugin designed for displaying captions/subtitles in GoldSRC engine & VGUI2 based singleplayer games.

## It could display captions/subtitles when :

  sound or sentence is played,

  HudText or HudTextPro (Counter Strike 1.6 or Condition Zero) is displayed,

# Compile Requirement:

  MetaHook (https://github.com/Nagist/metahook)
  
  Visual Studio 2005 (msvc8) or higher
  
# Build

git clone https://github.com/Nagist/metahook

put the CaptionMod folder in `MetaHook\Plugins\CaptionMod`

the dir looks like

MetaHook&#92;<br/>&emsp;Plugins&#92;<br/>&emsp;&emsp;CaptionMod&#92;<br/>&emsp;&emsp;&emsp;CaptionMod.sln<br/>
    
Open CaptionMod.sln with MSVC and build it.

If no error occurs, output binary should be at `MetaHook\Plugins\CaptionMod\Release\CaptionMod.dll`
  
# changelog:

2015-11-28

修复了高分辨率下字幕面板不会缩放的bug

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
