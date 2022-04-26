#pragma once

#include <Windows.h>

void NPPDM_InitDarkMode();
void NPPDM_QueryNPPDarkmode();
bool NPPDM_IsEnabled();
void NPPDM_AutoSubclassAndThemeChildControls(HWND hwndParent, bool subclass = true, bool theme = true);
void NPPDM_SetDarkTitleBar(HWND hwnd);
void NPPDM_InitSysLink(HWND hCtl);
LRESULT NPPDM_OnCtlColor(HDC hdc);
LRESULT NPPDM_OnCtlColorSofter(HDC hdc);
LRESULT NPPDM_OnCtlColorDarker(HDC hdc);
LRESULT NPPDM_OnCtlColorError(HDC hdc);
LRESULT NPPDM_OnCtlColorSysLink(HDC hdc);
LRESULT NPPDM_OnCtlColorIfEnabled(HDC hdc, bool isEnabled);
LRESULT NPPDM_OnCtlHiliteIfEnabled(HDC hdc, bool isEnabled);
