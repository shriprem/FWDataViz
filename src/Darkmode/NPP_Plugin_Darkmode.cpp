#include "NPP_Plugin_Darkmode.h"
#include "NppDarkMode.h"

HWND nppHandle;

void NPPDM_InitDarkMode(const HWND _nppHandle) {
   nppHandle = _nppHandle;
   return NppDarkMode::initDarkMode();
}

void NPPDM_QueryNPPDarkmode() {
   NppDarkMode::queryNPPDarkmode();
}

bool NPPDM_IsEnabled() {
   return NppDarkMode::isEnabled();
}

void NPPDM_AutoSubclassAndThemeChildControls(HWND hwndParent) {
   NppDarkMode::autoSubclassAndThemeChildControls(hwndParent);
}

void NPPDM_AutoThemeChildControls(HWND hwndParent) {
   NppDarkMode::autoThemeChildControls(hwndParent);
}

void NPPDM_SetDarkTitleBar(HWND hwnd) {
   NppDarkMode::setDarkTitleBar(hwnd);
}

void NPPDM_InitSysLink(HWND hCtl) {
   NppDarkMode::initSysLink(hCtl);
}

LRESULT NPPDM_OnCtlColor(HDC hdc) {
   return NppDarkMode::onCtlColor(hdc);
}

LRESULT NPPDM_OnCtlColorSofter(HDC hdc) {
   return NppDarkMode::onCtlColorSofter(hdc);
}

LRESULT NPPDM_OnCtlColorDarker(HDC hdc) {
   return NppDarkMode::onCtlColorDarker(hdc);
}

LRESULT NPPDM_OnCtlColorError(HDC hdc) {
   return NppDarkMode::onCtlColorError(hdc);
}

LRESULT NPPDM_OnCtlColorSysLink(HDC hdc) {
   return NppDarkMode::onCtlColorSysLink(hdc);
}

LRESULT NPPDM_OnCtlColorIfEnabled(HDC hdc, bool isEnabled) {
   return NppDarkMode::onCtlColorIfEnabled(hdc, isEnabled);
}

LRESULT NPPDM_OnCtlHiliteIfEnabled(HDC hdc, bool isEnabled) {
   return NppDarkMode::onCtlHiliteIfEnabled(hdc, isEnabled);
}

INT_PTR NPPDM_OnCtlColorListbox(WPARAM wParam, LPARAM lParam) {
   return NppDarkMode::onCtlColorListbox(wParam, lParam);
}
