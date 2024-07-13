#pragma once

#include "../Utils.h"
#include "../ConfigIO.h"
#include "../NPP/StaticDialog.h"

extern NppData nppData;
extern ConfigIO _configIO;

class PreferencesDialog : public StaticDialog {
public:
   PreferencesDialog() : StaticDialog() {};
   ~PreferencesDialog() { if (hbr != NULL) DeleteObject(hbr); };

   void doDialog(HINSTANCE hInst);
   void refreshDarkMode();

   static COLORREF getPreferenceFoldLineColor();
   static void applyFoldLineColorAlpha();

private:
   INT_PTR CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
   void localize();
   void initCheckbox(int nIDButton, const string& preference, bool defaultVal);
   void setCheckbox(int nIDButton, const string& preference);

   void chooseColor();
   void setPreferenceFoldLineColor(COLORREF rgbColor);
   void displayFoldLineColor();
   INT_PTR colorStaticControl(WPARAM wParam, LPARAM lParam);
   void setFoldLineAlpha();

   HBRUSH hbr{};
};

