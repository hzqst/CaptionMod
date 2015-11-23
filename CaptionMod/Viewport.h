#pragma once

#ifdef GetCurrentTime
#undef GetCurrentTime
#endif

#include <vgui/VGUI.h>
#include <vgui/ISurface.h>
#include <vgui/ILocalize.h>
#include <vgui/IScheme.h>
#include <vgui/IVGui.h>
#include <vgui/IInput.h>
#include <VGUI_controls/Controls.h>
#include <VGUI_controls/Panel.h>
#include <VGUI_controls/Frame.h>
#include <sequence.h>

class SubtitlePanel;
class CHudMessage;

enum dict_t
{
	DICT_CUSTOM,
	DICT_SOUND,
	DICT_MESSAGE,
};

enum textalign_t
{
	ALIGN_DEFAULT,
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT,
};

class CDictionary
{
public:
	CDictionary()
	{
		m_Type = DICT_CUSTOM;
		m_szTitle[0] = 0;
		m_szSentence = NULL;
		m_Color = Color(255, 255, 255, 255);
		m_flDuration = 0;
		m_flNextDelay = 0;
		m_szNext[0] = 0;
		m_pNext = NULL;
		m_pTextMessage = NULL;
		m_iTextAlign = ALIGN_DEFAULT;
	}

	~CDictionary()
	{
		if(m_szSentence)
			delete m_szSentence;
		if(m_pTextMessage)
		{
			if(m_pTextMessage->pMessage)
				delete m_pTextMessage->pMessage;
			delete m_pTextMessage;
		}
	}

	dict_t					m_Type;
	char					m_szTitle[64];
	wchar_t					*m_szSentence;
	Color					m_Color;
	float					m_flDuration;
	float					m_flNextDelay;
	char					m_szNext[64];
	CDictionary				*m_pNext;
	client_textmessage_t	*m_pTextMessage;
	int						m_iTextAlign;
};

class CPendingSubtitle
{
public:
	CDictionary		*m_pDict;
	float			m_flStartTime;
};

typedef struct hash_item_s
{
	CDictionary *dict;
	struct hash_item_s *next;
	struct hash_item_s *lastHash;
	int dictIndex;
}hash_item_t;

class CViewport : public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(CViewport, vgui::Panel);

public:
	CViewport(void);
	~CViewport(void);

public:
	//ClientVGUI Interface
	void Start(void);
	void Init(void);
	void VidInit(void);
	void Think(void);
	void Paint(void);
	void SetParent(vgui::VPANEL vPanel);
	void ActivateClientUI(void);
	void HideClientUI(void);
	void LoadDictionary(void);

	//Subtitle Interface
	void StartSubtitle(CDictionary *dict);
	void StartNextSubtitle(CDictionary *dict);

	//Dictionary Hashtable
	CDictionary *FindDictionary(const char *szValue);
	int CaseInsensitiveHash(const char *string, int iBounds);
	void EmptyDictionaryHash(void);
	void AddDictionaryHash(CDictionary *dict, const char *value);
	void RemoveDictionaryHash(CDictionary *dict, const char *value);

public:
	CHudMessage	*m_pHudMessage;

private:
	SubtitlePanel *m_pSubtitle;
	CUtlVector<hash_item_t> m_StringsHashTable;
	CUtlVector<CDictionary> m_Dictionary;	
};

extern CViewport *g_pViewPort;