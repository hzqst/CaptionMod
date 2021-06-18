#include <metahook.h>
#include "exportfuncs.h"
#include "engfuncs.h"

//Steam API
#include "steam_api.h"

//Viewport
#include <VGUI/VGUI.h>
#include "Viewport.h"

cl_enginefunc_t gEngfuncs;

cvar_t* al_enable = NULL;
cvar_t* cap_debug = NULL;
cvar_t* cap_enabled = NULL;
cvar_t* cap_max_distance = NULL;

void* NewClientFactory(void)
{
	return Sys_GetFactoryThis();
}

int Initialize(struct cl_enginefuncs_s* pEnginefuncs, int iVersion)
{
	memcpy(&gEngfuncs, pEnginefuncs, sizeof(gEngfuncs));

	pEnginefuncs->pfnTextMessageGet = pfnTextMessageGet;

	return gExportfuncs.Initialize(pEnginefuncs, iVersion);
}

int HUD_VidInit(void)
{
	int result = gExportfuncs.HUD_VidInit();

	g_pViewPort->VidInit();

	return result;
}

void Cap_Version_f(void)
{
	gEngfuncs.Con_Printf("%s\n", CAPTION_MOD_VERSION);
}

void HUD_Init(void)
{
	gExportfuncs.HUD_Init();

	g_pViewPort->Init();

	al_enable = gEngfuncs.pfnGetCvarPointer("al_enable");
	cap_debug = gEngfuncs.pfnRegisterVariable("cap_show", "0", FCVAR_CLIENTDLL);
	cap_enabled = gEngfuncs.pfnRegisterVariable("cap_enabled", "1", FCVAR_CLIENTDLL | FCVAR_ARCHIVE);
	cap_max_distance = gEngfuncs.pfnRegisterVariable("cap_max_distance", "1500", FCVAR_CLIENTDLL | FCVAR_ARCHIVE);
	gEngfuncs.pfnAddCommand("cap_version", Cap_Version_f);
}

static CDictionary* m_SentenceDictionary = NULL;
static qboolean m_bSentenceSound = false;
static float m_flSentenceDuration = 0;

float S_GetDuration(sfx_t* sfx)
{
	float flDuration = 0;

	if (sfx->name[0] != '*' && sfx->name[0] != '?')
	{
		//MetaAudio is active? use MetaAudio's structs
		if (al_enable && al_enable->value)
		{
			aud_sfxcache_t* asc = (aud_sfxcache_t*)gCapFuncs.S_LoadSound(sfx, NULL);
			//not a voice sound
			if (asc && asc->length != 0x40000000)
			{
				flDuration = (float)asc->length / asc->speed;
			}
		}
		else
		{
			sfxcache_t* sc = gCapFuncs.S_LoadSound(sfx, NULL);
			//not a voice sound
			if (sc && sc->length != 0x40000000)
			{
				flDuration = (float)sc->length / sc->speed;
			}
		}
	}
	return flDuration;
}

//2015-11-26 added, support added up the duration of sound for zero-duration sentences
void S_StartWave(sfx_t* sfx)
{
	const char* name = sfx->name;

	if (!Q_strnicmp(name, "sound/", 6))
		name += 6;
	else if (!Q_strnicmp(name + 1, "sound/", 6))
		name += 7;

	if (m_bSentenceSound && m_SentenceDictionary)
	{
		if (m_SentenceDictionary->m_flDuration <= 0)
		{
			float flDuration = S_GetDuration(sfx);
			if (flDuration > 0)
			{
				m_flSentenceDuration += flDuration;
			}
		}
	}

	CDictionary* Dict = g_pViewPort->FindDictionary(name, DICT_SOUND);

	if (cap_debug && cap_debug->value)
	{
		gEngfuncs.Con_Printf((Dict) ? "CaptionMod: Sound [%s] found.\n" : "CaptionMod: Sound [%s] not found.\n", name);
	}

	if (!Dict)
		return;

	//Get duration for zero-duration
	if (Dict->m_flDuration <= 0)
	{
		float flDuration = S_GetDuration(sfx);
		if (flDuration > 0)
		{
			Dict->m_flDuration = flDuration;
		}
	}

	if (Dict->m_flDuration > 0)
	{
		m_flSentenceDuration += Dict->m_flDuration;
	}

	g_pViewPort->StartSubtitle(Dict);
}

void S_StartSentence(const char* name)
{
	CDictionary* Dict = g_pViewPort->FindDictionary(name, DICT_SENTENCE);

	if (!Dict && (name[0] == '!' || name[0] == '#'))
	{
		//skip ! and # then search again
		Dict = g_pViewPort->FindDictionary(name + 1);
	}

	if (cap_debug && cap_debug->value)
	{
		gEngfuncs.Con_Printf((Dict) ? "CaptionMod: SENTENCE [%s] found.\n" : "CaptionMod: SENTENCE [%s] not found.\n", name);
	}

	m_SentenceDictionary = Dict;
}

//2015-11-26 fixed, to support !SENTENCE and #SENTENCE
void S_EndSentence(void)
{
	if (!m_SentenceDictionary)
		return;

	//use the total duration we added up before
	if (m_SentenceDictionary->m_flDuration <= 0 && m_flSentenceDuration > 0)
	{
		m_SentenceDictionary->m_flDuration = m_flSentenceDuration;
	}

	g_pViewPort->StartSubtitle(m_SentenceDictionary);
}

//2015-11-26 fixed, to support !SENTENCE and #SENTENCE
//2015-11-26 added, support added up the duration of sound for zero-duration sentences
void S_StartDynamicSound(int entnum, int entchannel, sfx_t* sfx, float* origin, float fvol, float attenuation, int flags, int pitch)
{
	if (sfx)
	{
		bool bIgnore = false;
		if (cap_max_distance->value)
		{
			float dir[3];
			VectorSubtract(origin, gEngfuncs.GetLocalPlayer()->origin, dir);
			auto distance = VectorLength(dir);
			if (distance > cap_max_distance->value)
				bIgnore = true;
		}

		if (!bIgnore)
		{
			if (sfx->name[0] == '!' || sfx->name[0] == '#')
			{
				m_bSentenceSound = true;
				m_flSentenceDuration = 0;
				S_StartSentence(sfx->name);
			}
			else
			{
				S_StartWave(sfx);
			}
		}
	}

	gCapFuncs.S_StartDynamicSound(entnum, entchannel, sfx, origin, fvol, attenuation, flags, pitch);

	if (m_bSentenceSound)
	{
		S_EndSentence();
		m_flSentenceDuration = 0;
		m_bSentenceSound = false;
	}
}

//2015-11-26 fixed, to support !SENTENCE and #SENTENCE
//2015-11-26 added, support added up the duration of sound for zero-duration sentences
void S_StartStaticSound(int entnum, int entchannel, sfx_t* sfx, float* origin, float fvol, float attenuation, int flags, int pitch)
{
	if (sfx)
	{
		bool bIgnore = false;
		if (cap_max_distance->value)
		{
			float dir[3];
			VectorSubtract(origin, gEngfuncs.GetLocalPlayer()->origin, dir);
			auto distance = VectorLength(dir);
			if (distance > cap_max_distance->value)
				bIgnore = true;
		}

		if (!bIgnore)
		{
			if (sfx->name[0] == '!' || sfx->name[0] == '#')
			{
				m_bSentenceSound = true;
				m_flSentenceDuration = 0;
				S_StartSentence(sfx->name);
			}
			else
			{
				S_StartWave(sfx);
			}
		}
	}

	gCapFuncs.S_StartStaticSound(entnum, entchannel, sfx, origin, fvol, attenuation, flags, pitch);

	if (m_bSentenceSound)
	{
		S_EndSentence();
		m_flSentenceDuration = 0;
		m_bSentenceSound = false;
	}
}

sfx_t* S_FindName(char* name, int* pfInCache)
{
	sfx_t* sfx = gCapFuncs.S_FindName(name, pfInCache);;

	//we should add
	if (m_bSentenceSound && sfx)
	{
		S_StartWave(sfx);
	}
	return sfx;
}

IBaseInterface* NewCreateInterface(const char* pName, int* pReturnCode)
{
	//MessageBoxA(NULL, pName, "NewCreateInterface", 0);

	auto fnCreateInterface = (decltype(NewCreateInterface)*)Sys_GetFactoryThis();
	auto fn = fnCreateInterface(pName, pReturnCode);
	if (fn)
		return fn;

	fnCreateInterface = (decltype(NewCreateInterface)*)gCapFuncs.GetProcAddress(g_hClientDll, CREATEINTERFACE_PROCNAME);
	fn = fnCreateInterface(pName, pReturnCode);
	if (fn)
		return fn;

	return NULL;
}

FARPROC WINAPI NewGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	if (hModule == g_hClientDll && (DWORD)lpProcName > 0xFFFF && !strcmp(lpProcName, CREATEINTERFACE_PROCNAME))
	{
		//MessageBoxA(NULL, lpProcName, "NewGetProcAddress", 0);
		return (FARPROC)NewCreateInterface;
	}
	return gCapFuncs.GetProcAddress(hModule, lpProcName);
}

void Steam_Init(void)
{
	g_bIsUseSteam = CommandLine()->CheckParm("-steam") != NULL;

	SteamAPI_Load();

	gCapFuncs.szLanguage[0] = '\0';

	if (g_bIsUseSteam && SteamAPI_Init())
	{
		g_bIsRunningSteam = SteamAPI_IsSteamRunning();

		if (g_bIsRunningSteam)
		{
			ISteamApps* Apps = SteamApps();
			if (Apps)
			{
				const char* pszLanguage = SteamApps()->GetCurrentGameLanguage();

				if (pszLanguage)
					Q_strncpy(gCapFuncs.szLanguage, pszLanguage, sizeof(gCapFuncs.szLanguage));
			}
		}
	}

	if (!gCapFuncs.szLanguage[0])
	{
		Sys_GetRegKeyValueUnderRoot("Software\\Valve\\Steam", "Language", gCapFuncs.szLanguage, sizeof(gCapFuncs.szLanguage), "english");
	}
}