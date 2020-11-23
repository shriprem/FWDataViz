#include "SubmenuManager.h"
#include "Dialogs/VisualizerPanel.h"
#include "Resources/SamplesMenuDefs.h"

extern FuncItem pluginMenuItems[MI_COUNT];
extern VisualizerPanel _vizPanel;

void SubmenuManager::listSampleFiles() {
   HMENU hSubMenu = getPluginSubMenu();
   if (hSubMenu == NULL) return;

   HMENU hMenuSingleRec = CreatePopupMenu();
   ModifyMenu(hSubMenu, MI_DEMO_SINGLE_REC_FILES, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hMenuSingleRec, MENU_DEMO_SINGLE_REC_FILES);

   HMENU hMenuMultiRec = CreatePopupMenu();
   ModifyMenu(hSubMenu, MI_DEMO_MULTI_REC_FILES, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hMenuMultiRec, MENU_DEMO_MULTI_REC_FILES);

   HMENU hMenuMultiLine = CreatePopupMenu();
   ModifyMenu(hSubMenu, MI_DEMO_MULTI_LINE_FILES, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hMenuMultiLine, MENU_DEMO_MULTI_LINE_FILES);

   itemCount = std::size(gSampleFiles);
   if (!nppMessage(NPPM_ALLOCATECMDID, itemCount, (LPARAM)&itemIDStart)) return;

   nppMessage(NPPM_GETPLUGINHOMEPATH, MAX_PATH, (LPARAM)pluginSamplesDir);
   PathAppend(pluginSamplesDir, PLUGIN_FOLDER_NAME);
   PathAppend(pluginSamplesDir, L"Samples");

   HMENU hWhich;
   TCHAR sampleFile[MAX_PATH];

   for (size_t i{}; i < itemCount; i++) {
      switch (gSampleFiles[i].sample_type)
      {
         case SINGLE_REC:
            hWhich = hMenuSingleRec;
            break;

         case MULTI_REC:
            hWhich = hMenuMultiRec;
            break;

         case MULTI_LINE:
            hWhich = hMenuMultiLine;
            break;

         default:
            continue;
      }

      PathCombine(sampleFile, pluginSamplesDir, gSampleFiles[i].file_name.c_str());
      if (PathFileExists(sampleFile))
         AppendMenu(hWhich, MF_STRING, itemIDStart + i, gSampleFiles[i].display_name.c_str());
   }
}

void SubmenuManager::loadSampleFile(WPARAM wParam, LPARAM) {
   size_t cmdID{ LOWORD(wParam) - itemIDStart };
   if (cmdID < 0 || cmdID > itemCount) return;

   TCHAR sampleFile[MAX_PATH];

   PathCombine(sampleFile, pluginSamplesDir, gSampleFiles[cmdID].file_name.c_str());
   if (!PathFileExists(sampleFile)) return;

   nppMessage(NPPM_DOOPEN, NULL, (LPARAM)sampleFile);
   ShowVisualizerPanel(TRUE);

   _vizPanel.clearVisualize();
   _vizPanel.setDocFileType(getCurrentScintilla(), gSampleFiles[cmdID].file_type);
   _vizPanel.onBufferActivate();
}


HMENU SubmenuManager::getPluginSubMenu() {
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
