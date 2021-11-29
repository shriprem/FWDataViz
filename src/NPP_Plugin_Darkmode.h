#pragma once

#include <Windows.h>

#ifdef NPP_PLUGIN_DARKMODE_EXPORTS
#define NPP_PLUGIN_DARKMODE_API __declspec(dllexport)
#else
#define NPP_PLUGIN_DARKMODE_API __declspec(dllimport)
#endif

extern "C" NPP_PLUGIN_DARKMODE_API void NPPDM_InitDarkMode(const HWND _nppHandle);
extern "C" NPP_PLUGIN_DARKMODE_API bool NPPDM_IsEnabled();
extern "C" NPP_PLUGIN_DARKMODE_API bool NPPDM_IsToolBarFilled();
extern "C" NPP_PLUGIN_DARKMODE_API void NPPDM_AutoSubclassAndThemeChildControls(HWND hwndParent, bool subclass = true, bool theme = true);
extern "C" NPP_PLUGIN_DARKMODE_API void NPPDM_SetDarkTitleBar(HWND hwnd);
extern "C" NPP_PLUGIN_DARKMODE_API LRESULT NPPDM_OnCtlColor(HDC hdc);
extern "C" NPP_PLUGIN_DARKMODE_API LRESULT NPPDM_OnCtlColorSofter(HDC hdc);
extern "C" NPP_PLUGIN_DARKMODE_API LRESULT NPPDM_OnCtlColorDarker(HDC hdc);
extern "C" NPP_PLUGIN_DARKMODE_API LRESULT NPPDM_OnCtlColorError(HDC hdc);
extern "C" NPP_PLUGIN_DARKMODE_API LRESULT NPPDM_OnCtlColorSysLink(HDC hdc);
