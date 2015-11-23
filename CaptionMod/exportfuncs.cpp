#include <metahook.h>
#include "exportfuncs.h"
#include "engfuncs.h"
#include "msghook.h"

//viewport
#include <VGUI/VGUI.h>
#include "Viewport.h"

cl_enginefunc_t gEngfuncs;

cvar_t *al_enable = NULL;
cvar_t *cap_show = NULL;

int Initialize(struct cl_enginefuncs_s *pEnginefuncs, int iVersion)
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
	cap_show = gEngfuncs.pfnRegisterVariable("cap_show", "0", FCVAR_CLIENTDLL);
	gEngfuncs.pfnAddCommand("cap_version", Cap_Version_f);
}

static bool m_bStartSound = false;

void S_WavSubtitle(sfx_t *sfx)
{
	if(!sfx)
		return;

	const char *name = sfx->name;

	if(!Q_strnicmp(name, "sound/", 6))
		name += 6;
	else if(!Q_strnicmp(name + 1, "sound/", 6))
		name += 7;

	CDictionary *dict = g_pViewPort->FindDictionary(name);

	if(cap_show && cap_show->value)
	{
		gEngfuncs.Con_Printf((dict) ? "CaptionMod: Sound [%s] found.\n" : "CaptionMod: Sound [%s] not found.\n", name);
	}

	if(!dict)
		return;

	if(!dict->m_Type == DICT_SOUND)
		return;

	//Skip steam / voice sound
	if(dict->m_flDuration <= 0 && sfx->name[0] != '*' && sfx->name[0] != '?' && sfx->name[0] != '!' && sfx->name[0] != '#')
	{
		//Meta Audio is active
		if(al_enable && al_enable->value)
		{
			aud_sfxcache_t *asc = (aud_sfxcache_t *)gCapFuncs.S_LoadSound(sfx, NULL);
			//not a voice sound
			if(asc && asc->length != 0x40000000)
			{
				dict->m_flDuration = asc->length / asc->speed;
			}
		}
		else
		{
			sfxcache_t *sc = gCapFuncs.S_LoadSound(sfx, NULL);
			//not a voice sound
			if(sc && sc->length != 0x40000000)
			{
				dict->m_flDuration = sc->length / sc->speed;
			}
		}
	}

	g_pViewPort->StartSubtitle(dict);
}

void S_SentenceSubtitle(sfx_t *sfx)
{
	const char *name = sfx->name + 1;

	CDictionary *dict = g_pViewPort->FindDictionary(name);

	if(cap_show && cap_show->value)
	{
		gEngfuncs.Con_Printf((dict) ? "CaptionMod: SENTENCE [%s] found.\n" : "CaptionMod: SENTENCE [%s] not found.\n", name);
	}

	if(!dict)
		return;

	g_pViewPort->StartSubtitle(dict);
}

void S_StartDynamicSound(int entnum, int entchannel, sfx_t *sfx, float *origin, float fvol, float attenuation, int flags, int pitch)
{
	m_bStartSound = true;

	if(sfx && (sfx->name[0] == '!' || sfx->name[0] == '#'))
	{
		S_SentenceSubtitle(sfx);
	}

	gCapFuncs.S_StartDynamicSound(entnum, entchannel, sfx, origin, fvol, attenuation, flags, pitch);

	m_bStartSound = false;
}

void S_StartStaticSound(int entnum, int entchannel, sfx_t *sfx, float *origin, float fvol, float attenuation, int flags, int pitch)
{
	m_bStartSound = true;

	if(sfx && (sfx->name[0] == '!' || sfx->name[0] == '#'))
	{
		S_SentenceSubtitle(sfx);
	}

	gCapFuncs.S_StartStaticSound(entnum, entchannel, sfx, origin, fvol, attenuation, flags, pitch);

	m_bStartSound = false;
}

sfxcache_t *S_LoadSound(sfx_t *s, channel_t *ch)
{
	if(m_bStartSound && s)
	{
		S_WavSubtitle(s);
	}
	return gCapFuncs.S_LoadSound(s, ch);
}

void *NewClientFactory(void)
{
	return Sys_GetFactoryThis();
}

FARPROC WINAPI NewGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	if(hModule == g_hClientDll && (DWORD)lpProcName > 0xFFFF && !strcmp(lpProcName, CREATEINTERFACE_PROCNAME))
	{
		return (FARPROC)Sys_GetFactoryThis();
	}
	return gCapFuncs.GetProcAddress(hModule, lpProcName);
}