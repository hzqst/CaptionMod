#ifndef SUBTITLEPANEL_H
#define SUBTITLEPANEL_H

#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Panel.h>
#include <vgui_controls/EditablePanel.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/ImagePanel.h>
#include <UtlVector.h>
#include "Viewport.h"
#include "engfuncs.h"

enum LineAnim_T
{
	ANIM_ALPHAFADE,
	ANIM_MOVEMENT
};

class CSubLine;

//字幕行动画
class CSubLineAnim
{
public:
	CSubLineAnim(float StartTime, float AnimTime, CSubLine *Line)
	{
		m_StartTime = StartTime;
		m_AnimTime = AnimTime;
		m_Line = Line;
		m_Started = false;
	}
	virtual bool IsPlaying(void)
	{
		return (cl_time >= m_StartTime && cl_time < m_StartTime + m_AnimTime);
	}
	virtual bool IsDonePlay(void)
	{
		return (cl_time >= m_StartTime + m_AnimTime);
	}
	virtual int GetType(void) = 0;
	virtual bool Update(void) = 0;
protected:
	float	m_StartTime;
	float	m_AnimTime;
	CSubLine *m_Line;
	BOOL	m_Started;
};

class CAnimMovement : public CSubLineAnim
{
public:
	CAnimMovement(float StartTime, float AnimTime, int EndValue, CSubLine *Line) : CSubLineAnim(StartTime, AnimTime, Line)
	{
		m_EndValue = EndValue;
	}
	virtual int GetType(void){return ANIM_MOVEMENT;}
	virtual bool Update(void);
private:
	int m_StartValue;
	int m_EndValue;
};

class CAnimAlphaFade : public CSubLineAnim
{
public:
	CAnimAlphaFade(float StartTime, float AnimTime, int EndValue, CSubLine *Line) : CSubLineAnim(StartTime, AnimTime, Line)
	{
		m_EndValue = EndValue;
	}
	virtual int GetType(void){return ANIM_ALPHAFADE;}
	virtual bool Update(void);
private:
	int m_StartValue;
	int m_EndValue;
};

//字幕行，对应一行字幕
class CSubLine
{
public:
	CSubLine(SubtitlePanel *Panel, CDictionary *Dict)
	{
		m_Sentence = NULL;
		m_Length = 0;
		m_Duration = 0;
		m_StartTime = 0;
		m_Alpha = 0;
		m_YPos = 0;
		m_LineIndex = 0;
		m_Retired = false;
		m_FadeOut = 0;
		m_Panel = Panel;
		m_Dict = Dict;
		m_TextLength = 0;
		m_TextAlign = ALIGN_DEFAULT;
	}

	void Draw(int x, int w, int align);
	bool Update(void);
	void MoveTo(int Dist, float Time);
	void AlphaFade(int Alpha, float Time);
	bool ShouldRetire(void);
	bool ShouldStart(void);
	int CalcYPos(void);
	float GetYPosOutRate(void);

	wchar_t			*m_Sentence;
	int				m_Length;
	float			m_EndTime;
	float			m_StartTime;
	float			m_Duration;
	Color			m_Color;//实际渲染颜色
	int				m_Alpha;//实际渲染alpha（0~255，叠加在原颜色alpha上）
	int				m_YPos;//实际渲染位置Y坐标
	int				m_LineIndex;//行号，0为最下面
	bool			m_Retired;//是否应该消失
	float			m_FadeOut;//淡出时间
	CUtlVector<CSubLineAnim *>	m_AnimList;
	SubtitlePanel	*m_Panel;
	CDictionary		*m_Dict;
	int				m_TextLength;//文字本身长度
	int				m_TextAlign;
};

class SubtitlePanel : public vgui::EditablePanel
{
private:
	DECLARE_CLASS_SIMPLE(SubtitlePanel, vgui::EditablePanel);

public:
	SubtitlePanel(Panel *parent);
	void VidInit(void);

public://Subtitle interface
	void StartSubtitle(CDictionary *dict, float starttime);
	void StartNextSubtitle(CDictionary *Dict);
	CSubLine *AddLine(CDictionary *dict, wchar_t *sentence, int len, float starttime, float duration, int textlength);
	void StartLine(CSubLine *Line);
	void RetireLine(CSubLine *Line);
	void ClearSubtitle(void);

protected:
	virtual void ApplySchemeSettings(vgui::IScheme *pScheme);
	virtual void PaintBackground(void);
	virtual void Paint(void);
	virtual void ApplySettings( KeyValues *inResourceData );

	MESSAGE_FUNC(OnTick, "Tick");

	CPanelAnimationVar( int, m_iCornorSize, "cornorsize", "8" );
	CPanelAnimationVar( int, m_iXSpace, "xspace", "12" );
	CPanelAnimationVar( int, m_iYSpace, "yspace", "8" );
	CPanelAnimationVar( int, m_iLineSpace, "linespace", "8" );
	CPanelAnimationVar( Color, m_PanelColor, "panelcolor", "SubtitleBG" );
	CPanelAnimationVar( float, m_flFadeIn, "fadein", "0.5" );
	CPanelAnimationVar( float, m_flFadeOut, "fadeout", "0.8" );
	CPanelAnimationVar( float, m_flHoldTime, "holdtime", "4.0" );

public:
	//一些属性
	int							m_iTextAlign;
	vgui::HFont					m_hTextFont;
	int							m_iFontTall;
	int							m_iScaledCornorSize;
	int							m_iScaledXSpace;
	int							m_iScaledYSpace;
	int							m_iScaledLineSpace;
	int							m_iMaxLines;
	int							m_iPanelTop;
private:
	char						m_szTextAlign[32];
	char						m_szTextFont[32];
	int							m_iPanelY;
	int							m_iPanelYEnd;
	int							m_iPanelAlpha;
	//For panel scaling animation
	int							m_iCurPanelY;
	int							m_iCurPanelYEnd;
	int							m_iRoundCornorMaterial[4];
	//正在显示的字幕行
	CUtlVector<CSubLine *>		m_Lines;
	//等待显示的字幕行
	CUtlVector<CSubLine *>		m_BackLines;
	bool						m_bInLevel;
};

#endif