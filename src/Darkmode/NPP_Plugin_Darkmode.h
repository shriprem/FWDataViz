#pragma once

#include <Windows.h>



#ifdef NPP_PLUGIN_DARKMODE_EXPORTS
#define NPP_PLUGIN_DARKMODE_API __declspec(dllexport)
#else
#define NPP_PLUGIN_DARKMODE_API __declspec(dllimport)
#endif // NPP_PLUGIN_DARKMODE_EXPORTS



#ifdef NPP_PLUGIN_MODE_LIB_AND_DLL

extern "C" NPP_PLUGIN_DARKMODE_API void NPPDM_InitDarkMode(const HWND _nppHandle);
extern "C" NPP_PLUGIN_DARKMODE_API void NPPDM_QueryNPPDarkmode();
extern "C" NPP_PLUGIN_DARKMODE_API bool NPPDM_IsEnabled();
extern "C" NPP_PLUGIN_DARKMODE_API void NPPDM_AutoSubclassAndThemeChildControls(HWND hwndParent);
extern "C" NPP_PLUGIN_DARKMODE_API void NPPDM_AutoThemeChildControls(HWND hwndParent);
extern "C" NPP_PLUGIN_DARKMODE_API void NPPDM_SetDarkTitleBar(HWND hwnd);
extern "C" NPP_PLUGIN_DARKMODE_API void NPPDM_InitSysLink(HWND hCtl);
extern "C" NPP_PLUGIN_DARKMODE_API LRESULT NPPDM_OnCtlColor(HDC hdc);
extern "C" NPP_PLUGIN_DARKMODE_API LRESULT NPPDM_OnCtlColorSofter(HDC hdc);
extern "C" NPP_PLUGIN_DARKMODE_API LRESULT NPPDM_OnCtlColorDarker(HDC hdc);
extern "C" NPP_PLUGIN_DARKMODE_API LRESULT NPPDM_OnCtlColorError(HDC hdc);
extern "C" NPP_PLUGIN_DARKMODE_API LRESULT NPPDM_OnCtlColorSysLink(HDC hdc);
extern "C" NPP_PLUGIN_DARKMODE_API LRESULT NPPDM_OnCtlColorIfEnabled(HDC hdc, bool isEnabled);
extern "C" NPP_PLUGIN_DARKMODE_API LRESULT NPPDM_OnCtlHiliteIfEnabled(HDC hdc, bool isEnabled);
extern "C" NPP_PLUGIN_DARKMODE_API INT_PTR NPPDM_OnCtlColorListbox(WPARAM wParam, LPARAM lParam);

#else

void NPPDM_InitDarkMode(const HWND _nppHandle);
void NPPDM_QueryNPPDarkmode();
bool NPPDM_IsEnabled();
void NPPDM_AutoSubclassAndThemeChildControls(HWND hwndParent);
void NPPDM_AutoThemeChildControls(HWND hwndParent);
void NPPDM_SetDarkTitleBar(HWND hwnd);
void NPPDM_InitSysLink(HWND hCtl);
LRESULT NPPDM_OnCtlColor(HDC hdc);
LRESULT NPPDM_OnCtlColorSofter(HDC hdc);
LRESULT NPPDM_OnCtlColorDarker(HDC hdc);
LRESULT NPPDM_OnCtlColorError(HDC hdc);
LRESULT NPPDM_OnCtlColorSysLink(HDC hdc);
LRESULT NPPDM_OnCtlColorIfEnabled(HDC hdc, bool isEnabled);
LRESULT NPPDM_OnCtlHiliteIfEnabled(HDC hdc, bool isEnabled);
INT_PTR NPPDM_OnCtlColorListbox(WPARAM wParam, LPARAM lParam);

#endif // NPP_PLUGIN_MODE_LIB_AND_DLL
