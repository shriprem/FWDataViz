//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


#include "PluginDefinition.h"
#include "ConfigIO.h"
#include "SubmenuManager.h"

#include "Dialogs/VisualizerPanel.h"
#include "Dialogs/ConfigureDialog.h"
#include "Dialogs/ThemeDialog.h"
#include "Dialogs/AboutDialog.h"

#ifdef UNICODE
   #define generic_itoa _itow
#else
   #define generic_itoa itoa
#endif

FuncItem pluginMenuItems[MI_COUNT];

NppData nppData;
HINSTANCE _gModule;
ConfigIO _configIO;
SubmenuManager _submenu;

VisualizerPanel _vizPanel;
ConfigureDialog _configDlg;
ThemeDialog _themeDlg;
AboutDialog _aboutDlg;

void pluginInit(HANDLE hModule) {
   _gModule = (HINSTANCE)hModule;
   _vizPanel.init(_gModule, NULL);
}

void pluginCleanUp(){}

void commandMenuInit() {
   _configIO.init();
   NPPDM_InitDarkMode(nppData._nppHandle);

   ShortcutKey *shKeyOpen = new ShortcutKey;
   shKeyOpen->_isAlt = false;
   shKeyOpen->_isCtrl = true;
   shKeyOpen->_isShift = false;
   shKeyOpen->_key = VK_F8;
   setCommand(MI_FWVIZ_PANEL, MENU_SHOW_PANEL, ToggleVisualizerPanel, shKeyOpen, _vizPanel.isVisible());

   setCommand(MI_CONFIG_DIALOG, MENU_CONFIG_FILE_TYPES, ShowConfigDialog);
   setCommand(MI_CONFIG_THEMES, MENU_CONFIG_THEMES, ShowThemeDialog);
   setCommand(MI_SEPARATOR_1, L"-", NULL);

   ShortcutKey *shKeyJump = new ShortcutKey;
   shKeyJump->_isAlt = true;
   shKeyJump->_isCtrl = true;
   shKeyJump->_isShift = false;
   shKeyJump->_key = VK_LEFT;
   setCommand(MI_FIELD_JUMP, MENU_FIELD_JUMP, ShowJumpDialog, shKeyJump);

   ShortcutKey *shKeyLeft = new ShortcutKey;
   shKeyLeft->_isAlt = true;
   shKeyLeft->_isCtrl = false;
   shKeyLeft->_isShift = false;
   shKeyLeft->_key = VK_LEFT;
   setCommand(MI_FIELD_LEFT, MENU_FIELD_LEFT, FieldLeft, shKeyLeft);

   ShortcutKey * shKeyRight = new ShortcutKey;
   shKeyRight->_isAlt = true;
   shKeyRight->_isCtrl = false;
   shKeyRight->_isShift = false;
   shKeyRight->_key = VK_RIGHT;
   setCommand(MI_FIELD_RIGHT, MENU_FIELD_RIGHT, FieldRight, shKeyRight);

   ShortcutKey *shKeyCopy = new ShortcutKey;
   shKeyCopy->_isAlt = true;
   shKeyCopy->_isCtrl = false;
   shKeyCopy->_isShift = false;
   shKeyCopy->_key = VK_OEM_COMMA;
   setCommand(MI_FIELD_COPY, MENU_FIELD_COPY, FieldCopy, shKeyCopy);

   ShortcutKey *shKeyPaste = new ShortcutKey;
   shKeyPaste->_isAlt = true;
   shKeyPaste->_isCtrl = false;
   shKeyPaste->_isShift = false;
   shKeyPaste->_key = VK_OEM_PERIOD;
   setCommand(MI_FIELD_PASTE, MENU_FIELD_PASTE, FieldPaste, shKeyPaste);

   setCommand(MI_DATA_EXTRACTION, MENU_DATA_EXTRACTION, ShowDataExtractDialog);
   setCommand(MI_SEPARATOR_2, L"-", NULL);
   setCommand(MI_DEMO_SINGLE_REC_FILES, MENU_DEMO_SINGLE_REC_FILES, NULL);
   setCommand(MI_DEMO_MULTI_REC_FILES, MENU_DEMO_MULTI_REC_FILES, NULL);
   setCommand(MI_DEMO_MULTI_LINE_FILES, MENU_DEMO_MULTI_LINE_FILES, NULL);
   setCommand(MI_SEPARATOR_3, L"-", NULL);
   setCommand(MI_ABOUT_DIALOG, MENU_ABOUT, ShowAboutDialog);
}

void commandMenuCleanUp() {
   delete pluginMenuItems[MI_FWVIZ_PANEL]._pShKey;
   delete pluginMenuItems[MI_FIELD_JUMP]._pShKey;
   delete pluginMenuItems[MI_FIELD_LEFT]._pShKey;
   delete pluginMenuItems[MI_FIELD_RIGHT]._pShKey;
   delete pluginMenuItems[MI_FIELD_COPY]._pShKey;
   delete pluginMenuItems[MI_FIELD_PASTE]._pShKey;
}

// Initialize plugin commands
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool checkOnInit) {
    if (index >= MI_COUNT) return false;
    if (!pFunc) return false;

    lstrcpy(pluginMenuItems[index]._itemName, cmdName);
    pluginMenuItems[index]._pFunc = pFunc;
    pluginMenuItems[index]._pShKey = sk;
    pluginMenuItems[index]._init2Check = checkOnInit;

    return true;
}

HWND getCurrentScintilla() {
   int which{ -1 };

   nppMessage(NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
   if (which < 0) return (HWND)NULL;

   return (HWND)(which ? nppData._scintillaSecondHandle : nppData._scintillaMainHandle);
}

bool getDirectScintillaFunc(PSCIFUNC_T& fn, void*& ptr) {
   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return FALSE;

   fn = (LRESULT(__cdecl*)(void*, int, WPARAM, LPARAM)) SendMessage(hScintilla, SCI_GETDIRECTFUNCTION, 0, 0);
   ptr = (void*) SendMessage(hScintilla, SCI_GETDIRECTPOINTER, 0, 0);
   return TRUE;
}

LRESULT nppMessage(UINT messageID, WPARAM wparam, LPARAM lparam) {
   return SendMessage(nppData._nppHandle, messageID, wparam, lparam);
}

// Dockable Visualizer Dialog
void ShowVisualizerPanel(bool show) {
   if (show && !_vizPanel.isVisible()) {
      _vizPanel.setParent(nppData._nppHandle);
      tTbData data {};

      if (!_vizPanel.isCreated()) {
         _vizPanel.create(&data);

         data.uMask = DWS_DF_CONT_RIGHT | DWS_ICONTAB | DWS_USEOWNDARKMODE;
         data.pszModuleName = _vizPanel.getPluginFileName();
         data.dlgID = MI_FWVIZ_PANEL;
         data.pszName = MENU_PANEL_NAME;
         data.hIconTab = (HICON)::LoadImage(_gModule, MAKEINTRESOURCE(IDI_VIZ_TOOL_BTN_STD_FIELDS), IMAGE_ICON, 14, 14,
            LR_LOADMAP3DCOLORS | LR_LOADTRANSPARENT);

         nppMessage(NPPM_DMMREGASDCKDLG, 0, (LPARAM)&data);

         _vizPanel.initPanel();
      }
   }

   _vizPanel.display(show);
}

void ToggleVisualizerPanel() {
   ShowVisualizerPanel(!_vizPanel.isVisible());
}

void RefreshVisualizerPanel() {
    _vizPanel.loadListFileTypes();
    _vizPanel.loadListThemes();
}

void ShowConfigDialog() {
   _configDlg.doDialog((HINSTANCE)_gModule);
}

void ShowThemeDialog() {
   _themeDlg.doDialog((HINSTANCE)_gModule);
}

void ShowJumpDialog() {
   _vizPanel.showJumpDialog();
}

void FieldLeft() {
   _vizPanel.fieldLeft();
}

void FieldRight() {
   _vizPanel.fieldRight();
}

void FieldCopy() {
   _vizPanel.fieldCopy();
}

void FieldPaste() {
   _vizPanel.fieldPaste();
}

void ShowDataExtractDialog() {
   _vizPanel.showExtractDialog();
}

void ShowAboutDialog() {
   _aboutDlg.doDialog((HINSTANCE)_gModule);
}

void refreshDarkMode() {
   if (_vizPanel.isCreated())
      _vizPanel.refreshDarkMode();

   if (_configDlg.isCreated())
      _configDlg.refreshDarkMode();

   if (_themeDlg.isCreated())
      _themeDlg.refreshDarkMode();

   if (_aboutDlg.isCreated())
      _aboutDlg.refreshDarkMode();
}

