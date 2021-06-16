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
#include "SubmenuManager.h"
#include "Dialogs/VisualizerPanel.h"

extern FuncItem pluginMenuItems[MI_COUNT];
extern NppData nppData;
extern VisualizerPanel _vizPanel;
extern SubmenuManager _submenu;

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  reasonForCall, LPVOID /*lpReserved*/) {
    switch (reasonForCall) {
      case DLL_PROCESS_ATTACH:
        pluginInit(hModule);
        break;

      case DLL_PROCESS_DETACH:
        pluginCleanUp();
        break;

      case DLL_THREAD_ATTACH:
        break;

      case DLL_THREAD_DETACH:
        break;
    }

    return TRUE;
}


extern "C" __declspec(dllexport) void setInfo(NppData notpadPlusData) {
   nppData = notpadPlusData;
   commandMenuInit();
}

extern "C" __declspec(dllexport) const TCHAR * getName() {
   return MENU_PANEL_NAME;
}

extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int *nbF) {
   *nbF = MI_COUNT;
   return pluginMenuItems;
}


extern "C" __declspec(dllexport) void beNotified(SCNotification *notifyCode) {
   switch (notifyCode->nmhdr.code) {
      case NPPN_READY:
         _submenu.listSampleFiles();
         break;

      case NPPN_BUFFERACTIVATED:
         DisplayCaretFrame();
         _vizPanel.onBufferActivate();
         break;

      case NPPN_TBMODIFICATION:
         Utils::addToolbarIcon(MI_FWVIZ_PANEL, IDB_VIZ_TOOL_BTN_STD_FIELDS,
            IDI_VIZ_TOOL_BTN_FLUENT_FIELDS, IDI_VIZ_TOOL_BTN_DARK_FIELDS);
         Utils::addToolbarIcon(MI_CARET_FRAMED, IDB_VIZ_TOOL_BTN_STD_FRAMED,
            IDI_VIZ_TOOL_BTN_FLUENT_FRAMED, IDI_VIZ_TOOL_BTN_DARK_FRAMED);
         break;

      case SCN_UPDATEUI:
         _vizPanel.renderCurrentPage();
         break;

      case NPPN_SHUTDOWN:
         commandMenuCleanUp();
         break;

      default:
         return;
   }
}


extern "C" __declspec(dllexport) LRESULT messageProc(UINT message, WPARAM wParam, LPARAM lParam) {
   switch (message)
   {
      case WM_COMMAND:
         _submenu.loadSampleFile(wParam, lParam);
         break;

      default:
         break;
   }
   return TRUE;
}

#ifdef UNICODE
extern "C" __declspec(dllexport) BOOL isUnicode() {
    return TRUE;
}
#endif //UNICODE
