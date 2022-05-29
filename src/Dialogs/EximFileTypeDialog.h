#pragma once

#include "ConfigureDialog.h"
#include "ThemeDialog.h"
#include "../Utils.h"
#include "../ConfigIO.h"
#include "../NPP/StaticDialog.h"

extern NppData nppData;
extern ConfigIO _configIO;
extern ConfigureDialog _configDlg;
extern ThemeDialog _themeDlg;

class EximFileTypeDialog : public StaticDialog {
public:
   EximFileTypeDialog() : StaticDialog() {};
   void doDialog(HINSTANCE hInst);
   void initDialog(bool bExtract, bool bViz);
   void refreshDarkMode();
   void setFileTypeData(const wstring& ftConfig);

private:
   bool vizMode{}, extractMode{};

   INT_PTR CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);
   void localize();

   void appendExtractFile();
   void loadExtractFile();
   void saveExtractFile();

   wstring getEditControlText();
};
