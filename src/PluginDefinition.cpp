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
#include "Dialogs/VisualizerPanel.h"
#include "Dialogs/ConfigureDialog.h"
#include "Dialogs/AboutDialog.h"

#ifdef UNICODE
   #define generic_itoa _itow
#else
   #define generic_itoa itoa
#endif

FuncItem funcItem[MI_COUNT];

NppData nppData;
HINSTANCE _gModule;
ConfigIO _configIO;

VisualizerPanel _vizPanel;
ConfigureDialog _configDlg;
AboutDialog _aboutDlg;

void pluginInit(HANDLE hModule) {
   _gModule = (HINSTANCE)hModule;
   _vizPanel.init(_gModule, NULL);
}

void pluginCleanUp(){}

void commandMenuInit() {
   _configIO.init();

   ShortcutKey *shKeyOpen = new ShortcutKey;
   shKeyOpen->_isAlt = false;
   shKeyOpen->_isCtrl = true;
   shKeyOpen->_isShift = false;
   shKeyOpen->_key = VK_F8;

   setCommand(MI_GOTO_PANEL, MENU_SHOW_PANEL, ToggleVisualizerPanel, shKeyOpen, _vizPanel.isVisible());
   setCommand(MI_CONFIG_DIALOG, MENU_CONFIG, ShowConfigDialog, 0, 0);
   setCommand(MI_SEPARATOR, TEXT("-"), NULL, NULL, false);
   setCommand(MI_ABOUT_DIALOG, MENU_ABOUT, ShowAboutDialog, 0, 0);
}


void commandMenuCleanUp() {
   delete funcItem[MI_GOTO_PANEL]._pShKey;
}

// Initialize plugin commands
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool checkOnInit) {
    if (index >= MI_COUNT)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = checkOnInit;
    funcItem[index]._pShKey = sk;

    return true;
}

HWND createToolTip(HWND hDlg, int toolID, LPWSTR pTitle, LPWSTR pMessage) {
   if (!toolID || !hDlg || !pMessage)
      return FALSE;

   // Get the window of the tool.
   HWND hwndTool = GetDlgItem(hDlg, toolID);

   // Create the tooltip.
   HWND hwndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
      WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON,
      CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT,
      hDlg, NULL,
      _gModule, NULL);

   if (!hwndTool || !hwndTip)
      return (HWND)NULL;

   // Associate the tooltip with the tool.
   TOOLINFO toolInfo {};
   toolInfo.cbSize = sizeof(toolInfo);
   toolInfo.hwnd = hDlg;
   toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
   toolInfo.uId = (UINT_PTR)hwndTool;
   toolInfo.lpszText = pMessage;
   ::SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)& toolInfo);
   ::SendMessage(hwndTip, TTM_SETTITLE, TTI_INFO, (LPARAM)pTitle);
   //::SendMessage(hwndTip, TTM_SETMAXTIPWIDTH, 0, (LPARAM)PREFS_TIP_MAX_WIDTH);

   return hwndTip;
}

// Dockable Visualizer Dialog
void ToggleVisualizerPanel() {
   bool hidden = !_vizPanel.isVisible();

   if (hidden) {
      _vizPanel.setParent(nppData._nppHandle);
      tTbData data {};

      if (!_vizPanel.isCreated()) {
         _vizPanel.create(&data);

         data.uMask = DWS_DF_CONT_RIGHT;
         data.pszModuleName = _vizPanel.getPluginFileName();
         data.dlgID = MI_GOTO_PANEL;
         data.pszName = MENU_PANEL_NAME;

         ::SendMessage(nppData._nppHandle, NPPM_DMMREGASDCKDLG, 0, (LPARAM)& data);

         _vizPanel.initPanel();
      }
   }
   ShowVisualizerPanel(hidden);
}

void ShowVisualizerPanel(bool show) {
   _vizPanel.display(show);

   ::CheckMenuItem(::GetMenu(nppData._nppHandle), funcItem[MI_GOTO_PANEL]._cmdID,
               MF_BYCOMMAND | (show ? MF_CHECKED : MF_UNCHECKED));
}

void ShowConfigDialog() {
   _configDlg.doDialog((HINSTANCE)_gModule);
}

void ShowAboutDialog() {
   _aboutDlg.doDialog((HINSTANCE)_gModule);
}
