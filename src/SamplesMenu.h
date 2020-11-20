#pragma once

#include "PluginDefinition.h"
#include <vector>

class SamplesMenu {
public:
   void init();
   void procCommand(WPARAM wParam, LPARAM lParam);

private:
   size_t itemCount;
   size_t itemIDStart;

   TCHAR pluginSamplesDir[MAX_PATH];

   static HMENU getPluginSubMenu();
};
