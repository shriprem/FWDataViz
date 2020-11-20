#pragma once

#include "PluginDefinition.h"
#include <vector>

class SamplesMenu {
public:
   void init();

private:
   size_t itemCount;
   int itemIDStart;

   TCHAR pluginSamplesDir[MAX_PATH];

   static HMENU getPluginSubMenu();
};
