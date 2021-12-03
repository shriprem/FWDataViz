#pragma once

#include "../Utils.h"
#include "../ConfigIO.h"
#include "../NPP/StaticDialog.h"

extern NppData nppData;
extern ConfigIO _configIO;

class PreferencesDialog : public StaticDialog {
public:
   PreferencesDialog() : StaticDialog() {};
   void doDialog(HINSTANCE hInst);

private:
   INT_PTR CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
   void localize();
   void initCheckbox(int nIDButton, const string& preference, bool default);
   void setCheckbox(int nIDButton, const string& preference);
};

