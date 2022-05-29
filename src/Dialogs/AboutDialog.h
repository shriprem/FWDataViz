#pragma once

#include "../Utils.h"
#include "../NPP/StaticDialog.h"

using Utils::getVersionInfo;

extern NppData nppData;

class AboutDialog : public StaticDialog {
public:
   AboutDialog() : StaticDialog() {};
   void doDialog(HINSTANCE hInst);
   void refreshDarkMode();

private:
   INT_PTR CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);
   void localize();
};
