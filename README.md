# CaptionMod 3.4

This is a caption/subtitle plugin designed for displaying captions/subtitles in GoldSRC engine & VGUI2 based singleplayer games.

## It could display captions/subtitles when :

  sound or sentence is played,

  HudText or HudTextPro (Counter Strike 1.6 or Condition Zero) is displayed,

# Compile Requirement:

  MetaHook (https://github.com/Nagist/metahook)
  
  Visual Studio 2017 or higher
  
# Build

git clone https://github.com/Nagist/metahook

put the CaptionMod folder in `MetaHook\Plugins\CaptionMod`

the dir looks like

MetaHook&#92;<br/>&emsp;Plugins&#92;<br/>&emsp;&emsp;CaptionMod&#92;<br/>&emsp;&emsp;&emsp;CaptionMod.sln<br/>
    
Open CaptionMod.sln with MSVC and build it.

If no error occurs, output binary should be at `MetaHook\Plugins\CaptionMod\Release\CaptionMod.dll`
  
# Usage Instruction

### console var: cap_show 0 / 1

When set to 1, debug message will be displayed in the console.

### console command: cap_version

To check current version of this plugin.

### console command: cap_reload

Reload the dictionary file

### console command: cap_configs

Open a dialog to modify the configurations

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

### gamedir/captionmod/dictionary.csv (.xls .xlsx)

Dictionary file, encoded in ANSI or local system encoding.

Support MS Excel XLS files and XLSX files. xlsx has the

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

## Detail about macro.csv(.xls .xlsx)

### Macri Type:

regex: this is a perl regular expression that replace the untranslated Sentence (before transfered by VGUI Localization language files and after the Fill macros is executed) to the Destination Text with Source RegEx.

fill: fill this line with Destination Text, no matter what this source sentence is.

### Macro Name:

Used to identify a macro.

### Source RegEx:

see Macri Type

### Destination Text:

see Macri Type

### How to use a macro? add these in cell which is not in a sentence row.
'''
@use MacroName //to enable a macro
@end MacroName //to disable an active macro
@end //to disable the last activated macro
@endall //to disable all active macros.
'''
 
# changelog:

2020-05-01

fix a bug when Blue Shift crash.

fix issue #4

2020-04-30

fix for engine 8383.

2015-11-28

fix a bug that subtitle panel don't scale in high resolution.
