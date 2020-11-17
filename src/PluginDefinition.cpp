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
#include "Resources/samples_list.h"
#include "Dialogs/VisualizerPanel.h"
#include "Dialogs/ConfigureDialog.h"
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
   setCommand(MI_SEPARATOR_1, L"-", NULL);
   setCommand(MI_DEMO_SINGLE_REC_FILES, MENU_DEMO_SINGLE_REC_FILES, NULL);
   setCommand(MI_DEMO_MULTI_REC_FILES, MENU_DEMO_MULTI_REC_FILES, NULL);
   setCommand(MI_DEMO_MULTI_LINE_FILES, MENU_DEMO_MULTI_LINE_FILES, NULL);
   setCommand(MI_SEPARATOR_2, L"-", NULL);
   setCommand(MI_ABOUT_DIALOG, MENU_ABOUT, ShowAboutDialog, 0, 0);
}


void commandMenuCleanUp() {
   delete pluginMenuItems[MI_GOTO_PANEL]._pShKey;
}

// Initialize plugin commands
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool checkOnInit) {
    if (index >= MI_COUNT)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(pluginMenuItems[index]._itemName, cmdName);
    pluginMenuItems[index]._pFunc = pFunc;
    pluginMenuItems[index]._init2Check = checkOnInit;
    pluginMenuItems[index]._pShKey = sk;

    return true;
}

HWND getCurrentScintilla() {
   int which{ -1 };

   nppMessage(NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
   if (which < 0) return (HWND)NULL;

   return (HWND)(which ? nppData._scintillaSecondHandle : nppData._scintillaMainHandle);
}

bool getDirectScintillaFunc(PSCIFUNC_T &fn, void* &ptr) {
   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return FALSE;

   fn = (LRESULT(__cdecl*)(void*, int, WPARAM, LPARAM)) SendMessage(hScintilla, SCI_GETDIRECTFUNCTION, 0, 0);
   ptr = (void*) SendMessage(hScintilla, SCI_GETDIRECTPOINTER, 0, 0);
   return TRUE;
}

LRESULT nppMessage(UINT messageID, WPARAM wparam, LPARAM lparam)
{
   return SendMessage(nppData._nppHandle, messageID, wparam, lparam);
}

HMENU getPluginSubMenu() {
   HMENU hPluginMenu = (HMENU)nppMessage(NPPM_GETMENUHANDLE, 0, 0);
   int menuItemCount = GetMenuItemCount(hPluginMenu);

   for (int i{}; i < menuItemCount; i++)
   {
      TCHAR pluginItemText[MAX_PATH];
      int pluginItemLen{};

      pluginItemLen = GetMenuString(hPluginMenu, i, pluginItemText, MAX_PATH, MF_BYPOSITION);
      if (pluginItemLen > 0 && wstring{ pluginItemText } == MENU_PANEL_NAME) {
         HMENU hSubMenu = ::GetSubMenu(hPluginMenu, i);

         if (GetMenuState(hSubMenu, (UINT)pluginMenuItems[0]._cmdID, MF_BYCOMMAND) != -1)
            return hSubMenu;
      }
   }

   return NULL;
}

void initMenuSampleFiles() {
   HMENU hSubMenu = getPluginSubMenu();
   if (hSubMenu == NULL) return;

   int itemID{};
   size_t itemCount{};

   HMENU hMenuSingleRec = CreatePopupMenu();
   ModifyMenu(hSubMenu, MI_DEMO_SINGLE_REC_FILES, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hMenuSingleRec, MENU_DEMO_SINGLE_REC_FILES);

   itemID = 101;
   itemCount = std::size(gSampleSingleRecs);
   for (int i{}; i < itemCount; i++) {
      AppendMenu(hMenuSingleRec, MF_STRING, itemID + i, gSampleSingleRecs[i].display_name.c_str());
   }

   HMENU hMenuMultiRec = CreatePopupMenu();
   ModifyMenu(hSubMenu, MI_DEMO_MULTI_REC_FILES, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hMenuMultiRec, MENU_DEMO_MULTI_REC_FILES);

   itemID = 201;
   itemCount = std::size(gSampleMultiRecs);
   for (int i{}; i < itemCount; i++) {
      AppendMenu(hMenuMultiRec, MF_STRING, itemID + i, gSampleMultiRecs[i].display_name.c_str());
   }

   HMENU hMenuMultiLine = CreatePopupMenu();
   ModifyMenu(hSubMenu, MI_DEMO_MULTI_LINE_FILES, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hMenuMultiLine, MENU_DEMO_MULTI_LINE_FILES);

   itemID = 301;
   itemCount = std::size(gSampleMultiLines);
   for (int i{}; i < itemCount; i++) {
      AppendMenu(hMenuMultiLine, MF_STRING, itemID + i, gSampleMultiLines[i].display_name.c_str());
   }
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

         nppMessage(NPPM_DMMREGASDCKDLG, 0, (LPARAM)& data);

         _vizPanel.initPanel();
      }
   }
   ShowVisualizerPanel(hidden);
}

void ShowVisualizerPanel(bool show) {
   _vizPanel.display(show);

   CheckMenuItem(GetMenu(nppData._nppHandle), pluginMenuItems[MI_GOTO_PANEL]._cmdID,
               MF_BYCOMMAND | (show ? MF_CHECKED : MF_UNCHECKED));
}

void RefreshVisualizerPanel() {
    _vizPanel.loadFileTypes();
}

void ShowConfigDialog() {
   _configDlg.doDialog((HINSTANCE)_gModule);
}

void ShowAboutDialog() {
   _aboutDlg.doDialog((HINSTANCE)_gModule);
}
