#include <metahook.h>
#include <VGUI/ISurface.h>
#include "FontTextureCache.h"
#include <IEngineSurface.h>
#include "Color.h"
#include "vgui_internal.h"
#include "plugins.h"
#include "tier1/utlvector.h"
#include "tier1/utlrbtree.h"
#include "tier1/UtlDict.h"
#include "engfuncs.h"
#include "Cursor.h"

extern IEngineSurface *staticSurface;

extern vgui::ISurface *g_pSurface;

using namespace vgui;

class CSurfaceProxy : public ISurface
{
public:
	virtual void Shutdown(void);
	virtual void RunFrame(void);
	virtual VPANEL GetEmbeddedPanel(void);
	virtual void SetEmbeddedPanel(VPANEL pPanel);
	virtual void PushMakeCurrent(VPANEL panel, bool useInsets);
	virtual void PopMakeCurrent(VPANEL panel);
	virtual void DrawSetColor(int r, int g, int b, int a);
	virtual void DrawSetColor(Color col);
	virtual void DrawFilledRect(int x0, int y0, int x1, int y1);
	virtual void DrawOutlinedRect(int x0, int y0, int x1, int y1);
	virtual void DrawLine(int x0, int y0, int x1, int y1);
	virtual void DrawPolyLine(int *px, int *py, int numPoints);
	virtual void DrawSetTextFont(HFont font);
	virtual void DrawSetTextColor(int r, int g, int b, int a);
	virtual void DrawSetTextColor(Color col);
	virtual void DrawSetTextPos(int x, int y);
	virtual void DrawGetTextPos(int &x, int &y);
	virtual void DrawPrintText(const wchar_t *text, int textLen);
	virtual void DrawUnicodeChar(wchar_t wch);
	virtual void DrawUnicodeCharAdd(wchar_t wch);
	virtual void DrawFlushText(void);
	virtual IHTML *CreateHTMLWindow(IHTMLEvents *events, VPANEL context);
	virtual void PaintHTMLWindow(IHTML *htmlwin);
	virtual void DeleteHTMLWindow(IHTML *htmlwin);
	virtual void DrawSetTextureFile(int id, const char *filename, int hardwareFilter, bool forceReload);
	virtual void DrawSetTextureRGBA(int id, const unsigned char *rgba, int wide, int tall, int hardwareFilter, bool forceReload);
	virtual void DrawSetTexture(int id);
	virtual void DrawGetTextureSize(int id, int &wide, int &tall);
	virtual void DrawTexturedRect(int x0, int y0, int x1, int y1);
	virtual bool IsTextureIDValid(int id);
	virtual int CreateNewTextureID(bool procedural = false);
	virtual void GetScreenSize(int &wide, int &tall);
	virtual void SetAsTopMost(VPANEL panel, bool state);
	virtual void BringToFront(VPANEL panel);
	virtual void SetForegroundWindow(VPANEL panel);
	virtual void SetPanelVisible(VPANEL panel, bool state);
	virtual void SetMinimized(VPANEL panel, bool state);
	virtual bool IsMinimized(VPANEL panel);
	virtual void FlashWindow(VPANEL panel, bool state);
	virtual void SetTitle(VPANEL panel, const wchar_t *title);
	virtual void SetAsToolBar(VPANEL panel, bool state);
	virtual void CreatePopup(VPANEL panel, bool minimised, bool showTaskbarIcon = true, bool disabled = false, bool mouseInput = true, bool kbInput = true);
	virtual void SwapBuffers(VPANEL panel);
	virtual void Invalidate(VPANEL panel);
	virtual void SetCursor(HCursor cursor);
	virtual bool IsCursorVisible(void);
	virtual void ApplyChanges(void);
	virtual bool IsWithin(int x, int y);
	virtual bool HasFocus(void);
	virtual bool SupportsFeature(SurfaceFeature_e feature);
	virtual void RestrictPaintToSinglePanel(VPANEL panel);
	virtual void SetModalPanel(VPANEL panel);
	virtual VPANEL GetModalPanel(void);
	virtual void UnlockCursor(void);
	virtual void LockCursor(void);
	virtual void SetTranslateExtendedKeys(bool state);
	virtual VPANEL GetTopmostPopup(void);
	virtual void SetTopLevelFocus(VPANEL panel);
	virtual HFont CreateFont(void);
	virtual bool AddGlyphSetToFont(HFont font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int lowRange, int highRange);
	virtual bool AddCustomFontFile(const char *fontFileName);
	virtual int GetFontTall(HFont font);
	virtual void GetCharABCwide(HFont font, int ch, int &a, int &b, int &c);
	virtual int GetCharacterWidth(HFont font, int ch);
	virtual void GetTextSize(HFont font, const wchar_t *text, int &wide, int &tall);
	virtual VPANEL GetNotifyPanel(void);
	virtual void SetNotifyIcon(VPANEL context, HTexture icon, VPANEL panelToReceiveMessages, const char *text);
	virtual void PlaySound(const char *fileName);
	virtual int GetPopupCount(void);
	virtual VPANEL GetPopup(int index);
	virtual bool ShouldPaintChildPanel(VPANEL childPanel);
	virtual bool RecreateContext(VPANEL panel);
	virtual void AddPanel(VPANEL panel);
	virtual void ReleasePanel(VPANEL panel);
	virtual void MovePopupToFront(VPANEL panel);
	virtual void MovePopupToBack(VPANEL panel);
	virtual void SolveTraverse(VPANEL panel, bool forceApplySchemeSettings = false);
	virtual void PaintTraverse(VPANEL panel);
	virtual void EnableMouseCapture(VPANEL panel, bool state);
	virtual void GetWorkspaceBounds(int &x, int &y, int &wide, int &tall);
	virtual void GetAbsoluteWindowBounds(int &x, int &y, int &wide, int &tall);
	virtual void GetProportionalBase(int &width, int &height);
	virtual void CalculateMouseVisible(void);
	virtual bool NeedKBInput(void);
	virtual bool HasCursorPosFunctions(void);
	virtual void SurfaceGetCursorPos(int &x, int &y);
	virtual void SurfaceSetCursorPos(int x, int y);
	virtual void DrawTexturedPolygon(int *p, int n);
	virtual int GetFontAscent(HFont font, wchar_t wch);
	virtual void SetAllowHTMLJavaScript(bool state);
	virtual void SetLanguage(const char *pchLang);
	virtual const char *GetLanguage(void);
	virtual void DeleteTextureByID(int id);
	virtual void DrawUpdateRegionTextureBGRA(int nTextureID, int x, int y, const unsigned char *pchData, int wide, int tall);
	virtual void DrawSetTextureBGRA(int id, const unsigned char *rgba, int wide, int tall);
	virtual void CreateBrowser(VPANEL panel, IHTMLResponses *pBrowser, bool bPopupWindow, const char *pchUserAgentIdentifier);
	virtual void RemoveBrowser(VPANEL panel, IHTMLResponses *pBrowser);
	virtual IHTMLChromeController *AccessChromeHTMLController(void);
	virtual void setFullscreenMode(int wide, int tall, int bpp);
	virtual void setWindowedMode(void);
	virtual void SetAsTopMost(bool state);
	virtual void SetAsToolBar(bool state);
	virtual void PanelRequestFocus(VPANEL panel);
	virtual void EnableMouseCapture(bool state);
	virtual void DrawPrintChar(int x, int y, int wide, int tall, float s0, float t0, float s1, float t1);
	virtual void SetNotifyIcon(Image *image, VPANEL panelToReceiveMessages, const char *text);
	virtual bool SetWatchForComputerUse(bool state);
	virtual double GetTimeSinceLastUse(void);
	virtual bool VGUI2MouseControl(void);
	virtual void SetVGUI2MouseControl(bool state);

public:
	void DrawSetAlphaMultiplier(float alpha);
	float DrawGetAlphaMultiplier(void);
};

HFont g_hCurrentFont;
int g_iCurrentTextR, g_iCurrentTextG, g_iCurrentTextB, g_iCurrentTextA;

void (__fastcall *g_pfnDrawSetTextureFile)(void *pthis, int, int id, const char *filename, int hardwareFilter, bool forceReload);
void (__fastcall *g_pfnDrawTexturedRect)(void *pthis, int, int x1, int y1, int x2, int y2);

void (__fastcall *g_pfnSurface_Shutdown)(void *pthis, int);
void (__fastcall *g_pfnDrawSetTextFont)(void *pthis, int, HFont font);
void (__fastcall *g_pfnDrawUnicodeChar)(void *pthis, int, wchar_t wch);
void (__fastcall *g_pfnDrawUnicodeCharAdd)(void *pthis, int, wchar_t wch);
bool (__fastcall *g_pfnSupportsFeature)(void *pthis, int, ISurface::SurfaceFeature_e feature);
bool (__fastcall *g_pfnAddGlyphSetToFont)(void *pthis, int, HFont font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int lowRange, int highRange);
void (__fastcall *g_pfnAddCustomFontFile)(void *pthis, int, const char *fontFileName);
int (__fastcall *g_pfnGetFontTall)(void *pthis, int, HFont font);
void (__fastcall *g_pfnGetCharABCwide)(void *pthis, int, HFont font, int ch, int &a, int &b, int &c);
int (__fastcall *g_pfnGetCharacterWidth)(void *pthis, int, HFont font, int ch);
void (__fastcall *g_pfnGetTextSize)(void *pthis, int, HFont font, const wchar_t *text, int &wide, int &tall);
int (__fastcall *g_pfnGetFontAscent)(void *pthis, int, HFont font, wchar_t wch);
HFont (__fastcall *g_pfnCreateFont)(void *pthis, int);
void (__fastcall *g_pfnDrawSetTextColor)(void *pthis, int, int r, int g, int b, int a);
void (__fastcall *g_pfnDrawSetTextColor2)(void *pthis, int, Color col);
void (__fastcall *g_pfnSetAllowHTMLJavaScript)(void *pthis, int, bool state);
void (__fastcall *g_pfnSetLanguage)(void *pthis, int, const char *pchLang);
const char *(__fastcall *g_pfnGetLanguage)(void *pthis, int);
void (__fastcall *g_pfnDeleteTextureByID)(void *pthis, int, int id);
void (__fastcall *g_pfnDrawUpdateRegionTextureBGRA)(void *pthis, int, int nTextureID, int x, int y, const unsigned char *pchData, int wide, int tall);
void (__fastcall *g_pfnDrawSetTextureBGRA)(void *pthis, int, int id, const unsigned char *rgba, int wide, int tall);
void (__fastcall *g_pfnCreateBrowser)(void *pthis, int, VPANEL panel, IHTMLResponses *pBrowser, bool bPopupWindow, const char *pchUserAgentIdentifier);
void (__fastcall *g_pfnRemoveBrowser)(void *pthis, int, VPANEL panel, IHTMLResponses *pBrowser);
IHTMLChromeController *(__fastcall *g_pfnAccessChromeHTMLController)(void *pthis, int);
void (__fastcall *g_pfnsetFullscreenMode)(void *pthis, int, int wide, int tall, int bpp);
void (__fastcall *g_pfnsetWindowedMode)(void *pthis, int);
void (__fastcall *g_pfnSetAsTopMost)(void *pthis, int, bool state);
void (__fastcall *g_pfnSetAsToolBar)(void *pthis, int, bool state);
void (__fastcall *g_pfnPanelRequestFocus)(void *pthis, int, VPANEL panel);
void (__fastcall *g_pfnEnableMouseCapture)(void *pthis, int, bool state);
void (__fastcall *g_pfnDrawPrintChar)(void *pthis, int, int x, int y, int wide, int tall, float s0, float t0, float s1, float t1);
void (__fastcall *g_pfnSetNotifyIcon)(void *pthis, int, Image *image, VPANEL panelToReceiveMessages, const char *text);
bool (__fastcall *g_pfnSetWatchForComputerUse)(void *pthis, int, bool state);
double (__fastcall *g_pfnGetTimeSinceLastUse)(void *pthis, int);
bool (__fastcall *g_pfnVGUI2MouseControl)(void *pthis, int);
void (__fastcall *g_pfnSetVGUI2MouseControl)(void *pthis, int, bool state);
void (__fastcall *g_pfnSurfaceSetCursorPos)(void *pthis, int, int x, int y);
void (__fastcall *g_pfnSetCursor)(void *pthis, int, HCursor cursor);

CSurfaceProxy g_SurfaceProxy;

void CSurfaceProxy::Shutdown(void)
{
	g_pfnSurface_Shutdown(this, 0);

	FontManager().ClearAllFonts();
}

void CSurfaceProxy::RunFrame(void)
{
	g_pSurface->RunFrame();
}

VPANEL CSurfaceProxy::GetEmbeddedPanel(void)
{
	return g_pSurface->GetEmbeddedPanel();
}

void CSurfaceProxy::SetEmbeddedPanel(VPANEL pPanel)
{
	g_pSurface->SetEmbeddedPanel(pPanel);
}

void CSurfaceProxy::PushMakeCurrent(VPANEL panel, bool useInsets)
{
	g_pSurface->PushMakeCurrent(panel, useInsets);
}

void CSurfaceProxy::PopMakeCurrent(VPANEL panel)
{
	g_pSurface->PopMakeCurrent(panel);
}

void CSurfaceProxy::DrawSetColor(int r, int g, int b, int a)
{
	g_pSurface->DrawSetColor(r, g, b, a);
}

void CSurfaceProxy::DrawSetColor(Color col)
{
	g_pSurface->DrawSetColor(col);
}

void CSurfaceProxy::DrawFilledRect(int x0, int y0, int x1, int y1)
{
	g_pSurface->DrawFilledRect(x0, y0, x1, y1);
}

void CSurfaceProxy::DrawOutlinedRect(int x0, int y0, int x1, int y1)
{
	g_pSurface->DrawOutlinedRect(x0, y0, x1, y1);
}

void CSurfaceProxy::DrawLine(int x0, int y0, int x1, int y1)
{
	g_pSurface->DrawLine(x0, y0, x1, y1);
}

void CSurfaceProxy::DrawPolyLine(int *px, int *py, int numPoints)
{
	g_pSurface->DrawPolyLine(px, py, numPoints);
}

void CSurfaceProxy::DrawSetTextFont(HFont font)
{
	g_hCurrentFont = font;
	g_pfnDrawSetTextFont(this, 0, font);
}

void CSurfaceProxy::DrawSetTextColor(int r, int g, int b, int a)
{
	g_iCurrentTextR = r;
	g_iCurrentTextG = g;
	g_iCurrentTextB = b;
	g_iCurrentTextA = a;

	g_pfnDrawSetTextColor(this, 0, r, g, b, a);
}

void CSurfaceProxy::DrawSetTextColor(Color col)
{
	int r = col.r();
	int g = col.g();
	int b = col.b();
	int a = col.a();

	g_iCurrentTextR = r;
	g_iCurrentTextG = g;
	g_iCurrentTextB = b;
	g_iCurrentTextA = a;

	g_pfnDrawSetTextColor2(this, 0, col);
}

void CSurfaceProxy::DrawSetTextPos(int x, int y)
{
	g_pSurface->DrawSetTextPos(x, y);
}

void CSurfaceProxy::DrawGetTextPos(int &x, int &y)
{
	g_pSurface->DrawGetTextPos(x, y);
}

void CSurfaceProxy::DrawPrintText(const wchar_t *text, int textLen)
{
	g_pSurface->DrawPrintText(text, textLen);
}

void CSurfaceProxy::DrawUnicodeChar(wchar_t wch)
{
	if (g_hCurrentFont == INVALID_FONT)
		return;

	int x, y;
	g_pSurface->DrawGetTextPos(x, y);

	int a, b, c;
	FontManager().GetCharABCwide(g_hCurrentFont, wch, a, b, c);

	int rgbaWide, rgbaTall;
	rgbaTall = GetFontTall(g_hCurrentFont);

	if (FontManager().GetFontUnderlined(g_hCurrentFont))
	{
		rgbaWide = c + b + a;
	}
	else
	{
		x += a;
		rgbaWide = b;
	}

	int textureID;
	float *texCoords = NULL;

	if (!g_FontTextureCache.GetTextureForChar(g_hCurrentFont, wch, &textureID, &texCoords))
		return;

	g_pSurface->DrawSetTexture(textureID);
	g_pfnDrawSetTextColor(this, 0, g_iCurrentTextR, g_iCurrentTextG, g_iCurrentTextB, g_iCurrentTextA);
	staticSurface->drawPrintChar(x, y, rgbaWide, rgbaTall, texCoords[0], texCoords[1], texCoords[2], texCoords[3]);
	g_pfnDrawSetTextColor(this, 0, g_iCurrentTextR, g_iCurrentTextG, g_iCurrentTextB, g_iCurrentTextA);
	g_pSurface->DrawSetTextPos(x + b + c, y);
}

void CSurfaceProxy::DrawUnicodeCharAdd(wchar_t wch)
{
	if (g_hCurrentFont == INVALID_FONT)
		return;

	int x, y;
	g_pSurface->DrawGetTextPos(x, y);

	int a, b, c;
	FontManager().GetCharABCwide(g_hCurrentFont, wch, a, b, c);

	int rgbaWide, rgbaTall;
	rgbaTall = GetFontTall(g_hCurrentFont);

	if (FontManager().GetFontUnderlined(g_hCurrentFont))
	{
		rgbaWide = c + b + a;
	}
	else
	{
		x += a;
		rgbaWide = b;
	}

	int textureID;
	float *texCoords = NULL;

	if (!g_FontTextureCache.GetTextureForChar(g_hCurrentFont, wch, &textureID, &texCoords))
		return;

	g_pSurface->DrawSetTexture(textureID);
	g_pfnDrawSetTextColor(this, 0, g_iCurrentTextR, g_iCurrentTextG, g_iCurrentTextB, g_iCurrentTextA);
	staticSurface->drawPrintCharAdd(x, y, rgbaWide, rgbaTall, texCoords[0], texCoords[1], texCoords[2], texCoords[3]);
	g_pfnDrawSetTextColor(this, 0, g_iCurrentTextR, g_iCurrentTextG, g_iCurrentTextB, g_iCurrentTextA);
	g_pSurface->DrawSetTextPos(x + b + c, y);
}

void CSurfaceProxy::DrawFlushText(void)
{
	g_pSurface->DrawFlushText();
}

IHTML *CSurfaceProxy::CreateHTMLWindow(IHTMLEvents *events, VPANEL context)
{
	return g_pSurface->CreateHTMLWindow(events, context);
}

void CSurfaceProxy::PaintHTMLWindow(IHTML *htmlwin)
{
	g_pSurface->PaintHTMLWindow(htmlwin);
}

void CSurfaceProxy::DeleteHTMLWindow(IHTML *htmlwin)
{
	g_pSurface->DeleteHTMLWindow(htmlwin);
}

void CSurfaceProxy::DrawSetTextureFile(int id, const char *filename, int hardwareFilter, bool forceReload)
{
	g_pSurface->DrawSetTextureFile(id, filename, hardwareFilter, forceReload);
}

void CSurfaceProxy::DrawSetTextureRGBA(int id, const unsigned char *rgba, int wide, int tall, int hardwareFilter, bool forceReload)
{
	g_pSurface->DrawSetTextureRGBA(id, rgba, wide, tall, hardwareFilter, forceReload);
}

void CSurfaceProxy::DrawSetTexture(int id)
{
	g_pSurface->DrawSetTexture(id);
}

void CSurfaceProxy::DrawGetTextureSize(int id, int &wide, int &tall)
{
	g_pSurface->DrawGetTextureSize(id, wide, tall);
}

void CSurfaceProxy::DrawTexturedRect(int x0, int y0, int x1, int y1)
{
	g_pSurface->DrawTexturedRect(x0, y0, x1, y1);
}

bool CSurfaceProxy::IsTextureIDValid(int id)
{
	return g_pSurface->IsTextureIDValid(id);
}

int CSurfaceProxy::CreateNewTextureID(bool procedural)
{
	return g_pSurface->CreateNewTextureID(procedural);
}

void CSurfaceProxy::GetScreenSize(int &wide, int &tall)
{
	g_pSurface->GetScreenSize(wide, tall);
}

void CSurfaceProxy::SetAsTopMost(VPANEL panel, bool state)
{
	g_pSurface->SetAsTopMost(panel, state);
}

void CSurfaceProxy::BringToFront(VPANEL panel)
{
	g_pSurface->BringToFront(panel);
}

void CSurfaceProxy::SetForegroundWindow(VPANEL panel)
{
	g_pSurface->SetForegroundWindow(panel);
}

void CSurfaceProxy::SetPanelVisible(VPANEL panel, bool state)
{
	g_pSurface->SetPanelVisible(panel, state);
}

void CSurfaceProxy::SetMinimized(VPANEL panel, bool state)
{
	g_pSurface->SetMinimized(panel, state);
}

bool CSurfaceProxy::IsMinimized(VPANEL panel)
{
	return g_pSurface->IsMinimized(panel);
}

void CSurfaceProxy::FlashWindow(VPANEL panel, bool state)
{
	g_pSurface->FlashWindow(panel, state);
}

void CSurfaceProxy::SetTitle(VPANEL panel, const wchar_t *title)
{
	g_pSurface->SetTitle(panel, title);
}

void CSurfaceProxy::SetAsToolBar(VPANEL panel, bool state)
{
	g_pSurface->SetAsToolBar(panel, state);
}

void CSurfaceProxy::CreatePopup(VPANEL panel, bool minimised, bool showTaskbarIcon, bool disabled, bool mouseInput, bool kbInput)
{
	g_pSurface->CreatePopup(panel, minimised, showTaskbarIcon, disabled, mouseInput, kbInput);
}

void CSurfaceProxy::SwapBuffers(VPANEL panel)
{
	g_pSurface->SwapBuffers(panel);
}

void CSurfaceProxy::Invalidate(VPANEL panel)
{
	g_pSurface->Invalidate(panel);
}

void CSurfaceProxy::SetCursor(HCursor cursor)
{
	g_pfnSetCursor(this, 0, cursor);
}

bool CSurfaceProxy::IsCursorVisible(void)
{
	return g_pSurface->IsCursorVisible();
}

void CSurfaceProxy::ApplyChanges(void)
{
	g_pSurface->ApplyChanges();
}

bool CSurfaceProxy::IsWithin(int x, int y)
{
	return g_pSurface->IsWithin(x, y);
}

bool CSurfaceProxy::HasFocus(void)
{
	return g_pSurface->HasFocus();
}

bool CSurfaceProxy::SupportsFeature(vgui::ISurface::SurfaceFeature_e feature)
{
	switch (feature)
	{
		case OUTLINE_FONTS:
		{
			if (IsX360())
				return false;

			return true;
		}
	}

	return g_pfnSupportsFeature(this, 0, feature);
}

void CSurfaceProxy::RestrictPaintToSinglePanel(VPANEL panel)
{
	g_pSurface->RestrictPaintToSinglePanel(panel);
}

void CSurfaceProxy::SetModalPanel(VPANEL panel)
{
	g_pSurface->SetModalPanel(panel);
}

VPANEL CSurfaceProxy::GetModalPanel(void)
{
	return g_pSurface->GetModalPanel();
}

void CSurfaceProxy::UnlockCursor(void)
{
	g_pSurface->UnlockCursor();
}

void CSurfaceProxy::LockCursor(void)
{
	g_pSurface->LockCursor();
}

void CSurfaceProxy::SetTranslateExtendedKeys(bool state)
{
	g_pSurface->SetTranslateExtendedKeys(state);
}

VPANEL CSurfaceProxy::GetTopmostPopup(void)
{
	return g_pSurface->GetTopmostPopup();
}

void CSurfaceProxy::SetTopLevelFocus(VPANEL panel)
{
	g_pSurface->SetTopLevelFocus(panel);
}

HFont CSurfaceProxy::CreateFont(void)
{
	return FontManager().CreateFont();
}

bool CSurfaceProxy::AddGlyphSetToFont(HFont font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int lowRange, int highRange)
{
	return FontManager().AddGlyphSetToFont(font, windowsFontName, tall, weight, blur, scanlines, flags, lowRange, highRange);
}

bool CSurfaceProxy::AddCustomFontFile(const char *fontFileName)
{
	char fullPath[MAX_PATH];

	if (!g_pFullFileSystem->GetLocalPath(fontFileName, fullPath, sizeof(fullPath)))
	{
		Msg("Couldn't find custom font file '%s'\n", fontFileName);
		return false;
	}

	return FontManager().AddCustomFontFile(fullPath);
}

int CSurfaceProxy::GetFontTall(HFont font)
{
	return FontManager().GetFontTall(font);
}

void CSurfaceProxy::GetCharABCwide(HFont font, int ch, int &a, int &b, int &c)
{
	return FontManager().GetCharABCwide(font, ch, a, b, c);
}

int CSurfaceProxy::GetCharacterWidth(HFont font, int ch)
{
	return FontManager().GetCharacterWidth(font, ch);
}

void CSurfaceProxy::GetTextSize(HFont font, const wchar_t *text, int &wide, int &tall)
{
	return FontManager().GetTextSize(font, text, wide, tall);
}

VPANEL CSurfaceProxy::GetNotifyPanel(void)
{
	return g_pSurface->GetNotifyPanel();
}

void CSurfaceProxy::SetNotifyIcon(VPANEL context, HTexture icon, VPANEL panelToReceiveMessages, const char *text)
{
	g_pSurface->SetNotifyIcon(context, icon, panelToReceiveMessages, text);
}

void CSurfaceProxy::PlaySound(const char *fileName)
{
	g_pSurface->PlaySound(fileName);
}

int CSurfaceProxy::GetPopupCount(void)
{
	return g_pSurface->GetPopupCount();
}

VPANEL CSurfaceProxy::GetPopup(int index)
{
	return g_pSurface->GetPopup(index);
}

bool CSurfaceProxy::ShouldPaintChildPanel(VPANEL childPanel)
{
	return g_pSurface->ShouldPaintChildPanel(childPanel);
}

bool CSurfaceProxy::RecreateContext(VPANEL panel)
{
	return g_pSurface->RecreateContext(panel);
}

void CSurfaceProxy::AddPanel(VPANEL panel)
{
	g_pSurface->AddPanel(panel);
}

void CSurfaceProxy::ReleasePanel(VPANEL panel)
{
	g_pSurface->ReleasePanel(panel);
}

void CSurfaceProxy::MovePopupToFront(VPANEL panel)
{
	g_pSurface->MovePopupToFront(panel);
}

void CSurfaceProxy::MovePopupToBack(VPANEL panel)
{
	g_pSurface->MovePopupToBack(panel);
}

void CSurfaceProxy::SolveTraverse(VPANEL panel, bool forceApplySchemeSettings)
{
	g_pSurface->SolveTraverse(panel, forceApplySchemeSettings);
}

void CSurfaceProxy::PaintTraverse(VPANEL panel)
{
	g_pSurface->PaintTraverse(panel);
}

void CSurfaceProxy::EnableMouseCapture(VPANEL panel, bool state)
{
	g_pSurface->EnableMouseCapture(panel, state);
}

void CSurfaceProxy::GetWorkspaceBounds(int &x, int &y, int &wide, int &tall)
{
	g_pSurface->GetWorkspaceBounds(x, y, wide, tall);
}

void CSurfaceProxy::GetAbsoluteWindowBounds(int &x, int &y, int &wide, int &tall)
{
	g_pSurface->GetAbsoluteWindowBounds(x, y, wide, tall);
}

void CSurfaceProxy::GetProportionalBase(int &width, int &height)
{
	g_pSurface->GetProportionalBase(width, height);
}

void CSurfaceProxy::CalculateMouseVisible(void)
{
	g_pSurface->CalculateMouseVisible();
}

bool CSurfaceProxy::NeedKBInput(void)
{
	return g_pSurface->NeedKBInput();
}

bool CSurfaceProxy::HasCursorPosFunctions(void)
{
	return g_pSurface->HasCursorPosFunctions();
}

void CSurfaceProxy::SurfaceGetCursorPos(int &x, int &y)
{
	g_pSurface->SurfaceGetCursorPos(x, y);
}

void CSurfaceProxy::SurfaceSetCursorPos(int x, int y)
{
	return g_pfnSurfaceSetCursorPos(this, 0, x, y);
}

void CSurfaceProxy::DrawTexturedPolygon(int *p, int n)
{
	g_pSurface->DrawTexturedPolygon(p, n);
}

int CSurfaceProxy::GetFontAscent(HFont font, wchar_t wch)
{
	return FontManager().GetFontAscent(font, wch);
}

void CSurfaceProxy::SetAllowHTMLJavaScript(bool state)
{
	g_pfnSetAllowHTMLJavaScript(this, 0, state);
}

void CSurfaceProxy::SetLanguage(const char *pchLang)
{
	g_pfnSetLanguage(this, 0, pchLang);
}

const char *CSurfaceProxy::GetLanguage(void)
{
	return g_pfnGetLanguage(this, 0);
}

void CSurfaceProxy::DeleteTextureByID(int id)
{
	g_pfnDeleteTextureByID(this, 0, id);
}

void CSurfaceProxy::DrawUpdateRegionTextureBGRA(int nTextureID, int x, int y, const unsigned char *pchData, int wide, int tall)
{
	g_pfnDrawUpdateRegionTextureBGRA(this, 0, nTextureID, x, y, pchData, wide, tall);
}

void CSurfaceProxy::DrawSetTextureBGRA(int id, const unsigned char *rgba, int wide, int tall)
{
	g_pfnDrawSetTextureBGRA(this, 0, id, rgba, wide, tall);
}

void CSurfaceProxy::CreateBrowser(VPANEL panel, IHTMLResponses *pBrowser, bool bPopupWindow, const char *pchUserAgentIdentifier)
{
	g_pfnCreateBrowser(this, 0, panel, pBrowser, bPopupWindow, pchUserAgentIdentifier);
}

void CSurfaceProxy::RemoveBrowser(VPANEL panel, IHTMLResponses *pBrowser)
{
	g_pfnRemoveBrowser(this, 0, panel, pBrowser);
}

IHTMLChromeController *CSurfaceProxy::AccessChromeHTMLController(void)
{
	return g_pfnAccessChromeHTMLController(this, 0);
}

void CSurfaceProxy::setFullscreenMode(int wide, int tall, int bpp)
{
	g_pfnsetFullscreenMode(this, 0, wide, tall, bpp);
}

void CSurfaceProxy::setWindowedMode(void)
{
	g_pfnsetWindowedMode(this, 0);
}

void CSurfaceProxy::SetAsTopMost(bool state)
{
	g_pfnSetAsTopMost(this, 0, state);
}

void CSurfaceProxy::SetAsToolBar(bool state)
{
	g_pfnSetAsToolBar(this, 0, state);
}

void CSurfaceProxy::PanelRequestFocus(VPANEL panel)
{
	g_pfnPanelRequestFocus(this, 0, panel);
}

void CSurfaceProxy::EnableMouseCapture(bool state)
{
	g_pfnEnableMouseCapture(this, 0, state);
}

void CSurfaceProxy::DrawPrintChar(int x, int y, int wide, int tall, float s0, float t0, float s1, float t1)
{
	g_pfnDrawPrintChar(this, 0, x, y, wide, tall, s0, s1, s1, t1);
}

void CSurfaceProxy::SetNotifyIcon(Image *image, VPANEL panelToReceiveMessages, const char *text)
{
	g_pfnSetNotifyIcon(this, 0, image, panelToReceiveMessages, text);
}

bool CSurfaceProxy::SetWatchForComputerUse(bool state)
{
	return g_pfnSetWatchForComputerUse(this, 0, state);
}

double CSurfaceProxy::GetTimeSinceLastUse(void)
{
	return g_pfnGetTimeSinceLastUse(this, 0);
}

bool CSurfaceProxy::VGUI2MouseControl(void)
{
	return g_pfnVGUI2MouseControl(this, 0);
}

void CSurfaceProxy::SetVGUI2MouseControl(bool state)
{
	g_pfnSetVGUI2MouseControl(this, 0, state);
}

#define CWIN32FONT_GETCHARABCWIDTHS_SIG "\x55\x8B\xEC\x83\xEC\x70\x53\x56\x8B\xF1\x8D\x45\xD0\x57\x8D\x4D\xE4\x50\x8B\x45\x08\x8D\x55\xD4\x51\x52\x50\x8B\xCE"

void (__fastcall *g_pfnCWin32Font_GetCharRGBA)(void *pthis, int, int ch, int rgbaX, int rgbaY, int rgbaWide, int rgbaTall, unsigned char *rgba);
hook_t *g_hCWin32Font_GetCharRGBA;

void Surface_InstallHook(void)
{
	DWORD *pVFTable = *(DWORD **)&g_SurfaceProxy;

	g_pfnCWin32Font_GetCharRGBA = (void (__fastcall *)(void *, int, int, int, int, int, int, unsigned char *))g_pMetaHookAPI->SearchPattern((void *)g_dwEngineBase, g_dwEngineSize, CWIN32FONT_GETCHARABCWIDTHS_SIG, sizeof(CWIN32FONT_GETCHARABCWIDTHS_SIG) - 1);
	g_hCWin32Font_GetCharRGBA = g_pMetaHookAPI->InlineHook(g_pfnCWin32Font_GetCharRGBA, CWin32Font_GetCharRGBA, (void *&)g_pfnCWin32Font_GetCharRGBA);

	g_pMetaHookAPI->VFTHook(g_pSurface, 0, 1, (void *)pVFTable[1], (void *&)g_pfnSurface_Shutdown);
	g_pMetaHookAPI->VFTHook(g_pSurface, 0, 13, (void *)pVFTable[13], (void *&)g_pfnDrawSetTextFont);
	g_pMetaHookAPI->VFTHook(g_pSurface, 0, 15, (void *)pVFTable[15], (void *&)g_pfnDrawSetTextColor);
	g_pMetaHookAPI->VFTHook(g_pSurface, 0, 14, (void *)pVFTable[14], (void *&)g_pfnDrawSetTextColor2);
	g_pMetaHookAPI->VFTHook(g_pSurface, 0, 19, (void *)pVFTable[19], (void *&)g_pfnDrawUnicodeChar);
	g_pMetaHookAPI->VFTHook(g_pSurface, 0, 20, (void *)pVFTable[20], (void *&)g_pfnDrawUnicodeCharAdd);
	g_pMetaHookAPI->VFTHook(g_pSurface, 0, 50, (void *)pVFTable[50], (void *&)g_pfnSupportsFeature);
	g_pMetaHookAPI->VFTHook(g_pSurface, 0, 59, (void *)pVFTable[59], (void *&)g_pfnCreateFont);
	g_pMetaHookAPI->VFTHook(g_pSurface, 0, 60, (void *)pVFTable[60], (void *&)g_pfnAddGlyphSetToFont);
	g_pMetaHookAPI->VFTHook(g_pSurface, 0, 61, (void *)pVFTable[61], (void *&)g_pfnAddCustomFontFile);
	g_pMetaHookAPI->VFTHook(g_pSurface, 0, 62, (void *)pVFTable[62], (void *&)g_pfnGetFontTall);
	g_pMetaHookAPI->VFTHook(g_pSurface, 0, 63, (void *)pVFTable[63], (void *&)g_pfnGetCharABCwide);
	g_pMetaHookAPI->VFTHook(g_pSurface, 0, 64, (void *)pVFTable[64], (void *&)g_pfnGetCharacterWidth);
	g_pMetaHookAPI->VFTHook(g_pSurface, 0, 65, (void *)pVFTable[65], (void *&)g_pfnGetTextSize);
	g_pMetaHookAPI->VFTHook(g_pSurface, 0, 89, (void *)pVFTable[89], (void *&)g_pfnGetFontAscent);
}