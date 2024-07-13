#pragma once

#include "PluginDefinition.h"
#include <vector>

class SubmenuManager {
public:
   void listSampleFiles();
   void loadSampleFile(WPARAM wParam, LPARAM lParam) const;
   void initSamplesPopup(HMENU hPopup);

private:
   static HMENU getPluginSubMenu();

   size_t itemCount;
   size_t itemIDStart;

   TCHAR pluginSamplesDir[MAX_PATH];
};
