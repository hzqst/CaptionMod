# CaptionMod (legacy)

This project has been archived as legacy, any future update goes here : [MetaHookSv](https://github.com/hzqst/MetaHookSv) -> [CaptionMod](https://github.com/hzqst/MetaHookSv/blob/main/CaptionMod.md)

# 

This is a caption/subtitle plugin designed for displaying captions/subtitles in GoldSRC engine & VGUI2 based singleplayer games.

It could display captions/subtitles when:

1. Sound (wav only, not mp3) is played.

2. Sentence is played.

3. HudText or HudTextPro (Counter Strike 1.6 or Condition Zero) is displayed.

4. Counter Strike 1.6 or Condition Zero SendAudio (radio) message

# Build Requirement:

  MetaHook (https://github.com/Nagist/metahook)
  
  Visual Studio 2017
  
# Build Instruction

Open CaptionMod.sln with MSVC and build it with Release config.

If no error occurs, output binary will be at `MetaHook\Plugins\CaptionMod\Release\CaptionMod.dll` and `Binary\CaptionMod.dll`

# Installation (for those who don't have or don't want to build the binary for themself)

1. Download zip or git clone https://github.com/hzqst/CaptionMod

2. Run CaptionMod3.3_installer.exe and install.

3. Copy `Binary\CaptionMod.dll` to `steamapps\common\Half-Life\(your_game_name)\metahook\plugins\` and overwrite it.

4. Launch game from `steamapps\common\Half-Life\metahook.exe -game (your_game_name)`

# Console cvar

### cap_enabled 0 / 1

When set to 0, captions will be disabled. Otherwise, captions will be enabled.

### cap_debug 0 / 1

When set to 1, debug message will be displayed in the console.

### cap_netmessage 0 / 1

When set to 1, __NETMESSAGE__ will be translated.

### cap_max_distance

When set to non-zero, sound or sentences playing far away from this distance will be ignored.

### console command: cap_version

To check current version of this plugin.

### console command: cap_reload

Reload the dictionary file

## Files Usage

### gamedir/captionmod/CaptionScheme.res

Color settings, font settings, loading custom font files(.ttf only)...

### gamedir/captionmod/SubtitlePanel.res

Subtitle panel settings and some detail settings for subtitle text.

### gamedir/captionmod/fonts/*.ttf

Put custom font files in it.

Then add font files in CaptionScheme.res->CustomFontFiles

Then add font tag in CaptionScheme.res->Fonts, or just mofify "SubtitleFont".

### gamedir/captionmod/materials/*.tga

Textures of panel's round cornor.

### gamedir/captionmod/dictionary_english.txt
### gamedir/captionmod/dictionary_schinese.txt
### gamedir/captionmod/dictionary_language.txt

Localization files, for multi-language support.

### gamedir/captionmod/dictionary.csv

Dictionary file, encoded in ANSI or local system encoding.

It is not recommended to put any non-ANSI characters in dictionary file.

# You can trigger the subtitle in three ways:

## 1.Play a wave file.

Once the game plays a .wav file, it will scan the dictionary with the sound path (without sound/ prefix),

The subtitle will display if an entry was found in the dictionary.

## 2.By playing SENTENCE

see gamedir\sound\sentences.txt

for example, HG_GREN0 is a SENTENCE, the subtitle will display once a SENTENCE is played and the name of it was found in the dictionary.

## 3.By "Next Subtitle"

Triggered by some other subtitle from the "Next Subtitle" section. it's very useful when making subtitles for long sentences.

(do not try to trigger itself or make a circular trigger, it might cause infinite loop)

## Detail about dictionary.csv(.xls .xlsx)

### Title (Required) (Case-Sensitive):

The title of this subtitle, used to identify a subtitle sentence.

file name with extension ".wav" means it's triggered by a .wav sound. notice that do not add "sound/" prefix!

or it's the message name in gamedir/titles.txt?

or it's the sentence name in gamedir/sentence.txt?

or it's just triggered by another subtitle?

### Sentence:

Subtitle text that displayed in the subtitle panel.

Support line-break(Alt+Enter in Office Excel) ,quote,comma for XLS/XLSX format dictionary ...

Support Valve's language file ("#" + String Name in dictionary_english/language.txt). for example: "#CaptionMod_ForExample"

### Color (Case-Sensitive):

Must be declared in CaptionScheme.res!

You can use two color for HudMessage, one for $color and another for $color2. empty means no changes.

### Duration:

How long this sentence stays alive for? 0/empty means the life of subtitle depends on the length of sound (.wav only!).

For HudMessage, it's will modify the $holdtime param.

### Speaker:

Add prefix such as "[Hologram]" at the begin of the subtitle sentence.

Support Valve's language file. For example #HalfLife_Scientist will add [Scientist] at the begin.

### Next Subtitle (Case-Sensitive):

Which subtitle to trigger after "Delay to Next Subtitle" second(s) ?

### Delay to Next Subtitle:

See "Next Subtitle", unit: second

### Style:

Text alignment for subtitle text and the way how this sentence fade in?

available value: L / C / R (Case-Insensitive)

available value: alphafade / leftscan (Case-Insensitive)
 
# changelog:

2021-01-03

fix issue #7

2020-05-01

fix : Blue Shift crash.

fix issue #4

2020-04-30

fix : engine 8383 crash.

2015-11-28

fix : subtitle panel don't scale in high resolution.
