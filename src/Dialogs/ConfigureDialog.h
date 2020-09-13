#pragma once

#include "../PluginDefinition.h"
#include "../NPP/StaticDialog.h"

extern NppData nppData;

class ConfigureDialog : public StaticDialog
{
public:
   ConfigureDialog() : StaticDialog() {};
   void doDialog(HINSTANCE hInst);

protected:
   INT_PTR CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);
   void localize();
};


