#include <metahook.h>
#include <vgui/VGUI.h>
#include <vgui/IScheme.h>
#include <vgui/ILocalize.h>
#include <vgui/IEngineVGui.h>
#include <vgui_controls/Controls.h>
#include <vgui_controls/Label.h>
#include "viewport.h"
#include "SubtitlePanel.h"
#include "libcsv/csv_document.h"
#include "MemPool.h"
#include "message.h"
#include "engfuncs.h"

using namespace vgui;

CViewport *g_pViewPort = NULL;

CMemoryPool m_HashItemMemPool(sizeof(hash_item_t), 64);

extern CHudMessage m_HudMessage;

CViewport::CViewport(void) : Panel(NULL, "CaptionViewport")
{
	int swide, stall;
	surface()->GetScreenSize(swide, stall);

	MakePopup(false, true);

	SetScheme("CaptionScheme");
	SetBounds(0, 0, swide, stall);
	SetPaintBorderEnabled(false);
	SetPaintBackgroundEnabled(false);
	SetMouseInputEnabled(false);
	SetKeyBoardInputEnabled(false);
	SetProportional(true);
}

CViewport::~CViewport(void)
{

}

CDictionary *CViewport::FindDictionary(const char *szValue)
{
	int hash = 0;
	hash_item_t *item;
	int count;

	hash = CaseInsensitiveHash(szValue, m_StringsHashTable.Count());
	count = m_StringsHashTable.Count();
	item = &m_StringsHashTable[hash];

	while (item->dict)
	{
		if (!Q_strcmp(item->dict->m_szTitle, szValue))
			break;

		hash = (hash + 1) % count;
		item = &m_StringsHashTable[hash];
	}

	if (!item->dict)
	{
		item->lastHash = NULL;
		return NULL;
	}

	m_StringsHashTable[hash].lastHash = item;
	return item->dict;
}

int CViewport::CaseInsensitiveHash(const char *string, int iBounds)
{
	unsigned int hash = 0;

	if (!*string)
		return 0;

	while (*string)
	{
		if (*string < 'A' || *string > 'Z')
			hash = *string + 2 * hash;
		else
			hash = *string + 2 * hash + ' ';

		string++;
	}

	return (hash % iBounds);
}

void CViewport::EmptyDictionaryHash(void)
{
	int i;
	hash_item_t *item;
	hash_item_t *temp;
	hash_item_t *free;

	for (i = 0; i < m_StringsHashTable.Count(); i++)
	{
		item = &m_StringsHashTable[i];
		temp = item->next;
		item->dict = NULL;
		item->dictIndex = 0;
		item->lastHash = NULL;
		item->next = NULL;

		while (temp)
		{
			free = temp;
			temp = temp->next;
			m_HashItemMemPool.Free(free);
		}
	}
}

void CViewport::AddDictionaryHash(CDictionary *dict, const char *value)
{
	int count;
	hash_item_t *item;
	hash_item_t *next;
	hash_item_t *temp;
	hash_item_t *newp;
	unsigned int hash = 0;
	int dictIndex;
	CDictionary *dictTemp;

	if (!dict->m_szTitle[0])
		return;

	count = m_StringsHashTable.Count();
	hash = CaseInsensitiveHash(value, count);
	dictIndex = dict - &m_Dictionary[0];

	item = &m_StringsHashTable[hash];

	while (item->dict)
	{
		if (!Q_strcmp(item->dict->m_szTitle, dict->m_szTitle))
			break;

		hash = (hash + 1) % count;
		item = &m_StringsHashTable[hash];
	}

	if (item->dict)
	{
		next = item->next;

		while (next)
		{
			if (item->dict == dict)
				break;

			if (item->dictIndex >= dictIndex)
				break;

			item = next;
			next = next->next;
		}

		if (dictIndex < item->dictIndex)
		{
			dictTemp = item->dict;
			item->dict = dict;
			item->lastHash = NULL;
			item->dictIndex = dictIndex;
			dictIndex = dictTemp - &m_Dictionary[0];
		}
		else
			dictTemp = dict;

		if (item->dict != dictTemp)
		{
			temp = item->next;
			newp = (hash_item_t *)m_HashItemMemPool.Alloc(sizeof(hash_item_t));
			item->next = newp;
			newp->dict = dictTemp;
			newp->lastHash = NULL;
			newp->dictIndex = dictIndex;

			if (temp)
				newp->next = temp;
			else
				newp->next = NULL;
		}
	}
	else
	{
		item->dict = dict;
		item->lastHash = NULL;
		item->dictIndex = dict - &m_Dictionary[0];
	}
}

void CViewport::RemoveDictionaryHash(CDictionary *dict, const char *value)
{
	int hash = 0;
	hash_item_t *item;
	hash_item_t *last;
	int dictIndex;
	int count;

	count = m_StringsHashTable.Count();
	hash = CaseInsensitiveHash(value, count);
	dictIndex = dict - &m_Dictionary[0];


	hash = hash % count;
	item = &m_StringsHashTable[hash];

	while (item->dict)
	{
		if (!Q_strcmp(item->dict->m_szTitle, dict->m_szTitle))
			break;

		hash = (hash + 1) % count;
		item = &m_StringsHashTable[hash];
	}

	if (item->dict)
	{
		last = item;

		while (item->next)
		{
			if (item->dict == dict)
				break;

			last = item;
			item = item->next;
		}

		if (item->dict == dict)
		{
			if (last == item)
			{
				if (item->next)
				{
					item->dict = item->next->dict;
					item->dictIndex = item->next->dictIndex;
					item->lastHash = NULL;
					item->next = item->next->next;
				}
				else
				{
					item->dict = NULL;
					item->lastHash = NULL;
					item->dictIndex = 0;
				}
			}
			else
			{
				if (m_StringsHashTable[hash].lastHash == item)
					m_StringsHashTable[hash].lastHash = NULL;

				last->next = item->next;
				m_HashItemMemPool.Free(item);
			}
		}
	}
}

void CViewport::LoadDictionary(void)
{
	char szLocalPath[MAX_PATH];
	g_pFullFileSystem->GetLocalPath("captionmod/dictionary.csv", szLocalPath, sizeof(szLocalPath));

	CSV::CSVDocument doc;
	CSV::CSVDocument::row_index_type row_count = doc.load_file(szLocalPath);

	if(doc.row_count() < 2)
		return;

	m_Dictionary.SetCount(doc.row_count()-1);

	IScheme *ischeme = scheme()->GetIScheme(GetScheme());

	if(!ischeme)
		return;

	Color defaultColor = ischeme->GetColor("BaseText", Color(255, 255, 255, 200));
	
	//Prepare for making the dictionary hashtable
	m_StringsHashTable.SetSize(2048);

	for (int i = 0; i < m_StringsHashTable.Count(); i++)
		m_StringsHashTable[i].next = NULL;

	EmptyDictionaryHash();

	//Parse from the document

	for (int i = 0;i < doc.row_count()-1; ++i)
	{
		CSV::CSVDocument::row_type row = doc.get_row(i+1);

		if(row.size() < 4)
			continue;

		CDictionary &Dict = m_Dictionary[i];

		Dict.m_Color = defaultColor;

		const char *title = row[0].c_str();

		if(!title || !title[0])
			continue;

		Q_strncpy(Dict.m_szTitle, title, sizeof(Dict.m_szTitle));

		//Title ended with .wav
		int titlelen = strlen(title);
		if(!Q_stricmp(&title[titlelen-4], ".wav"))
		{
			Dict.m_Type = DICT_SOUND;
		}

		//Message found in engine (gamedir/titles.txt)
		client_textmessage_t *textmsg = gEngfuncs.pfnTextMessageGet(title);
		if(textmsg)
		{
			Dict.m_Type = DICT_MESSAGE;
			Dict.m_pTextMessage = new client_textmessage_t;
			memcpy(Dict.m_pTextMessage, textmsg, sizeof(client_textmessage_t));
		}

		const char *sentence = row[1].c_str();
		wchar_t *pLocalized = NULL;
		int localizedLength;
		if(sentence[0] == '#')
		{
			pLocalized = localize()->Find(sentence);
			if(pLocalized)
			{
				localizedLength = Q_wcslen(pLocalized);
				Dict.m_szSentence = new wchar_t[localizedLength+1];
				Q_wcsncpy(Dict.m_szSentence, pLocalized, (localizedLength + 1) * sizeof(wchar_t));
			}
		}
		if(!pLocalized)
		{
			localizedLength = MultiByteToWideChar(CP_ACP, 0, sentence, -1, NULL, 0);
			Dict.m_szSentence = new wchar_t[localizedLength+1];
			MultiByteToWideChar(CP_ACP, 0, sentence, -1, Dict.m_szSentence, localizedLength);
			Dict.m_szSentence[localizedLength] = L'0';
		}

		const char *color = row[2].c_str();
		if(color[0])
		{
			Dict.m_Color = ischeme->GetColor(color, defaultColor);

			if(Dict.m_pTextMessage)
			{
				char szColor1[16];
				char szColor2[16];
				sscanf(color, "%s %s", szColor1, szColor2);
				if(szColor2[0])
				{
					Color clrColor2 = ischeme->GetColor(szColor2, defaultColor);
					Dict.m_pTextMessage->r2 = clrColor2.r();
					Dict.m_pTextMessage->g2 = clrColor2.g();
					Dict.m_pTextMessage->b2 = clrColor2.b();
				}
				if(szColor1[0])
				{
					Color clrColor1 = ischeme->GetColor(szColor1, defaultColor);
					Dict.m_pTextMessage->r1 = clrColor1.r();
					Dict.m_pTextMessage->g1 = clrColor1.g();
					Dict.m_pTextMessage->b1 = clrColor1.b();
				}
			}
		}

		const char *duration = row[3].c_str();
		if(duration[0])
		{
			Dict.m_flDuration = Q_atof(duration);

			if(Dict.m_pTextMessage)
			{
				Dict.m_pTextMessage->holdtime = Dict.m_flDuration;
			}			
		}

		if(Dict.m_pTextMessage)
		{
			//Covert to ANSI since client.dll use ConvertANSIToUnicode to print textmessage
			int AnsiLength = WideCharToMultiByte(CP_UTF8, 0, Dict.m_szSentence, -1, NULL, 0, NULL, NULL);
			char *AnsiText = new char[AnsiLength + 1];
			WideCharToMultiByte(CP_UTF8, 0, Dict.m_szSentence, -1, AnsiText, AnsiLength, NULL, NULL);
			AnsiText[AnsiLength] = '\0';
			Dict.m_pTextMessage->pMessage = AnsiText; 
		}

		if(row.size() >= 6)
		{
			const char *next = row[4].c_str();
			if(next[0])
			{
				Q_strncpy(Dict.m_szNext, next, sizeof(Dict.m_szNext));
			}

			const char *nextdelay = row[5].c_str();
			if(nextdelay[0])
			{
				Dict.m_flNextDelay = Q_atof(nextdelay);
			}
		}
		if(row.size() >= 7)
		{
			const char *textalign = row[6].c_str();
			if(textalign[0] == 'R' || textalign[0] == 'r')
				Dict.m_iTextAlign = ALIGN_RIGHT;
			else if(textalign[0] == 'C' || textalign[0] == 'c')
				Dict.m_iTextAlign = ALIGN_CENTER;
			if(textalign[0] == 'L' || textalign[0] == 'l')
				Dict.m_iTextAlign = ALIGN_LEFT;
		}

		AddDictionaryHash(&Dict, Dict.m_szTitle);
	}

	//Link the dictionaries

	for(int i = 0; i < m_Dictionary.Count(); ++i)
	{
		CDictionary &Dict = m_Dictionary[i];
		if(Dict.m_szNext[0])
		{
			Dict.m_pNext = FindDictionary(Dict.m_szNext);
		}
	}
}

void CViewport::Start(void)
{
	m_pSubtitle = new SubtitlePanel(NULL);

	SetVisible(false);
}

void CViewport::SetParent(VPANEL vPanel)
{
	BaseClass::SetParent(vPanel);

	m_pSubtitle->SetParent(this);
}

void CViewport::Think(void)
{
}

void CViewport::VidInit(void)
{
	m_HudMessage.VidInit();
}

void CViewport::Init(void)
{
	LoadDictionary();
	m_HudMessage.Init();
}

void CViewport::StartSubtitle(CDictionary *dict)
{
	m_pSubtitle->StartSubtitle(dict, cl_time);
}

void CViewport::StartNextSubtitle(CDictionary *dict)
{
	m_pSubtitle->StartNextSubtitle(dict);
}

void CViewport::ActivateClientUI(void)
{
	SetVisible(true);
}

void CViewport::HideClientUI(void)
{
	SetVisible(false);
}

void CViewport::Paint(void)
{
	BaseClass::Paint();

	m_HudMessage.Draw();
}