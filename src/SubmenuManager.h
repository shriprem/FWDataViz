#pragma once

#include "PluginDefinition.h"
#include <vector>

class SubmenuManager {
public:
   void listSampleFiles();
   void loadSampleFile(WPARAM wParam, LPARAM lParam);

private:
   size_t itemCount;
   size_t itemIDStart;

   TCHAR pluginSamplesDir[MAX_PATH];

   static HMENU getPluginSubMenu();
};
